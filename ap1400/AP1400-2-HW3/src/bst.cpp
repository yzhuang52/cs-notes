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
    out <<&node << std::setw(20)
    << "=> value:"<< node.value << std::setw(20)
    << "left:" << &node.left  << std::setw(20)
    << "right:" << &node.right;
    return out;
}

BST::BST() {
    this->root = nullptr;
}

BST::Node* copy(BST::Node* root) {
    if (root == nullptr)
    {
        return nullptr;
    }
    BST::Node* newRoot = new BST::Node(root->value, nullptr, nullptr);
    newRoot->left = copy(root->left);
    newRoot->right = copy(root->right);
    return newRoot;
}

BST::BST(BST& bst) {
    if (bst.root == nullptr)
    {
        this->root = nullptr;
    } else 
    {
        this->root = copy(bst.root);
    }
}

BST::BST(BST&& bst) {
    this->root = bst.root;
    bst.root = nullptr;
}

BST::Node*& BST::get_root() {
    return this->root;
}

void BST::bfs(std::function<void(BST::Node*& node)> func) const {
    std::queue<Node*> q;
    if (this->root != nullptr)
    {        
        q.push(this->root);
        while (!q.empty())
        {
            Node* node = q.front();
            q.pop();
            func(node);
            if (node->left != nullptr)
            {
                q.push(node->left);
            }
            if (node->right != nullptr)
            {
                q.push(node->right);
            }
        }
    }
}

int num(BST::Node* node) {
    if (node == nullptr)
    {
        return 0;
    }
    return 1 + num(node->left) + num(node->right);
}
std::size_t BST::length() const{
    return num(this->root);
}

bool BST::add_node(int value) {
    // if root is null, create assign value for root
    if (this->root == nullptr)
    {
        Node* node = new Node(value, nullptr, nullptr);
        this->root = node;
        return true;
    } 
    // else, find the place for value and insert it
    Node* p = this->root;
    while (p != nullptr) 
    {
        if (p->value > value) {
            if (p->left == nullptr) 
            {
                // find place
                p->left = new Node(value, nullptr, nullptr);
                break;
            } else 
            {
                // value should be left side
                p = p->left;
            }
        } else if (p->value < value) 
        {
            if (p->right == nullptr)
            {
                // find place
                p->right = new Node(value, nullptr, nullptr);
                break;
            } else 
            {   
                // value should be right side
                p = p->right;
            }
        } else 
        {
            // same value cannot be inserted twice
            return false;
        }
    }
    return true;
}

BST::Node** BST::find_node(int value) const {
    Node* p = this->root;
    while (p != nullptr)
    {
        if (p->value == value)
        {
            // find it
            auto res = new Node* {p};
            return res;
        } else if (p->value < value)
        {
            // node should be on the right side
            if (p->right == nullptr)
            {
                // not find
                break;
            }
            p = p->right;
        } else 
        {
            if (p->left == nullptr)
            {
                // not find
                break;
            }
            p = p->left;
        }
    }
    return nullptr;
}

BST::Node** BST::find_parrent(int value) const {
    Node* prev;
    Node* p = this->root;
    while (p != nullptr)
    {
        if (p->value == value)
        {
            // find it
            auto res = new Node* {prev};
            return res;
        } else if (p->value < value)
        {
            // node should be on the right side
            if (p->right == nullptr)
            {
                // not find
                break;
            }
            prev = p;
            p = p->right;
        } else 
        {
            if (p->left == nullptr)
            {
                // not find
                break;
            }
            prev = p;
            p = p->left;
        }
    }
    return nullptr;
}

BST::Node** BST::find_successor(int value) const {
    Node* prev;
    Node* p = this->root;
    while (p != nullptr)
    {
        if (p->value == value)
        {
            // find it
            if (p->left != nullptr)
            {
                p = p->left;
            } else 
            {
                break;
            }
            while (p->right != nullptr)
            {
                p = p->right;
            }
            auto res = new Node*(p);
            return res;
        } else if (p->value < value)
        {
            // node should be on the right side
            if (p->right == nullptr)
            {
                // not find
                break;
            }
            prev = p;
            p = p->right;
        } else 
        {
            if (p->left == nullptr)
            {
                // not find
                break;
            }
            prev = p;
            p = p->left;
        }
    }
    return nullptr;
}

bool BST::delete_node(int value) {
    Node* prev;
    Node* p = this->root;
    while (p != nullptr)
    {
        if (p->value == value)
        {
            // leaf node, delete it
            if (p->right == nullptr && p->left == nullptr)
            {
                if (prev->right->value == value)
                {
                    prev->right = nullptr;
                    delete p;
                } else 
                {
                    prev->left = nullptr;
                    delete p;
                }
                return true;
            }
            // node with one child
            if (p->right == nullptr || p->left == nullptr)
            {
                if (prev->right->value == value)
                {
                    if (p->right != nullptr) 
                    {
                        prev->right = p->right;
                        delete p;
                    } else 
                    {
                        prev->right = p->left;
                        delete p;
                    }
                } else 
                {
                    if (p->right != nullptr) 
                    {
                        prev->left = p->right;
                        delete p;
                    } else 
                    {
                        prev->left = p->left;
                        delete p;
                    }
                }
                return true;
            }
            // node with two child
            if (p->right != nullptr && p->left != nullptr)
            {
                Node** successor{this->find_successor(value)};
                int successor_value = (*successor)->value;
                this->delete_node((*successor)->value);
                p->value = successor_value;
                return true;
            }
            
        } else if (p->value < value)
        {
            // node should be on the right side
            if (p->right == nullptr)
            {
                // not find
                break;
            }
            prev = p;
            p = p->right;
        } else 
        {
            if (p->left == nullptr)
            {
                // not find
                break;
            }
            prev = p;
            p = p->left;
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& out, const BST& bst) {
    out << "********************************************************************************" << std::endl;
    bst.bfs([](BST::Node*& node){std::cout<<*node<<std::endl;});
    out << "binary search tree size: " << bst.length() << std::endl;
    out << "********************************************************************************" << std::endl;
    return out;
}

BST& BST::operator=(const BST& bst) {
    this->root = copy(bst.root);
    return *this;
}

BST& BST::operator=(BST&& bst) {
    this->root = bst.root;
    bst.root = nullptr;
    return *this;
}

BST::BST(std::initializer_list<int> list) {
    this->root = nullptr;
    if (list.size() ==0)
    {

    } else 
    {
        for (auto& num: list)
            {
                this->add_node(num);
            }
    }
    
}

BST::~BST() {
    std::vector<Node*> nodes;
    bfs([&nodes](BST::Node*& node){nodes.push_back(node);});
    for(auto& node: nodes) {
        delete node;
    }
}

BST& BST::operator++() {
    this->bfs([](BST::Node*& node){node->value += 1;});
    return *this;
}

BST BST::operator++(int) {
    BST bst = BST(*this);
    this->bfs([](BST::Node*& node){node->value += 1;});
    return bst;
}
