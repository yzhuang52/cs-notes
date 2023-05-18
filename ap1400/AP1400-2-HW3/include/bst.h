#ifndef BST_H
#define BST_H
#include <iostream>
#include <functional>


class BST
{
    public:
    class Node;
    BST();
    BST(BST& bst);
    BST(BST&& bst);
    Node*& get_root();
    void bfs(std::function<void(Node*& node)> func);
    std::size_t length() const;
    bool add_node(int value);
    Node** find_node(int value) const;
    Node** find_parrent(int value) const;
    Node** find_successor(int value) const;
    bool delete_node(int value);
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
void operator=(BST& bst1, BST& bst2);
void operator=(BST& bst1, BST&& bst2);
BST& operator++(BST& bst1);
BST operator++(BST& bst1, int);
#endif //BST_H