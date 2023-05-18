#include "bst.h"

Node::Node(int value, Node* left, Node* right) {
    this->value = value;
    this->left = left;
    this->right = right;
}

Node::Node() {
    this->left = nullptr;
    this->right = nullptr;
    this->value = 0;
}

Node::Node(const Node& node) {
    this->value = node.value;
    this->left = node.left;
    this->right = node.right;
}





