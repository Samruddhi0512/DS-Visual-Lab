#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Event
{
    string type;
    int a;
    int b;
};

struct Node
{
    int key;
    Node *left;
    Node *right;
    int height;

    Node(int k)
    {
        key = k;
        left = NULL;
        right = NULL;
        height = 1;
    }
};

class GenericTree {
public:
    vector<Event> events;

    void insert(int key){
        events.clear();
        events.push_back({"INSERT", key, -1});
    }
};


// ALV Tree implementation with event logging
class AVLTree
{
public:
    Node *root;
    vector<Event> events;

    AVLTree()
    {
        root = NULL;
    }

    int height(Node *n)
    {
        if (n == NULL)
            return 0;
        return n->height;
    }

    int getBalance(Node *n)
    {
        if (n == NULL)
            return 0;
        return height(n->left) - height(n->right);
    }

    Node *rightRotate(Node *y)
    {
        Node *x = y->left;
        Node *T2 = x->right;

        events.push_back({"ROTATE_RIGHT", y->key, x->key});

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    Node *leftRotate(Node *x)
    {
        Node *y = x->right;
        Node *T2 = y->left;

        events.push_back({"ROTATE_LEFT", x->key, y->key});

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    Node *insertNode(Node *node, int key)
    {
        if (node == NULL)
        {
            events.push_back({"INSERT", key, -1});
            return new Node(key);
        }

        events.push_back({"VISIT", node->key, key});

        if (key < node->key)
            node->left = insertNode(node->left, key);
        else if (key > node->key)
            node->right = insertNode(node->right, key);
        else
            return node;

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && key > node->left->key)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && key < node->right->key)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void insert(int key)
{
    clearEvents();     // important
    root = insertNode(root, key);
}


    void clearEvents()
    {
        events.clear();
    }
};

// BST implementation with event logging
class BST {
public:
    Node* root;
    vector<Event> events;

    BST(){ root = NULL; }

    Node* insertNode(Node* node, int key){
        if(!node){
            events.push_back({"INSERT", key, -1});
            return new Node(key);
        }

        events.push_back({"VISIT", node->key, key});

        if(key < node->key)
            node->left = insertNode(node->left, key);
        else if(key > node->key)
            node->right = insertNode(node->right, key);

        return node;
    }

    Node* search(Node* node, int key){
        if(!node || node->key == key) return node;

        events.push_back({"VISIT", node->key, key});

        if(key < node->key)
            return search(node->left, key);
        else
            return search(node->right, key);
    }

    Node* findMin(Node* node){
        while(node->left) node = node->left;
        return node;
    }

    Node* deleteNode(Node* node, int key){
        if(!node) return node;

        if(key < node->key)
            node->left = deleteNode(node->left, key);
        else if(key > node->key)
            node->right = deleteNode(node->right, key);
        else{
            if(!node->left) return node->right;
            if(!node->right) return node->left;

            Node* temp = findMin(node->right);
            node->key = temp->key;
            node->right = deleteNode(node->right, temp->key);
        }
        return node;
    }

    void insert(int key){
        events.clear();
        root = insertNode(root, key);
    }
};

// Red-Black Tree implementation with event logging
struct RBNode {
    int key;
    RBNode *left, *right, *parent;
    bool color; // 0 RED, 1 BLACK

    RBNode(int k){
        key = k;
        left = right = parent = NULL;
        color = 0;
    }
};

class RedBlackTree {
public:
    RBNode* root;
    vector<Event> events;

    RedBlackTree(){ root = NULL; }

    void leftRotate(RBNode* x){
        RBNode* y = x->right;
        x->right = y->left;

        if(y->left) y->left->parent = x;

        y->parent = x->parent;

        if(!x->parent) root = y;
        else if(x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    void rightRotate(RBNode* y){
        RBNode* x = y->left;
        y->left = x->right;

        if(x->right) x->right->parent = y;

        x->parent = y->parent;

        if(!y->parent) root = x;
        else if(y == y->parent->left) y->parent->left = x;
        else y->parent->right = x;

        x->right = y;
        y->parent = x;
    }

    void insert(int key){
        RBNode* z = new RBNode(key);
        events.clear();
        events.push_back({"INSERT", key, -1});

        RBNode* y = NULL;
        RBNode* x = root;

        while(x){
            y = x;
            if(z->key < x->key) x = x->left;
            else x = x->right;
        }

        z->parent = y;

        if(!y) root = z;
        else if(z->key < y->key) y->left = z;
        else y->right = z;

        fixInsert(z);
    }

    void fixInsert(RBNode* z){
        while(z->parent && z->parent->color == 0){
            if(z->parent == z->parent->parent->left){
                RBNode* y = z->parent->parent->right;

                if(y && y->color == 0){
                    z->parent->color = 1;
                    y->color = 1;
                    z->parent->parent->color = 0;
                    z = z->parent->parent;
                } else {
                    if(z == z->parent->right){
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = 1;
                    z->parent->parent->color = 0;
                    rightRotate(z->parent->parent);
                }
            } else {
                RBNode* y = z->parent->parent->left;

                if(y && y->color == 0){
                    z->parent->color = 1;
                    y->color = 1;
                    z->parent->parent->color = 0;
                    z = z->parent->parent;
                } else {
                    if(z == z->parent->left){
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = 1;
                    z->parent->parent->color = 0;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = 1;
    }
};

// Splay Tree implementation with event logging
struct SplayNode {
    int key;
    SplayNode *left, *right, *parent;

    SplayNode(int k){
        key = k;
        left = right = parent = NULL;
    }
};

class SplayTree {
public:
    SplayNode* root;
    vector<Event> events;

    void rotateRight(SplayNode* x){
        SplayNode* y = x->left;
        x->left = y->right;
        y->right = x;
        root = y;
    }

    void rotateLeft(SplayNode* x){
        SplayNode* y = x->right;
        x->right = y->left;
        y->left = x;
        root = y;
    }

    void insert(int key){
        events.clear();
        events.push_back({"INSERT", key, -1});

        root = insertNode(root, key);
    }

    SplayNode* insertNode(SplayNode* node, int key){
        if(!node) return new SplayNode(key);

        if(key < node->key)
            node->left = insertNode(node->left, key);
        else if(key > node->key)
            node->right = insertNode(node->right, key);

        return node;
    }
};

// Treap implementation with event logging
struct TreapNode {
    int key, priority;
    TreapNode *left, *right;

    TreapNode(int k){
        key = k;
        priority = rand();
        left = right = NULL;
    }
};

class Treap {
public:
    TreapNode* root;
    vector<Event> events;

    TreapNode* rotateRight(TreapNode* y){
        TreapNode* x = y->left;
        y->left = x->right;
        x->right = y;
        return x;
    }

    TreapNode* rotateLeft(TreapNode* x){
        TreapNode* y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    TreapNode* insertNode(TreapNode* node, int key){
        if(!node) return new TreapNode(key);

        if(key < node->key){
            node->left = insertNode(node->left, key);
            if(node->left->priority < node->priority)
                node = rotateRight(node);
        }
        else{
            node->right = insertNode(node->right, key);
            if(node->right->priority < node->priority)
                node = rotateLeft(node);
        }

        return node;
    }

    void insert(int key){
        events.clear();
        events.push_back({"INSERT", key, -1});
        root = insertNode(root, key);
    }
};

// B+ Tree implementation with event logging
struct BPlusNode {
    vector<int> keys;
};

class BPlusTree {
public:
    BPlusNode* root;
    vector<Event> events;

    BPlusTree() {
        root = new BPlusNode();
    }

    void insert(int key){
        events.clear();
        events.push_back({"INSERT", key, -1});

        root->keys.push_back(key);
        sort(root->keys.begin(), root->keys.end());
    }

    bool search(int key){
        events.push_back({"SEARCH", key, -1});

        for(int k : root->keys){
            if(k == key){
                events.push_back({"FOUND", key, -1});
                return true;
            }
        }
        return false;
    }
};

// Segment Tree implementations with event logging
class SegmentTree {
public:
    vector<int> tree;
    vector<Event> events;

    void build(vector<int>& arr, int node, int l, int r){
        if(l == r){
            tree[node] = arr[l];
            return;
        }

        int mid = (l + r) / 2;

        build(arr, 2*node, l, mid);
        build(arr, 2*node+1, mid+1, r);

        tree[node] = tree[2*node] + tree[2*node+1];
    }

    int query(int node, int l, int r, int ql, int qr){
        if(qr < l || ql > r) return 0;

        if(ql <= l && r <= qr){
            events.push_back({"QUERY", l, r});
            return tree[node];
        }

        int mid = (l+r)/2;

        return query(2*node,l,mid,ql,qr) +
               query(2*node+1,mid+1,r,ql,qr);
    }
};

// Interval Tree implementation with event logging
struct IntervalNode {
    int low, high, max;
    IntervalNode *left, *right;

    IntervalNode(int l,int h){
        low=l; high=h; max=h;
        left=right=NULL;
    }
};

class IntervalTree {
public:
    IntervalNode* root;
    vector<Event> events;

    IntervalTree(){ root=NULL; }

    IntervalNode* insertNode(IntervalNode* node,int l,int h){
        if(!node){
            events.push_back({"INSERT", l, h});
            return new IntervalNode(l,h);
        }

        if(l < node->low)
            node->left = insertNode(node->left,l,h);
        else
            node->right = insertNode(node->right,l,h);

        node->max = max(node->max, h);
        return node;
    }

    bool overlap(int l1,int h1,int l2,int h2){
        return (l1 <= h2 && l2 <= h1);
    }

    bool searchOverlap(IntervalNode* node,int l,int h){
        if(!node) return false;

        events.push_back({"VISIT", node->low, node->high});

        if(overlap(node->low,node->high,l,h)){
            events.push_back({"FOUND", node->low, node->high});
            return true;
        }

        if(node->left && node->left->max >= l)
            return searchOverlap(node->left,l,h);
        return searchOverlap(node->right,l,h);
    }

    void insert(int l,int h){
        events.clear();
        root = insertNode(root,l,h);
    }

    void search(int l,int h){
        events.clear();
        searchOverlap(root,l,h);
    }
};

struct Point {
    int x,y;
};

// Quad Tree implementation with event logging
class QuadTree {
public:
    vector<Point> points;
    vector<Event> events;

    void insert(int x,int y){
        events.push_back({"INSERT", x, y});
        points.push_back({x,y});
    }

    bool search(int x,int y){
        events.push_back({"SEARCH", x, y});

        for(auto p: points){
            if(p.x == x && p.y == y){
                events.push_back({"FOUND", x, y});
                return true;
            }
        }
        return false;
    }
};

struct Rect {
    int x,y,w,h;
};

// R-Tree implementation with event logging
class RTree {
public:
    vector<Rect> rects;
    vector<Event> events;

    void insert(int x,int y,int w,int h){
        events.push_back({"INSERT_RECT", x, y});
        rects.push_back({x,y,w,h});
    }

    bool overlap(Rect a, Rect b){
        return !(b.x > a.x + a.w ||
                 b.x + b.w < a.x ||
                 b.y > a.y + a.h ||
                 b.y + b.h < a.y);
    }

    void search(int x,int y,int w,int h){
        events.push_back({"SEARCH_RECT", x, y});

        Rect q = {x,y,w,h};

        for(auto r : rects){
            if(overlap(r,q)){
                events.push_back({"FOUND", r.x, r.y});
            }
        }
    }
};

// Main function to read input and perform operations
int main() {
    string type;
    cin >> type;

    int x;

    if(type == "AVL"){
        AVLTree t;
        while(cin >> x) t.insert(x);
    }
    else if(type == "BST"){
        BST t;
        while(cin >> x) t.insert(x);
    }
    else if(type == "RBT"){
        RedBlackTree t;
        while(cin >> x) t.insert(x);
    }
    else if(type == "SPLAY"){
        SplayTree t;
        while(cin >> x) t.insert(x);
    }
    else if(type == "TREAP"){
        Treap t;
        while(cin >> x) t.insert(x);
    }
    else if(type == "BPLUS"){
        BPlusTree t;
        while(cin >> x) t.insert(x);
    }

    else if(type == "INTERVAL"){
        IntervalTree t;
        int l, h;
        while(cin >> l >> h){
            t.insert(l, h);
        }
    }
    else if(type == "QUAD"){
        QuadTree t;
        int x, y;
        while(cin >> x >> y){
            t.insert(x, y);
        }
    }
    else if(type == "RTREE"){
        RTree t;
        int x, y, w, h;
        while(cin >> x >> y >> w >> h){
            t.insert(x, y, w, h);
        }
    }

    return 0;
}