// #include "primer/trie.h"
// #include <string_view>
// #include "common/exception.h"

// namespace bustub {


// template <class T>
// auto Trie::Get(std::string_view key) const -> const T * {
//   // You should walk through the trie to find the node corresponding to the key. If the node doesn't exist, return
//   // nullptr. After you find the node, you should use `dynamic_cast` to cast it to `const TrieNodeWithValue<T> *`. If
//   // dynamic_cast returns `nullptr`, it means the type of the value is mismatched, and you should return nullptr.
//   // Otherwise, return the value.

//   // if key[0] in root.childrens -> if key[1] in c1.childrens
//   // ... if key[len(key) - 1] in c[len(key)-1].childrens and c[len(key)-1] is terminal node
//   // return value
   
//   auto crawler = root_;
//   for (int i = 0; i < static_cast<int>(key.size()); i++)
//   {
//     char k = key[i];
//     if (crawler->children_.count(k) == 0) {
//       return nullptr;
//     }
//     crawler = crawler->children_.at(k);
//   }
//   auto value_node = std::dynamic_pointer_cast<TrieNodeWithValue<T>>(crawler);
//   if (value_node == nullptr) {
//     return nullptr;
//   }
//   return value_node->value_.get();
// }

// template <class T>
// auto Trie::Put(std::string_view key, T value) const -> Trie {
//   // Note that `T` might be a non-copyable type. Always use `std::move` when creating `shared_ptr` on that value.
//   // You should walk through the trie and create new nodes if necessary. If the node corresponding to the key already
//   // exists, you should create a new `TrieNodeWithValue`.
//   // auto new_trie = Trie(TrieClone(root_));
//   // auto crawler = new_trie.root_;
//   // for (int i = 0; i < static_cast<int>(key.size()) - 1; i++)
//   // {
//   //   char k = key[i];
//   //   if (crawler->children_.count(key[i]) == 0) {
//   //     // crawler->children_.at(k) = new TrieNode();
//   //   }
//   //   crawler = crawler->children_.at(k);
//   // }
//   // if (crawler->children_.count(key.back()) == 0) {
//   //   // crawler->children_.at(key.back()) = std::make_shared<TrieNodeWithValue<T>>(std::make_shared(value));
//   // } else {
//   //   // std::make_shared<TrieNodeWithValue<T>>(crawler->chidlren_.at(key.back()), std::make_shared<T>(value));
//   // }
//   // return new_trie;
//   throw NotImplementedException("Trie::Remove is not implemented.");
// }

// auto Trie::Remove(std::string_view key) const -> Trie {
//   throw NotImplementedException("Trie::Remove is not implemented.");

//   // You should walk through the trie and remove nodes if necessary. If the node doesn't contain a value any more,
//   // you should convert it to `TrieNode`. If a node doesn't have children any more, you should remove it.
// }

// // Below are explicit instantiation of template functions.
// //
// // Generally people would write the implementation of template classes and functions in the header file. However, we
// // separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
// // implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked up
// // by the linker.

// template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
// template auto Trie::Get(std::string_view key) const -> const uint32_t *;

// template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
// template auto Trie::Get(std::string_view key) const -> const uint64_t *;

// template auto Trie::Put(std::string_view key, std::string value) const -> Trie;
// template auto Trie::Get(std::string_view key) const -> const std::string *;

// // If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

// using Integer = std::unique_ptr<uint32_t>;

// template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
// template auto Trie::Get(std::string_view key) const -> const Integer *;

// template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
// template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

// }  // namespace bustub

#include "primer/trie.h"
#include <string_view>
#include "common/exception.h"

namespace bustub {

template <class T>
auto Trie::Get(std::string_view key) const -> const T * {
  auto crawler = root_;
  for (int i = 0; i < static_cast<int>(key.size()); i++)
  {
    char k = key[i];
    if (crawler->children_.count(k) == 0) {
      return nullptr;
    }
    crawler = crawler->children_.at(k);
  }
  auto value_node = std::dynamic_pointer_cast<const TrieNodeWithValue<T>>(crawler);

  if (value_node == nullptr) return nullptr;
  return value_node->value_.get();

  // You should walk through the trie to find the node corresponding to the key. If the node doesn't exist, return
  // nullptr. After you find the node, you should use `dynamic_cast` to cast it to `const TrieNodeWithValue<T> *`. If
  // dynamic_cast returns `nullptr`, it means the type of the value is mismatched, and you should return nullptr.
  // Otherwise, return the value.
}

template <class T>
auto Trie::Put(std::string_view key, T value) const -> Trie {
  // Note that `T` might be a non-copyable type. Always use `std::move` when creating `shared_ptr` on that value.
  Trie new_trie = new Trie();
  new_trie.root_ = std::shared_ptr<T>(std::move(root_->Clone()));
  auto crawler = root_;
  auto new_crawler = new_trie.root_;
  for (int i = 0; i < static_cast<int>(key.size()) - 1; i++)
  {
    int flag = 0;
    for (auto const& [k, child]: crawler->children_) {
      // key exists, create new node for existing node
      if (k == key[i]) {
        flag = 1;
        std::shared_ptr<TrieNode> new_node = std::shared_ptr(std::move(crawler->children_.at(k)->Clone()));
        new_crawler->children_[k] = new_node;
        break;
      }
    }
    if (flag == 0) {
      // key not exists, create new node
      std::shared_ptr<TrieNode> new_node = std::make_shared<TrieNode>();
      new_crawler->children_[key[i]] = new_node;
    }
    crawler = crawler->children_.at(key[i]);
    new_crawler = new_crawler->children_[key[i]];
  }
  if (new_crawler->children_.count(key.back()) == 0) {
    new_crawler->children_[key.back()] = std::make_shared<TrieNodeWithValue<T>>(std::make_shared(value));
  } else {
    new_crawler->children_[key.back()] = std::make_shared<TrieNodeWithValue<T>>(crawler->chidlren_.at(key.back()), std::make_shared<T>(value));
  }
  return new_trie;

  // You should walk through the trie and create new nodes if necessary. If the node corresponding to the key already
  // exists, you should create a new `TrieNodeWithValue`.
}

auto Trie::Remove(std::string_view key) const -> Trie {
  throw NotImplementedException("Trie::Remove is not implemented.");

  // You should walk through the trie and remove nodes if necessary. If the node doesn't contain a value any more,
  // you should convert it to `TrieNode`. If a node doesn't have children any more, you should remove it.
}

// Below are explicit instantiation of template functions.
//
// Generally people would write the implementation of template classes and functions in the header file. However, we
// separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
// implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked up
// by the linker.

template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint32_t *;

template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint64_t *;

template auto Trie::Put(std::string_view key, std::string value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const std::string *;

// If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

using Integer = std::unique_ptr<uint32_t>;

template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const Integer *;

template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

}  // namespace bustub