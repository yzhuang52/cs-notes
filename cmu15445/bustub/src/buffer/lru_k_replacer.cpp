//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_k_replacer.cpp
//
// Identification: src/buffer/lru_k_replacer.cpp
//
// Copyright (c) 2015-2022, Carnegie Mellon University Database Group
//

/*
LRU-K Replacer 用于存储 buffer pool 中 page 被引用的记录，
并根据引用记录来选出在 buffer pool 满时需要被驱逐的 page。
在普通的 LRU 中，我们仅需记录 page 最近一次被引用的时间，在驱逐时，选择最近一次引用时间最早的 page。
在 LRU-K 中，我们需要记录 page 最近 K 次被引用的时间。
假如 list 中所有 page 都被引用了大于等于 K 次，则比较最近第 K 次被引用的时间，驱逐最早的。
假如 list 中存在引用次数少于 K 次的 page，
则将这些 page 挑选出来，用普通的 LRU 来比较这些 page 第一次被引用的时间，驱逐最早的。
LRU-K Replacer 中的 page 有一个 evictable 属性，当一个 page 的 evicitable 为 false 时，
上述算法跳过此 page 即可。这里主要是为了上层调用者可以 pin 住一个 page，
对其进行一些读写操作，此时需要保证 page 驻留在内存中。
*/
//===----------------------------------------------------------------------===//

#include "buffer/lru_k_replacer.h"

namespace bustub {

LRUKReplacer::LRUKReplacer(size_t num_frames, size_t k) : replacer_size_(num_frames), k_(k) {}

auto LRUKReplacer::Evict(frame_id_t *frame_id) -> bool {
  // 按照LRU-K原则驱逐一个最不常用帧
  std::scoped_lock<std::mutex> lock(latch_);

  if (curr_size_ == 0) {
    return false;
  }

  // 倒序遍历history_list_（存放的是使用次数小于k次的）
  for (auto it = history_list_.rbegin(); it != history_list_.rend(); it++) {
    auto frame = *it;
    if (is_evictable_[frame]) {
      // 找到第一个可以被驱逐的帧，将其删除
      access_count_[frame] = 0;
      history_list_.erase(history_map_[frame]);
      history_map_.erase(frame);
      *frame_id = frame;
      curr_size_--;
      is_evictable_[frame] = false;
      return true;
    }
  }

  // 若history_list_中没有可以驱逐的，则从cache_list_中倒序寻找一个可以驱逐的页
  for (auto it = cache_list_.rbegin(); it != cache_list_.rend(); it++) {
    auto frame = *it;
    if (is_evictable_[frame]) {
      access_count_[frame] = 0;
      cache_list_.erase(cache_map_[frame]);
      cache_map_.erase(frame);
      *frame_id = frame;
      curr_size_--;
      is_evictable_[frame] = false;
      return true;
    }
  }

  return false;
}

void LRUKReplacer::RecordAccess(frame_id_t frame_id) {
  // frame_id被访问，更新相关记录
  std::scoped_lock<std::mutex> lock(latch_);

  if (frame_id > static_cast<int>(replacer_size_)) {
    throw std::exception();
  }
  // 当前帧使用次数+1
  access_count_[frame_id]++;

  if (access_count_[frame_id] == k_) {
    // 若使用次数增加后等于k，则将该页从history_list_中加入到cache_list_头部
    auto it = history_map_[frame_id];
    history_list_.erase(it);
    history_map_.erase(frame_id);

    cache_list_.push_front(frame_id);
    cache_map_[frame_id] = cache_list_.begin();
  } else if (access_count_[frame_id] > k_) {
    // 若使用次数增加后大于k，则将该页从cache_list_中移动到cache_list_头部
    if (cache_map_.count(frame_id) != 0U) {
      auto it = cache_map_[frame_id];
      cache_list_.erase(it);
    }
    cache_list_.push_front(frame_id);
    cache_map_[frame_id] = cache_list_.begin();
  } else {
    // 若使用次数增加后小于k，则判断其是否在history_list_中，若不存在，将其加入history_list_头部
    if (history_map_.count(frame_id) == 0U) {
      history_list_.push_front(frame_id);
      history_map_[frame_id] = history_list_.begin();
    }
  }
}

void LRUKReplacer::SetEvictable(frame_id_t frame_id, bool set_evictable) {
  // 设置某个帧是否可被驱逐，更新相关记录
  std::scoped_lock<std::mutex> lock(latch_);
  if (frame_id > static_cast<int>(replacer_size_)) {
    throw std::exception();
  }

  if (access_count_[frame_id] == 0) {
    return;
  }

  if (!is_evictable_[frame_id] && set_evictable) {
    curr_size_++;
  }
  if (is_evictable_[frame_id] && !set_evictable) {
    curr_size_--;
  }
  is_evictable_[frame_id] = set_evictable;
}

void LRUKReplacer::Remove(frame_id_t frame_id) {
  // 删除一个具体的帧，无论其是否满足LRU-K的驱逐原则
  std::scoped_lock<std::mutex> lock(latch_);

  if (frame_id > static_cast<int>(replacer_size_)) {
    throw std::exception();
  }

  auto cnt = access_count_[frame_id];
  if (cnt == 0) {
    return;
  }
  if (!is_evictable_[frame_id]) {
    throw std::exception();
  }

  // 根据frame_id的使用次数判断应该去history_list_还是cache_list_删除
  if (cnt < k_) {
    history_list_.erase(history_map_[frame_id]);
    history_map_.erase(frame_id);

  } else {
    cache_list_.erase(cache_map_[frame_id]);
    cache_map_.erase(frame_id);
  }
  curr_size_--;
  access_count_[frame_id] = 0;
  is_evictable_[frame_id] = false;
}

auto LRUKReplacer::Size() -> size_t {
  std::scoped_lock<std::mutex> lock(latch_);
  return curr_size_;
}

}  // namespace bustub
