#ifndef BST_H
#define BST_H
#include <iostream>
#include <functional>
#include <queue>
#include <iomanip>
#include <vector>

class BST
{
    public:
    class Node;
    BST();
    BST(BST& bst);
    BST(BST&& bst);
    BST(std::initializer_list<int> list);
    Node*& get_root();
    void bfs(std::function<void(Node*& node)> func) const;
    std::size_t length() const;
    bool add_node(int value);
    Node** find_node(int value) const;
    Node** find_parrent(int value) const;
    Node** find_successor(int value) const;
    BST& operator=(const BST& bst);
    BST& operator=(BST&& bst);
    bool delete_node(int value);
    BST& operator++();
    BST operator++(int);
    ~BST();
private:
    Node* root;
};

class BST::Node
{
public:
    Node(int value, Node* left, Node* right);
    Node();
    Node(const Node& node);

    int value;
    Node* left;
    Node* right;
};

bool operator>(const BST::Node& node, int num);
bool operator>=(const BST::Node& node, int num);
bool operator<(const BST::Node& node, int num);
bool operator<=(const BST::Node& node, int num);
bool operator==(const BST::Node& node, int num);
bool operator<(int num, const BST::Node& node);
bool operator<=(int num, const BST::Node& node);
bool operator>(int num, const BST::Node& node);
bool operator>=(int num, const BST::Node& node);
bool operator==(int num, const BST::Node& node);
std::ostream& operator<<(std::ostream& out, const BST::Node& node);
std::ostream& operator<<(std::ostream& out, const BST& bst);

#endif //BST_H

