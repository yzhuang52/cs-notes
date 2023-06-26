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
//===----------------------------------------------------------------------===//

#include "buffer/lru_k_replacer.h"
#include <stdexcept>

namespace bustub {

LRUKReplacer::LRUKReplacer(size_t num_frames, size_t k) : replacer_size_(0), k_(k) {
    // history_.resize(num_frames);
    // cache_.resize(num_frames);
}

auto LRUKReplacer::Evict(frame_id_t *frame_id) -> bool {
    if (!history_.empty()) {
        for (auto entry = history_.rbegin(); entry != history_.rend(); entry++) {
            if ((*entry)->second.second) {
                *frame_id = (*entry)->first;
                history_.erase((entry + 1).base());
                replacer_size_ -= 1;
                return true;
            }
        }
    }

    if (!cache_.empty()) {
        for (auto entry = cache_.rbegin(); entry != cache_.rend(); entry++) {
            if ((*entry)->second.second) {
                *frame_id = (*entry)->first;
                cache_.erase((entry + 1).base());
                replacer_size_ -= 1;
                return true;
            }
        }
    }
    return false;
}

void LRUKReplacer::RecordAccess(frame_id_t frame_id) {
    for (auto &entry: history_) {
        if (entry && entry->first == frame_id) {
            entry->second.first += 1;
            if (entry->second.first >= k_) {
                cache_.insert(cache_.begin(), entry);
                history_.erase(std::remove(history_.begin(), history_.end(), entry), history_.end());
            }
            return;
        }
    }

    for (auto &entry: cache_) {
        if (entry && entry->first == frame_id) {
            entry->second.first += 1;
            auto temp = entry;
            cache_.erase(std::remove(cache_.begin(), cache_.end(), entry), cache_.end());
            cache_.insert(cache_.begin(), temp);
            return;
        }
    }
    std::pair<size_t, bool> info = std::make_pair(1, true);
    std::shared_ptr<std::pair<frame_id_t, std::pair<size_t, bool>>> new_entry = std::make_shared<std::pair<frame_id_t, std::pair<size_t, bool>>>(frame_id, info);
    history_.insert(history_.begin(), new_entry);
    replacer_size_ += 1;
}

void LRUKReplacer::SetEvictable(frame_id_t frame_id, bool set_evictable) {
    for (auto &entry: history_) {
        if (entry && entry->first == frame_id) {
            if (entry->second.second != set_evictable && entry->second.second) {
                replacer_size_ -= 1;
            } else if (entry->second.second != set_evictable && !entry->second.second) {
                replacer_size_ += 1;
            }
            entry->second.second = set_evictable;
            return;
        }
    }
    for (auto & entry: cache_) {
        if (entry && entry->first == frame_id) {
            if (entry->second.second != set_evictable && entry->second.second) {
                replacer_size_ -= 1;
            } else if (entry->second.second != set_evictable && !entry->second.second) {
                replacer_size_ += 1;
            }
            entry->second.second = set_evictable;
            return;
        }
    }
}

void LRUKReplacer::Remove(frame_id_t frame_id) {
    for (auto &entry: history_) {
        if (entry && entry->first == frame_id) {
            if (!entry->second.second) {
                throw std::runtime_error("Frame not evictable!\n");
            }
            (void) std::remove(history_.begin(), history_.end(), entry);
            replacer_size_ -= 1;
            return;
        }
    }
    for (auto &entry: cache_) {
        if (entry && entry->first == frame_id) {
            if (!entry->second.second) {
                throw std::runtime_error("Frame not evictable!\n");
            }
            (void) std::remove(history_.begin(), history_.end(), entry);
            replacer_size_ -= 1;
            return;
        }
    }
}

auto LRUKReplacer::Size() -> size_t { return replacer_size_; }

}  // namespace bustub

