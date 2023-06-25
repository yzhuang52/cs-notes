//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// extendible_hash_table.cpp
//
// Identification: src/container/hash/extendible_hash_table.cpp
//
// Copyright (c) 2022, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include <sys/types.h>
#include <cassert>
#include <cstdlib>
#include <functional>
#include <list>
#include <memory>
#include <utility>

#include "container/hash/extendible_hash_table.h"
#include "container/hash/hash_function.h"
#include "storage/page/page.h"

namespace bustub {

template <typename K, typename V>
ExtendibleHashTable<K, V>::ExtendibleHashTable(size_t bucket_size)
    : global_depth_(0), bucket_size_(bucket_size), num_buckets_(1) {
      dir_.resize(num_buckets_, std::make_shared<Bucket>(bucket_size, 0));
    }

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::IndexOf(const K &key) -> size_t {
  int mask = (1 << global_depth_) - 1;
  return std::hash<K>()(key) & mask;
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetGlobalDepth() const -> int {
  std::scoped_lock<std::mutex> lock(latch_);
  return GetGlobalDepthInternal();
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetGlobalDepthInternal() const -> int {
  return global_depth_;
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetLocalDepth(int dir_index) const -> int {
  std::scoped_lock<std::mutex> lock(latch_);
  return GetLocalDepthInternal(dir_index);
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetLocalDepthInternal(int dir_index) const -> int {
  return dir_[dir_index]->GetDepth();
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetNumBuckets() const -> int {
  std::scoped_lock<std::mutex> lock(latch_);
  return GetNumBucketsInternal();
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetNumBucketsInternal() const -> int {
  return num_buckets_;
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Find(const K &key, V &value) -> bool {
  size_t index = IndexOf(key);
  return dir_[index]->Find(key, value);
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Remove(const K &key) -> bool {
  std::scoped_lock<std::mutex> lock(latch_);
  size_t index = IndexOf(key);
  return dir_[index]->Remove(key);
}

template <typename K, typename V>
void ExtendibleHashTable<K, V>::Insert(const K &key, const V &value) {
  // Check if key value pair exists
  // Try insert into bucket, return if succeed
  while (true) {
    size_t index = IndexOf(key);
    int local_depth = GetLocalDepth(index);
    int global_depth = GetGlobalDepth();
    std::scoped_lock<std::mutex> lock(latch_);
    if(dir_[index]->Insert(key, value)) {
      return;
    }
    // Bucket is full, needs to be splitted
    assert(global_depth >= local_depth);
    if (global_depth == local_depth) {
      global_depth += 1;
      global_depth_ = global_depth;
      auto original_size = dir_.size();
      dir_.resize(dir_.size() * 2);
      dir_[index]->IncrementDepth();
      for (int i = 0; i < static_cast<int>(original_size); i++) {
        dir_[original_size + i] = dir_[i];
      }
      RedistributeBucket(dir_[index]);
    } else {
      // if global depth > local depth, no need to double directory size
      dir_[index]->IncrementDepth();
      RedistributeBucket(dir_[index]);
    }
  }
  
}
template <typename K, typename V>
void ExtendibleHashTable<K, V>::RedistributeBucket(std::shared_ptr<Bucket> bucket) {
  // RedistributeBucket should split the bucket and reallocate key-value pair between 2 created buckets
  int depth = bucket->GetDepth();
  std::shared_ptr<Bucket> new_bucket = std::make_shared<Bucket>(bucket_size_, depth);
  int bucket_low_bit = GetKLSB(std::hash<K>()(bucket->GetItems().begin()->first), depth - 1);
  num_buckets_ += 1;
  for (size_t i = 0; i < dir_.size(); i++) {
    if (TestKLSB(i, depth - 1) && GetKLSB(i, depth - 1) == bucket_low_bit) {
      dir_[i] = new_bucket;
    }
  }

  for (auto it = bucket->GetItems().begin(); it != bucket->GetItems().end();) {
    if (TestKLSB(std::hash<K>()(it->first), depth - 1)) {
      new_bucket->Insert(it->first, it->second);
      it = bucket->GetItems().erase(it);
    } else {
      it++;
    }
  }


}
template <typename K, typename V>
auto ExtendibleHashTable<K, V>::TestKLSB(int n, int k) -> int {
  int mask = (1 << k);
  return n & mask; 
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetKLSB(int n, int k) -> int {
  int mask = (1 << k) - 1;
  return n & mask;
}
//===--------------------------------------------------------------------===//
// Bucket
//===--------------------------------------------------------------------===//
template <typename K, typename V>
ExtendibleHashTable<K, V>::Bucket::Bucket(size_t array_size, int depth) : size_(array_size), depth_(depth) {}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Bucket::Find(const K &key, V &value) -> bool {
  for (auto it = list_.begin(); it != list_.end(); it++) {
    if (it->first == key) {
      value = it->second;
      return true;
    }
  }
  return false;
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Bucket::Remove(const K &key) -> bool {
  if (!std::any_of(list_.begin(), list_.end(), [&](std::pair<K, V> p){return p.first==key;})) {
    return false;
  }
  list_.remove_if([&](std::pair<K, V> p){return p.first == key;});
  return true;
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Bucket::Insert(const K &key, const V &value) -> bool {
  if (IsFull()) {
    return false;
  }
  for (auto &pair: list_) {
    if (pair.first == key) {
      pair.second = value;
      return true;
    }
  }
  list_.push_back(std::pair<K, V>(key, value));
  return true;
}

template class ExtendibleHashTable<page_id_t, Page *>;
template class ExtendibleHashTable<Page *, std::list<Page *>::iterator>;
template class ExtendibleHashTable<int, int>;
// test purpose
template class ExtendibleHashTable<int, std::string>;
template class ExtendibleHashTable<int, std::list<int>::iterator>;

}  // namespace bustub
