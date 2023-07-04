// //===----------------------------------------------------------------------===//
// //
// //                         BusTub
// //
// // buffer_pool_manager_instance.cpp
// //
// // Identification: src/buffer/buffer_pool_manager.cpp
// //
// // Copyright (c) 2015-2021, Carnegie Mellon University Database Group
// //
// //===----------------------------------------------------------------------===//

// #include "buffer/buffer_pool_manager_instance.h"

// #include "common/config.h"
// #include "common/exception.h"
// #include "common/macros.h"

// namespace bustub {

// BufferPoolManagerInstance::BufferPoolManagerInstance(size_t pool_size, DiskManager *disk_manager, size_t replacer_k,
//                                                      LogManager *log_manager)
//     : pool_size_(pool_size), disk_manager_(disk_manager), log_manager_(log_manager) {
//   // we allocate a consecutive memory space for the buffer pool
//   pages_ = new Page[pool_size_];
//   page_table_ = new ExtendibleHashTable<page_id_t, frame_id_t>(bucket_size_);
//   replacer_ = new LRUKReplacer(pool_size, replacer_k);

//   // Initially, every page is in the free list.
//   for (size_t i = 0; i < pool_size_; ++i) {
//     free_list_.emplace_back(static_cast<int>(i));
//   }
// }

// BufferPoolManagerInstance::~BufferPoolManagerInstance() {
//   delete[] pages_;
//   delete page_table_;
//   delete replacer_;
// }

// auto BufferPoolManagerInstance::NewPgImp(page_id_t *page_id) -> Page * {
//   std::lock_guard<std::mutex> lk(latch_);
//   frame_id_t frame_id;
//   *page_id = AllocatePage();
//   if (free_list_.empty()) {
//     // Try to evict from buffer
//     if (!replacer_->Evict(&frame_id)) {
//       return nullptr;
//     }
//     if (pages_[frame_id].IsDirty()) {
//       FlushPage(pages_[frame_id].GetPageId());
//     }
//     pages_[frame_id].ResetMemory();
//     page_table_->Remove(pages_[frame_id].GetPageId());
//     // All pages are pinned
//   } else {
//     // Create a new Page and place it to the first free frame
//     frame_id = free_list_.front();
//     free_list_.pop_front();
//   }
//   pages_[frame_id].page_id_ = *page_id;
//   pages_[frame_id].pin_count_ = 1;
//   page_table_->Insert(*page_id, frame_id);
//   replacer_->RecordAccess(frame_id);
//   replacer_->SetEvictable(frame_id, false);
//   return &pages_[frame_id];
// }

// auto BufferPoolManagerInstance::FetchPgImp(page_id_t page_id) -> Page * {
//   std::lock_guard<std::mutex> lk(latch_);
//   frame_id_t frame_id;
//   // Search page in buffer
//   if (page_table_->Find(page_id, frame_id)) {
//     pages_[frame_id].pin_count_ += 1;
//     replacer_->RecordAccess(frame_id);
//     replacer_->SetEvictable(frame_id, false);
//     return &pages_[frame_id];
//   }
//   // Not find in buffer, try to replace page
//   // Find a replacement frame
//   if (!free_list_.empty()) {
//     frame_id = free_list_.front();
//     free_list_.pop_front();
//   } else {
//     if (!replacer_->Evict(&frame_id)) {
//       return nullptr;
//     }
//     if (pages_[frame_id].IsDirty()) {
//       FlushPage(pages_[frame_id].GetPageId());
//     }
//     page_table_->Remove(pages_[frame_id].GetPageId());
//     pages_[frame_id].ResetMemory();
//   }
//   page_table_->Insert(page_id, frame_id);
//   pages_[frame_id].page_id_ = page_id;
//   pages_[frame_id].pin_count_ = 1;
//   disk_manager_->ReadPage(page_id, pages_[frame_id].data_);
//   replacer_->RecordAccess(frame_id);
//   replacer_->SetEvictable(frame_id, false);
//   return &pages_[frame_id];
// }

// auto BufferPoolManagerInstance::UnpinPgImp(page_id_t page_id, bool is_dirty) -> bool {
//   std::lock_guard<std::mutex> lk(latch_);
//   frame_id_t frame_id;
//   if (!page_table_->Find(page_id, frame_id) || pages_[frame_id].GetPinCount() == 0) {
//     return false;
//   }
//   if (is_dirty) {
//     pages_[frame_id].is_dirty_ = is_dirty;
//   }
//   pages_[frame_id].pin_count_ -= 1;
//   if (pages_[frame_id].GetPinCount() == 0) {
//     replacer_->SetEvictable(frame_id, true);
//   }
//   return true;
// }

// auto BufferPoolManagerInstance::FlushPgImp(page_id_t page_id) -> bool {
//   if (page_id == INVALID_PAGE_ID) {
//     return false;
//   }
//   frame_id_t frame_id;
//   if (!page_table_->Find(page_id, frame_id)) {
//     return false;
//   }
//   disk_manager_->WritePage(page_id, pages_[frame_id].GetData());
//   pages_[frame_id].is_dirty_ = false;
//   return true;
// }

// void BufferPoolManagerInstance::FlushAllPgsImp() {
//   std::lock_guard<std::mutex> lk(latch_);
//   for (size_t frame_id = 0; frame_id < pool_size_; frame_id++) {
//     FlushPgImp(pages_[frame_id].GetPageId());
//   }
// }

// auto BufferPoolManagerInstance::DeletePgImp(page_id_t page_id) -> bool {
//   std::lock_guard<std::mutex> lk(latch_);
//   frame_id_t frame_id;
//   if (!page_table_->Find(page_id, frame_id) || pages_[frame_id].GetPinCount() > 0) {
//     return false;
//   }
//   replacer_->Remove(frame_id);
//   pages_[frame_id].ResetMemory();
//   pages_[frame_id].page_id_ = INVALID_PAGE_ID;
//   pages_[frame_id].pin_count_ = 0;
//   pages_[frame_id].is_dirty_ = false;

//   page_table_->Remove(page_id);
//   free_list_.push_back(frame_id);
//   DeallocatePage(page_id);
//   return true;
// }

// auto BufferPoolManagerInstance::AllocatePage() -> page_id_t { return next_page_id_++; }

// }  // namespace bustub

//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// buffer_pool_manager_instance.cpp
//
// Identification: src/buffer/buffer_pool_manager.cpp
//
// Copyright (c) 2015-2021, Carnegie Mellon University Database Group

/*
Buffer Pool Manager 里有几个重要的成员：

    pages：buffer pool 中缓存 pages 的指针数组
    disk_manager：框架提供，可以用来读取 disk 上指定 page id 的 page 数据，或者向 disk 上给定 page id 对应的 page
里写入数据 page_table：刚才实现的 Extendible Hash Table，用来将 page id 映射到 frame id，即 page 在 buffer pool 中的位置
    replacer：刚才实现的 LRU-K Replacer，在需要驱逐 page 腾出空间时，告诉我们应该驱逐哪个 pagefree_list：空闲的 frame
列表
*/
//
//===----------------------------------------------------------------------===//

#include "buffer/buffer_pool_manager_instance.h"

#include "common/exception.h"
#include "common/macros.h"

namespace bustub {

BufferPoolManagerInstance::BufferPoolManagerInstance(size_t pool_size, DiskManager *disk_manager, size_t replacer_k,
                                                     LogManager *log_manager)
    : pool_size_(pool_size), disk_manager_(disk_manager), log_manager_(log_manager) {
  // we allocate a consecutive memory space for the buffer pool
  pages_ = new Page[pool_size_];
  page_table_ = new ExtendibleHashTable<page_id_t, frame_id_t>(bucket_size_);
  replacer_ = new LRUKReplacer(pool_size, replacer_k);

  // Initially, every page is in the free list.
  for (size_t i = 0; i < pool_size_; ++i) {
    free_list_.emplace_back(static_cast<int>(i));
  }
}

BufferPoolManagerInstance::~BufferPoolManagerInstance() {
  delete[] pages_;
  delete page_table_;
  delete replacer_;
}

/*
New Page
上层调用者希望新建一个 page，调用 NewPgImp。

如果当前 buffer pool 已满并且所有 page 都是 unevictable 的，直接返回。否则：

如果当前 buffer pool 里还有空闲的 frame，创建一个空的 page 放置在 frame 中。
如果当前 buffer pool 里没有空闲的 frame，但有 evitable 的 page，利用 LRU-K Replacer 获取可以驱逐的 frame id，将 frame
中原 page 驱逐，并创建新的 page 放在此 frame 中。 驱逐时，如果当前 frame 为 dirty(发生过写操作)，将对应的 frame 里的
page 数据写入 disk，并重置 dirty 为 false。 清空 frame 数据，并移除 page_table 里的 page id，移除 replacer
里的引用记录。 如果当前 frame 不为 dirty，直接清空 frame 数据，并移除 page_table 里的 page id，移除 replacer
里的引用记录。


在 replacer 里记录 frame 的引用记录，并将 frame 的 evictable 设为 false。
因为上层调用者拿到 page 后可能需要对其进行读写操作，此时 page 必须驻留在内存中。

使用 AllocatePage 分配一个新的 page id(从0递增)。
将此 page id 和存放 page 的 frame id 插入 page_table。
page 的 pin_count 加 1。
*/
auto BufferPoolManagerInstance::NewPgImp(page_id_t *page_id) -> Page * {
  std::scoped_lock<std::mutex> lock(latch_);

  bool is_free_page = false;
  for (size_t i = 0; i < pool_size_; i++) {
    if (pages_[i].GetPinCount() == 0) {
      is_free_page = true;
      break;
    }
  }
  if (!is_free_page) {
    // 如果所有page都被pin住了，返回空指针
    return nullptr;
  }

  *page_id = AllocatePage();

  frame_id_t frame_id;

  if (!free_list_.empty()) {
    // 如果当前 buffer pool 里还有空闲的 frame，创建一个空的 page 放置在 frame 中。
    frame_id = free_list_.front();
    free_list_.pop_front();
  } else {
    // 如果当前 buffer pool 里没有空闲的 frame，但有 evitable 的 page，利用 LRU-K Replacer 获取可以驱逐的 frame id，
    // 将 frame 中原 page 驱逐，并创建新的 page 放在此 frame 中。
    // assert(replacer_->Evict(&frame_id));
    replacer_->Evict(&frame_id);
    page_id_t evicted_page_id = pages_[frame_id].GetPageId();

    if (pages_[frame_id].IsDirty()) {
      // 驱逐时，如果当前 frame 为 dirty(发生过写操作)，将对应的 frame 里的 page 数据写入 disk，并重置 dirty 为 false。
      disk_manager_->WritePage(evicted_page_id, pages_[frame_id].GetData());
      pages_[frame_id].is_dirty_ = false;
    }
    // 清空 frame 数据，并移除 page_table 里的 page id
    pages_[frame_id].ResetMemory();
    page_table_->Remove(evicted_page_id);
  }

  // 使用 AllocatePage 分配一个新的 page id(从0递增)。
  // 将此 page id 和存放 page 的 frame id 插入 page_table。
  // page 的 pin_count 加 1。
  page_table_->Insert(*page_id, frame_id);

  // 因为上层调用者拿到 page 后可能需要对其进行读写操作，此时 page 必须驻留在内存中。
  pages_[frame_id].page_id_ = *page_id;
  pages_[frame_id].pin_count_ = 1;

  // 在 replacer 里记录 frame 的引用记录，并将 frame 的 evictable 设为 false。
  replacer_->RecordAccess(frame_id);
  replacer_->SetEvictable(frame_id, false);

  return &pages_[frame_id];
}

/*
Fetch Page
上层调用者给定一个 page id，Buffer Pool Manager 返回对应的 page 指针。调用 FetchPgImp

假如可以在 buffer pool 中找到对应 page，直接返回。

否则需要将磁盘上的 page 载入内存，也就是放进 buffer pool。

如果当前 buffer pool 已满并且所有 page 都是 unevictable 的，直接返回空指针。
否则同 New Page 操作，先尝试在 free list 中找空闲的 frame 存放需要读取的 page，如果没有 frame 空闲，就驱逐一张
page。获得一个空闲的 frame。

通过 disk_manager 读取 page id 对应 page 的数据，存放在 frame 中。
在 replacer 里记录引用，将 evictable 设为 false，将 page id 插入 page_table，page 的 pin_count 加 1。

流程还是比较简单的，总的来说就是 buffer pool 里没空位也腾不出空位，直接返回，暂时处理不了请求，
如果有空位，就先用空位，没空位但可以驱逐，就驱逐一个 page 腾出空位。这样就可以在内存中缓存一个 page 方便上层调用者操作。
同时，还需要同步一些信息，比如 page_table 和 replacer，驱逐 page 时，如果是 dirty page 也需要先将其数据写回 disk。
*/
auto BufferPoolManagerInstance::FetchPgImp(page_id_t page_id) -> Page * {
  std::scoped_lock<std::mutex> lock(latch_);

  frame_id_t frame_id;
  if (page_table_->Find(page_id, frame_id)) {
    // 假如可以在 buffer pool 中找到对应 page，则更新该页面的访问记录，将该page设为不可驱逐，直接返回该页面。
    pages_[frame_id].pin_count_++;
    replacer_->RecordAccess(frame_id);
    replacer_->SetEvictable(frame_id, false);
    return &pages_[frame_id];
  }

  // 如果当前 buffer pool 已满并且所有 page 都是 unevictable 的，直接返回空指针。
  bool is_free_page = false;
  for (size_t i = 0; i < pool_size_; i++) {
    if (pages_[i].GetPinCount() == 0) {
      is_free_page = true;
      break;
    }
  }
  if (!is_free_page) {
    return nullptr;
  }

  // 否则同 New Page 操作，先尝试在 free list 中找空闲的 frame 存放需要读取的 page，
  // 如果没有 frame 空闲，就驱逐一张 page。获得一个空闲的 frame。
  if (!free_list_.empty()) {
    frame_id = free_list_.front();
    free_list_.pop_front();
  } else {
    // assert(replacer_->Evict(&frame_id));
    replacer_->Evict(&frame_id);
    page_id_t evicted_page_id = pages_[frame_id].GetPageId();

    if (pages_[frame_id].IsDirty()) {
      disk_manager_->WritePage(evicted_page_id, pages_[frame_id].GetData());
      pages_[frame_id].is_dirty_ = false;
    }
    pages_[frame_id].ResetMemory();
    page_table_->Remove(evicted_page_id);
  }

  // 通过 disk_manager 读取 page id 对应 page 的数据，存放在 frame 中。
  // 在 replacer 里记录引用，将 evictable 设为 false，将 page id 插入 page_table，page 的 pin_count 加 1。
  page_table_->Insert(page_id, frame_id);

  pages_[frame_id].page_id_ = page_id;
  pages_[frame_id].pin_count_ = 1;
  disk_manager_->ReadPage(page_id, pages_[frame_id].GetData());

  replacer_->RecordAccess(frame_id);
  replacer_->SetEvictable(frame_id, false);

  return &pages_[frame_id];
}

/*
当上层调用者新建一个 page 或者 fecth 一个 page 时，Buffer Pool Manager 会自动 pin 一下这个 page。
接下来上层调用者对这个 page 进行一系列读写操作，操作完之后调用 unpin，告诉 Buffer Pool Manager，
这个 page 我用完了，你可以把它直接丢掉或者 flush 掉了（也不一定真的可以，可能与此同时有其他调用者也在使用这个 page，
具体能不能 unpin 掉要 Buffer Pool Manager 在内部判断一下 page 的 pin_count 是否为 0）。
调用 unpin 时，同时传入一个 is_dirty 参数，告诉 Buffer Pool Manager 我刚刚对这个 page 进行的是读操作还是写操作。
需要注意的是，Buffer Pool Manager 不能够直接将 page 的 dirty flag 设为 is_dirty。
假设原本 dirty flag 为 true，则不能改变，代表其他调用者进行过写操作。
只有原本 dirty flag 为 false 时，才能将 dirty flag 直接设为 is_dirty。
*/
auto BufferPoolManagerInstance::UnpinPgImp(page_id_t page_id, bool is_dirty) -> bool {
  std::scoped_lock<std::mutex> lock(latch_);

  frame_id_t frame_id;
  if (!page_table_->Find(page_id, frame_id)) {
    return false;
  }

  if (pages_[frame_id].GetPinCount() <= 0) {
    return false;
  }

  if (is_dirty) {
    pages_[frame_id].is_dirty_ = is_dirty;
  }

  pages_[frame_id].pin_count_--;

  if (pages_[frame_id].pin_count_ == 0) {
    replacer_->SetEvictable(frame_id, true);
  }

  return true;
}

auto BufferPoolManagerInstance::FlushPgImp(page_id_t page_id) -> bool {
  if (page_id == INVALID_PAGE_ID) {
    return false;
  }

  frame_id_t frame_id;

  if (!page_table_->Find(page_id, frame_id)) {
    return false;
  }

  disk_manager_->WritePage(page_id, pages_[frame_id].data_);
  return true;
}

void BufferPoolManagerInstance::FlushAllPgsImp() {
  std::scoped_lock<std::mutex> lock(latch_);
  for (size_t frame_id = 0; frame_id < pool_size_; frame_id++) {
    FlushPgImp(pages_[frame_id].GetPageId());
  }
}

auto BufferPoolManagerInstance::DeletePgImp(page_id_t page_id) -> bool {
  std::scoped_lock<std::mutex> lock(latch_);

  frame_id_t frame_id;
  if (!page_table_->Find(page_id, frame_id)) {
    return true;
  }

  if (pages_[frame_id].GetPinCount() > 0) {
    return false;
  }

  replacer_->Remove(frame_id);

  pages_[frame_id].ResetMemory();
  pages_[frame_id].page_id_ = INVALID_PAGE_ID;
  pages_[frame_id].pin_count_ = 0;
  pages_[frame_id].is_dirty_ = false;

  page_table_->Remove(page_id);
  free_list_.push_back(frame_id);
  DeallocatePage(page_id);

  return true;
}

auto BufferPoolManagerInstance::AllocatePage() -> page_id_t { return next_page_id_++; }

}  // namespace bustub
