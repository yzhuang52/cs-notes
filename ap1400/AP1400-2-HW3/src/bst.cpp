#include "bst.h"

BST::Node::Node(int value, Node* left, Node* right) {
    this->value = value;
    this->left = left;
    this->right = right;
}

BST::Node::Node() {
    this->left = nullptr;
    this->right = nullptr;
    this->value = 0;
}

BST::Node::Node(const Node& node) {
    this->value = node.value;
    this->left = node.left;
    this->right = node.right;
}

bool operator>(const BST::Node& node, int num) {
    return node.value > num;
}

bool operator>=(const BST::Node& node, int num) {
    return node.value >= num;
}

bool operator<(const BST::Node& node, int num) {
    return node.value < num;
}

bool operator<=(const BST::Node& node, int num) {
    return node.value <= num;
}

bool operator==(const BST::Node& node, int num) {
    return node.value == num;
}

bool operator<(int num, const BST::Node& node) {
    return num < node.value;
}

bool operator<=(int num, const BST::Node& node) {
    return num <= node.value;
}

bool operator>(int num, const BST::Node& node) {
    return num > node.value;
}

bool operator>=(int num, const BST::Node& node) {
    return num >= node.value;
}

bool operator==(int num, const BST::Node& node) {
    return num == node.value;
}

std::ostream& operator<<(std::ostream& out, const BST::Node& node) {
    out << &node << "  " << node.value << "  " << &node.left  << "  " << &node.right << std::endl;
    return out;
}

BST::BST() {
    this->root = nullptr;
}

BST::BST(BST& bst) {
    this->root = bst.root;
}

BST::BST(BST&& bst) {
    this->root = bst.root;
}

BST::Node*& BST::get_root() {
    return this->root;
}

void BST::bfs(std::function<void(BST::Node*& node)> func) {

}

std::size_t BST::length() const {
    return 1;
}

bool BST::add_node(int value) {
    return 1;
}

BST::Node** BST::find_node(int value) const {
    return nullptr;
}

BST::Node** BST::find_parrent(int value) const {
    return nullptr;
}

BST::Node** BST::find_successor(int value) const {
    return nullptr;
}

bool BST::delete_node(int value) {
    return true;
}

std::ostream& operator<<(std::ostream& out, BST& bst) {

    return out;
}

void operator=(BST& bst1, BST& bst2) {
    bst1.root = bst2.root;
}

void operator=(BST& bst1, BST&& bst2) {
    bst1.root = bst2.root;
}




