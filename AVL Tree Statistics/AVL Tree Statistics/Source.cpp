

/*
 # Author: Onaopemipo Olagoke
 # Creation Date: 2/28/2019
 # Modification Date: 10/15/2019
 # Purpose: Code for Simple int-to-int of Order Map ADT Implementation based on an
 extension of AVL Trees
 # NOTE: only basic methods of the Order Map ADT (i.e., put, erase, find, size,
 empty) implemented
 # Modification Date: 10/8/2022
 # Purpose: Modified code for simple int-to-int (from int-to-string) order map ADT
 Implementation on linked-structured BSTs
 */
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <cstddef>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;
// Utility functions
void loadFile(string fname, fstream& file)
{
    file.open(fname.c_str());
    if (file.fail())
    {
        cout << "Cannot open file " << fname << endl;
    }
}

/*
 # Purpose: Class definition of a simple implementation of an ordered map ADT,
 # mapping integers keys to integer values, using a binary search tree (BST) with a
 linked-structure representation
 # NOTE: only basic methods of the ordered map ADT (i.e., put, erase, find, size,
 empty), and some auxiliary functions (e.g., successor and predecessor) implemented
 # NOTE: For the sake of consistency with the implementation on other programming
 languages (e.g., Java and C++), this simple implementation does not use overloading
 of the [] or del operator in Python. Also, the documentation/comments/description
 of methods uses the term "NULL" instead of "None" (as actually used in Python
 implementations)
 */
class BSTMap
{
public:
    // simple data structure used to create nodes for (linked-list) BST-based implementation of an ordered map ADT
    // from integer keys to integer values
    class Node {
    public:
        int key;
        int value;
        Node* left;
        Node* right;
        Node* parent;
        // node constructors
        Node() : left(NULL), right(NULL), parent(NULL) { };
        Node(int k, int v) :
            key(k), value(v), left(NULL), right(NULL), parent(NULL) { };
        Node(int k, int v, Node* l, Node* r, Node* p) : key(k), value(v), left(l), right(r), parent(p) { };
        // node desctructor
        virtual ~Node() {};
        // overloading output stream for a representation of BST node w
        friend ostream& operator<<(ostream& os, const Node& w) {
            os << w.key << ":" << w.value;
            return os;
        };
        // (overloadable) print node stats
        virtual void printStats() { cout << *this; };
    };
    // prints a representation of BST node w
    // (overloadable)
    // INPUT: node w
    virtual void printNode(const Node* w) const { if (w) cout << *((Node*)w); };
    // tree constructor
    BSTMap() : root(NULL), n(0) { };
    // tree destructor
    virtual ~BSTMap();
    // basic map operations
    Node* find(int k) const;
    void put(int k, int v);
    void erase(int k);
    int size() const;
    bool empty() const;
    // auxiliary utilities
    Node* youngestAncestorType(Node* w, bool check_left) const;
    Node* youngestDescendantType(Node* w, bool check_left) const;
    Node* successor(Node* w) const;
    Node* predecessor(Node* w) const;
    // print utilities
    void print() const;   // print as parenthetic string
    void printTree(Node* s, int space) const;
    void printMap() const;
    void printTreeMap(Node* s, int space) const;
protected:
    // data member: tree root node
    Node* root;
    // (overloadable) auxiliary node creation utility
    virtual Node* createNode(int k, int v, Node* l, Node* r, Node* p) {
        return new
            Node(k, v, l, r, p);
    };

    // auxiliary print utilities
    void printAux(const Node* w, bool simple) const;  // print utility
    void printTreeAux(Node* s, int space, bool simple) const;
    // auxiliary utilities
    void makeChild(Node* p, Node* c, bool isLeft);
    Node* findNode(int k) const;
    virtual Node* putNode(int k, int v);
    virtual Node* eraseNode(int k);
private:
    // auxiliary utilities
    virtual void deleteNode(Node* w);
    virtual void deleteAll();
    Node* removeNode(Node* w);
    // data member: tree size (number of nodes/map entries)
    int n;

};
/*
 *Purpose: Implement member functions/methods of BSTMap class
 */
 /*
  # utility/aux function to print out a parenthetic string representation of the
  BST
  # INPUT: a node w in the BST (or subclass) whose subtree is to be
  # printed out; a boolean flag, simple, determining whether a
  # simple key:value pair of the BST node or (if overloaded) a
  # possibly more sophisticated representation based on
  # characteristics of a subclass object
  */
void
BSTMap::printAux(const BSTMap::Node* w, bool simple) const {
    if (w) {
        if (simple)
            cout << "[" << *w << "]";
        else {
            cout << "[";
            printNode(w);
            cout << "]";
        }
        cout << "(";
        printAux(w->left, simple);
        cout << "),(";
        printAux(w->right, simple);
        cout << ")";
    }
}
// print out a parenthetic string representation of the whole BST
void
BSTMap::print() const {
    printAux(root, false);
    cout << endl;
}
// print out a parenthetic string representation of the whole BST
// using simply the key-value info of the map entries in each node
void
BSTMap::printMap() const {
    printAux(root, true);
    cout << endl;
}
/*
 # utility/aux method to print out a tree-like layout of the whole
 # BST using a reverse inorder traversal INPUT: a node s in the BST
 # whose rooted subtree needs to be printed; a natural number,
 # space, for the minimum space separation required between the
 # root and left side of the terminal and between each node and its
 # children; a boolean flag, simple, determining whether a simple
 # key:value pair of the BST node or (if overloaded) a possibly
 # more sophisticated representation based on characteristics of a
 # subclass object
 */
void
BSTMap::printTreeAux(BSTMap::Node* s, int space, bool simple) const {
    int addSpace = 8;
    // base case
    if (!s)
    {
        return;
    }
    // add more whitespace
    space = space + addSpace;
    // print right
    printTreeAux(s->right, space, simple);
    cout << endl;
    for (int i = addSpace; i < space; i++)
        cout << " ";
    if (simple) cout << *s;
    else printNode(s);
    cout << endl;
    // print left
    printTreeAux(s->left, space, simple);
}
// print tree-like layout of the whole BST
void
BSTMap::printTree(BSTMap::Node* s, int space) const {
    printTreeAux(s, space, false);
}
/*
 # print tree-like layout of the subtree rooted at input node s
 # using simply the key-value info of the map entries in each node
 # INPUT: s, an input node in the BST; space, a natural number
 # for the minimum space separation required between the root and
 # left side of the terminal and between each node and its
 # children
 */
void
BSTMap::printTreeMap(BSTMap::Node* s, int space) const {
    printTreeAux(s, space, true);
}
/*
 # INPUT: nodes p, c in the BST, and isLeft as a
 # predicate
 # POSTCONDITION: c becomes the left child of p if isLeft is true
 # and the right child otherwise; and p becomes the parent of c
 */
void
BSTMap::makeChild(BSTMap::Node* p, BSTMap::Node* c, bool isLeft) {
    if (p) {
        if (isLeft) p->left = c;
        else p->right = c;
    }
    if (c) c->parent = p;
}
// POSTCONDITION: The subtree rooted at node w, if any, is properly removed/deleted from the BST
void
BSTMap::deleteNode(BSTMap::Node* w)
{
    if (w) {
        BSTMap::Node* z = w->parent;
        // recursively delete children
        deleteNode(w->left);
        deleteNode(w->right);
        if (z) {
            if (z->left == w) z->left = NULL;
            else z->right = NULL;
        }
        delete w;
        n--;
    }
}
// POSTCONDITION: The BST is empty (all nodes are properly removed/deleted)
void
BSTMap::deleteAll()
{
    BSTMap::Node* w = root;
    while (w) {
        if (!(w->left || w->right)) {
            BSTMap::Node* x = w;
            w = w->parent;
            if (w) {
                if (w->left == x) w->left = NULL;
                else w->right = NULL;
            }
            delete x;
            n--;
            continue;
        }
        w = (w->left) ? w->left : w->right;
    }
}
// Destructor
// POSTCONDITION: The BST is empty
BSTMap::~BSTMap() {
    deleteAll();
}
/*
 # INPUT: a node w in the BST
 # OUTPUT: the parent of w, which may be NULL if w is the root of the BST
 # PRECONDITION: the left or right subtree, or both, of w are empty
 # POSTCONDITION: the size of the BST is reduced by 1 after w is properly removed
 from the BST
 */
BSTMap::Node*
BSTMap::removeNode(BSTMap::Node* w) {
    BSTMap::Node* z = w->parent;
    // identify child if it exists
    BSTMap::Node* x = (w->left) ? w->left : w->right;
    // BSTMap::Node* x = (successor(w)) ? successor(w) : predecessor(w);
    makeChild(z, x, !z || (z->left == w));
    if (!z) root = x;
    delete w;
    n--;
    return z;
}
/*
 # INPUT: a key k, as an integer
 # OUTPUT: the last node visited while trying to find a node with key k in the BST
 */
BSTMap::Node*
BSTMap::findNode(int k) const {
    BSTMap::Node* w = root;
    BSTMap::Node* z = NULL;
    // while current node is not null and we haven't found the key...
    while (w && (w->key != k)) {
        z = w;
        // check left or right child depending on input
        w = (w->key > k) ? w->left : w->right;
    }
    return (w) ? w : z;
}
/*
 # INPUT: a key k, as an integer
 # OUTPUT: the BST node with key k if in the map; otherwise returns NULL
 */
BSTMap::Node*
BSTMap::find(int k) const {
    BSTMap::Node* w = findNode(k);
    return (w && (w->key == k)) ? w : NULL;
}
/*
 # INPUT: a key-value pair k and v (both integers)
 # OUTPUT: if k is already in the ordered map, then output the node containing k,
 with v replacing the previous map value for k;
 # otherwise, output the new node in the tree with the corresponding key-value
 pair k and v.
 # POSTCONDITION: if key k is already in the ordered map, then the node containing
 k in the BST has v as its new map value;
 # otherwise, the size of the BST is increased by 1, and a new node with key-value
 pair k after v is properly added as a leaf to the BST;
 # if the BST was empty, then the new node becomes the root of the BST (and thus
 its only node)
 */
BSTMap::Node*
BSTMap::putNode(int k, int v) {
    BSTMap::Node* w = findNode(k);
    // if key already exists, just update value
    if (w && (w->key == k)) {
        w->value = v;
        return w;
    }
    // otherwise create new node and make it a child of the last searched node
    BSTMap::Node* x = createNode(k, v, NULL, NULL, w);
    if (w) makeChild(w, x, w->key > k);
    else root = x;
    n++;
    return x;
}
/*
 # INPUT: a key-value pair k and v (both integers)
 # POSTCONDITION: same as putNode member function
 */
void
BSTMap::put(int k, int v) {
    this->putNode(k, v);
}


/*
 # INPUT: a key k (as an integer)
 # OUTPUT: the node in the tree corresponding to the parent of the node actually
 removed from the BST during the operation removing the key k from the ordered map;
 or, if k is not a key in the ordered map, then output the node in the tree last
 visited during the search for key k, or NULL if the tree is empty
 # POSTCONDITION: no node in the BST has key k: that is, if the BST had a node
 with key k, then it is properly removed; otherwise, the BST remains intact
 */
BSTMap::Node*
BSTMap::eraseNode(int k) {
    BSTMap::Node* w = findNode(k);

    if (!w || w->key != k) return w;
    BSTMap::Node* l = w->left;
    BSTMap::Node* r = w->right;


    if (l && r) {

        BSTMap::Node* x = successor(w);
        w->key = x->key;
        w->value = x->value;
        w = x;
    }
    w = removeNode(w);

    return w;
}
/*
 # INPUT: a key k (as an integer)
 # OUTPUT: see output of eraseNode member function
 # POSTCONDITION: see postcondition of eraseNode member function
 */
void
BSTMap::erase(int k) {
    eraseNode(k);
}
/*
 # INPUT: a node w in the BST or NULL;
 # ancestor type flag: True iff searching for youngest left ancestor
 # OUTPUT: the node corresponding to the youngest right/left ancestor
 # of w: this is the node x in the tree
 # that is the first ancestor of w whose immediate descendant is a
 # left/right child, depending on input type flag (said differently, the node x is
 the first found in the path from w to
 # the root such that for a node z that is also an ancestor of w,
 # thus also in that path to the root from w, and where z could be w
 # itself, we have x.right = z or x.left = z, depending on input type flag); or
 NULL if w is NULL or the root
 # node of the tree Depending on the value of the input flag
 # (True/False), the output is the ancestor of w in the BST with the
 smallest/largest
 # key larger/smaller than that of w
 */
BSTMap::Node*
BSTMap::youngestAncestorType(BSTMap::Node* w, bool check_left) const {
    if (!w) return NULL;
    BSTMap::Node* z = w;
    BSTMap::Node* x = z->parent;
    while ((x && ((check_left ? x->left : x->right) == z))) {
        z = x;
        x = x->parent;
    }
    return x;
}
/*
 # INPUT: a node w in the BST or NULL;
 # descendant type flag: True iff searching for youngest left descendant
 # OUTPUT: a node corresponding to the youngest left/right descendant
 # of w, inclusive: this is the node x in the tree
 # that is the last left/right descendant of w, depending on input
 # type flag); or NULL if w is NULL Depending on the value of the input type
 # flag (True/False), the output is the node with the
 # smallest/largest key in the subtree of the BST rooted at w
 */
BSTMap::Node*
BSTMap::youngestDescendantType(BSTMap::Node* w, bool check_left) const {
    if (!w) return NULL;
    while (check_left ? w->left : w->right)
        w = check_left ? w->left : w->right;
    return w;
}
/*
 # INPUT: a node w in the BST or NULL
 # OUTPUT: the successor node of w,
 # which is the node in the BST with the key immediately following the key of w in
 the inorder sequence of keys in the ordered map or
 # NULL if w is NULL or w has no successor node in the BST (the key of w is the
 largest in the map)
 */
BSTMap::Node*
BSTMap::successor(BSTMap::Node* w) const {
    if (!w) return NULL;
    // if there is a right child, then sucessor must be in right subtree
    if (w->right) return youngestDescendantType(w->right, true);
    // otherwise the successor is an ancestor
    else return youngestAncestorType(w, true);
}
/*
 # INPUT: a node w in the BST; or NULL
 # OUTPUT: the predecessor node of w,
 # which is the node in the tree with the key immediately following the key of w
 in the inorder sequence of keys in the ordered map; or
 # NULL if w is NULL or w has no predecessor node in the tree
 */
BSTMap::Node*
BSTMap::predecessor(BSTMap::Node* w) const {
    if (!w) return NULL;
    // if there is a left child, then predecessor must be in left subtree
    if (w->left) return youngestDescendantType(w->left, false);
    // otherwise the predecessor is an ancestor
    else return youngestAncestorType(w, false);
}
// OUTPUT: size of the tree
int
BSTMap::size() const {
    return n;
}
// OUTPUT: true if the tree is empty; false otherwise
bool
BSTMap::empty() const {
    return (!root);
}
/*
 # Purpose: Class definition of a simple implementation of an ordered map ADT,
 # mapping integers keys to integer values, using an AVL tree, based on the BSTMap
 class
 # NOTE: basic methods (e.g., put, erase, and find), inherited from BSTMap
 */
class AVLTreeMap : public BSTMap {
public:
    AVLTreeMap() { };  // default constructor
    virtual ~AVLTreeMap() { }; // (overloadable) default destructor
    // AVL Tree Node class (extends BSTMap's embedded Node class)
    class Node : public BSTMap::Node {
    public:
        // data member: node height
        int ht;
        // node constructors
        Node() : BSTMap::Node() {  };
        Node(int k, int v, Node* l, Node* r, Node* p) : BSTMap::Node(k, v, l, r, p) {
            ht = 1 + std::max(l ? l->ht : 0, r ? r->ht : 0);
        };
        // (overloadable) node destructor
        virtual ~Node() { };
        // overloading output stream for a representation of AVL Tree node w
        friend ostream& operator<<(ostream& os, const Node& w) {
            os << ((BSTMap::Node)w) << "(" << w.ht << ")";
            return os;
        };
    };

protected:
    // (overloadable) auxiliary node creation utility
    virtual Node* createNode(int k, int v, BSTMap::Node* l, BSTMap::Node* r,
        BSTMap::Node* p) {
        return new Node(k, v, (Node*)l, (Node*)r, (Node*)p);
    };
    // prints a representation of AVL node w
    // (overloadable)
    // INPUT: node w
    virtual void printNode(const BSTMap::Node* w) const {
        if (w) cout << *((Node*)
            w);
    };
    // (overloadable) auxiliary utilities
    virtual void singleRotation(Node* y, Node* z);
    virtual Node* putNode(int k, int v);
    virtual Node* eraseNode(int k);

private:
    // auxiliary utilities
    int height(Node* w);
    Node* tallestChild(Node* w, bool breakLeft);
    void resetHeight(Node* w);
    bool balanced(Node* w);
    Node* rebalance(Node* w);
    void doubleRotation(Node* x, Node* y, Node* z);
    void rebalanceAncestors(Node* w);

};
/*
 # INPUT: a node w in the AVL Tree and a predicate breakLeft
 # OUTPUT: the tallest child of w, breaking ties based on the value of the input
 breakLeft predicate
 */
AVLTreeMap::Node*
AVLTreeMap::tallestChild(AVLTreeMap::Node* w, bool breakLeft) {
    int l_ht = height((AVLTreeMap::Node*)w->left);
    int r_ht = height((AVLTreeMap::Node*)w->right);
    return ((AVLTreeMap::Node*)(((l_ht > r_ht) || ((l_ht == r_ht) && breakLeft)) ?
        w->left : w->right));
}
/*
 # INPUT: a node z in the AVL Tree
 # OUTPUT: the new root node r of the subtree originally rooted at z before the
 restructure/rebalancing at z
 # PRECONDITION: z is the only unbalanced node in the subtree that it roots; the
 difference in height of its children is exactly 2
 # POSTCONDITION: the subtree originally rooted at z is a proper AVL subtree with
 node heights properly set
 */
AVLTreeMap::Node*
AVLTreeMap::rebalance(AVLTreeMap::Node* z) {
    AVLTreeMap::Node* r = z;
    // Your code here
    // perform rotation if z is not balanced
    // find tallest child of z
    AVLTreeMap::Node* y = tallestChild(z, true);
    // find tallest child of y which is also tallest grandchild of z
    AVLTreeMap::Node* x = tallestChild(y, z->left == y);
    if ((z->left == y) == (y->left == x)) {
        singleRotation(y, r);
        r = y;
    }
    else {
        doubleRotation(x, y, r);
        r = x;
    }
    resetHeight(r);
    return r;
}
/*
 # INPUT: nodes y and z in the AVL Tree
 # PRECONDITION: z is the only unbalanced node in the subtree that it roots; y is
 the tallest child of z, and the difference in heights with its sibling is exactly 2
 # POSTCONDITION: the subtree rooted at y after the rotation, with z as one of its
 children consistent with a proper single rotation operation, is a proper AVL
 subtree with node heights properly set
 */
void
AVLTreeMap::singleRotation(AVLTreeMap::Node* y, AVLTreeMap::Node* z) {
    // Your code here
    AVLTreeMap::Node* r = (AVLTreeMap::Node*)z->parent;
    // Declare a subtree of y that needs to be reassigned to z
    AVLTreeMap::Node* y_subT;
    // An indicator that checks it's rotating left or right
    // Rotate left if y is left child of z
    bool rotateLeft = ((z->right == y) ? z->right == y : z->right == y);
    if (rotateLeft)
        y_subT = (AVLTreeMap::Node*)y->left;
    else
        y_subT = (AVLTreeMap::Node*)y->right;
    // rotation
    makeChild(z, y_subT, !rotateLeft);
    makeChild(y, z, rotateLeft);
    // make parent of z the parent of y
    makeChild(r, y, (r == NULL) || (z == r->left));
    resetHeight(y);
    resetHeight(z);
}
/*
 # INPUT: nodes x, y, and z in the AVL Tree
 # PRECONDITION: z is the only unbalanced node in the subtree that it roots; y is
 the tallest child of z, and the difference in heights with its sibling is exactly
 2; x is the tallest child of y; the parent-child relationship between y and z is
 different from that of x and y
 # POSTCONDITION: the subtree rooted at x after the rotation, with y and z as its
 respective children consistent with a proper double rotation operation, is a proper
 AVL subtree with node heights properly set
 */
void
AVLTreeMap::doubleRotation(AVLTreeMap::Node* x, AVLTreeMap::Node* y,
    AVLTreeMap::Node* z) {
    singleRotation(x, y);
    singleRotation(x, z);
}
/*
 # INPUT: a node w
 # POSTCONDITION: a proper AVL Tree
 */
void
AVLTreeMap::rebalanceAncestors(AVLTreeMap::Node* w) {
    // check ancestors and rebalance if necessary
    AVLTreeMap::Node* x;
    int old_height;
    while (w) {
        x = (AVLTreeMap::Node*)w->parent;
        old_height = height(w);
        if (balanced(w))
            resetHeight(w);
        else {
            w = rebalance(w);
            if (!x)
                root = w;
        }
        w = (old_height == height(w)) ? NULL : x;
    }
}
/*
 # INPUT: a key-value pair k and v (integer and integer, respectively)
 # OUTPUT: if k is already in the ordered map, then output a pointer to the node
 containing k, with v replacing the previous map value for k;
 # otherwise, output a pointer to a new node with the corresponding key-value pair
 k and v.
 */
AVLTreeMap::Node*
AVLTreeMap::putNode(int k, int v) {

    AVLTreeMap::Node* z = (AVLTreeMap::Node*)BSTMap::putNode(k, v);
    if (z) rebalanceAncestors((Node*)z->parent);
    return z;
}
/*
 # INPUT: an integer key k
 # OUTPUT: NULL if the key k is not in the map; otherwise, the parent of the of
 the node actually removed from the tree during the erase key map operation
 # POSTCONDITION: if w is NULL, the AVL tree remains the same; otherwise, a
 proper AVL tree with its size reduced by 1 after erasing key k from the map so that
 no node in the AVL tree has that key
 */
AVLTreeMap::Node*
AVLTreeMap::eraseNode(int k) {
    // first erase like in a BST
    AVLTreeMap::Node* z = (AVLTreeMap::Node*)BSTMap::eraseNode(k);
    rebalanceAncestors(z);
    return z;
}
/*
 # INPUT: a node w in the AVL tree
 # OUTPUT: the height of w in the AVL tree
 */
int
AVLTreeMap::height(AVLTreeMap::Node* w) {
    return ((w) ? w->ht : 0);
}
/*
 # INPUT: a node w in the AVL tree
 # PRECONDITION: w is not NULL, and the height of its children in the AVL tree has
 been properly set
 # POSTCONDITION: the height of the node pointed to by w is properly set (to 1
 plus the height of its tallest child)
 */
void
AVLTreeMap::resetHeight(AVLTreeMap::Node* w) {
    w->ht = std::max(height((AVLTreeMap::Node*)w->left), height((AVLTreeMap::Node*)
        w->right)) + 1;
}
/*
 # INPUT: a node w in the AVL tree
 # OUTPUT: true if w is balanced; false otherwise
 # PRECONDITION: w is not NULL
 */
bool
AVLTreeMap::balanced(AVLTreeMap::Node* w) {
    return (abs(height((AVLTreeMap::Node*)w->left) - height((AVLTreeMap::Node*)w->right)) <= 1);
}
/*
 Purpose: Class definition of TreeMapStats, an extension of AVLTreeMap to account
 for basic statistics about the (ordered) map associated with each node/subtree
 */
class TreeMapStats : public AVLTreeMap {
public:
    // embedded node class extension of an AVL-Tree node
    class Node : public AVLTreeMap::Node {

    private:
        // stats class to account for basic statistics/information of subtree rooted at each node
        class Stats {

        private:
            // data members: number of nodes/map entries stored in the subtree; sum of all the map values of map entries stored in the subtree; the minimum map value of all the map entries stored in the subtree; the maximum map value of the map entries stored in the subtree
            int num;
            int sum;
            int min;
            int max;
        public:
            // stats constructors
            Stats() { };
            Stats(int v, Node* l, Node* r) : num(1), sum(v), min(v), max(v) {
            };
            // stats destructor
            ~Stats() { };
            // overloading output stream for a representation of stats s
            friend ostream& operator<<(ostream& os, const Stats& s) {
                os << "{" << s.num << "," << s.sum << "," << s.min << "," << s.max << "}";
                return os;
            };
            // Modify data members : sum, num, min, max
            int getNum() const { return num; }
            int getSum() const { return sum; }
            int getMin() const { return min; }
            int getMax() const { return max; }
            void setNum(const int n) { num = n; }
            void setSum(const int s) { sum = s; }
            void setMin(const int v_min) { min = v_min; }
            void setMax(const int v_max) { max = v_max; }
        };
        // data member: node info/stats
        Stats* info;
    public:
        // tree node constructors
        Node() : AVLTreeMap::Node() { };
        Node(int k, int v, Node* l, Node* r, Node* p) : AVLTreeMap::Node(k, v, l, r, p) {
            info = new Stats(v, l, r);
        };
        // tree node destructor
        virtual ~Node() { if (this->info) delete this->info; };
        // overloading output stream for a representation of TreeMapStats node w
        friend ostream& operator<<(ostream& os, const Node& w) {
            os << ((AVLTreeMap::Node)w) << *(w.info);
            return os;
        };
        // (overloading) print utility for a node, including map entry and additional info and stats
        void printStats() { cout << *this << endl; }
        /*
         # PRECONDITION: the info values for the left and right nodes for the children
         of the node have been properly set, consistent with the subtree that they root
         # POSTCONDITION: the info values for the node have been properly set,
         consistent with the subtree that it roots
         */
         // access to the data member info
        void setInfo(int n, int s, int v_min, int v_max) {
            info->setNum(n);
            info->setSum(s);
            info->setMin(v_min);
            info->setMax(v_max);
        }
        Stats* getInfo() const { return info; }
    };
    // print utilities
    void printTreeMapStats();
    void printTreeMapStats(Node* w);
    void printTreeMap();
    // tree constructor
    TreeMapStats() { };
    // tree desctructor
    virtual ~TreeMapStats() {  };

protected:
    // (overloadable) auxiliary node creation utility
    virtual Node* createNode(int k, int v, BSTMap::Node* l, BSTMap::Node* r,
        BSTMap::Node* p) {
        return new Node(k, v, (Node*)l, (Node*)r, (Node*)p);
    };

    // prints a representation of AVL node w
    // (overloadable)
    // INPUT: node w
    virtual void printNode(const BSTMap::Node* w) const {
        if (w) cout << *((Node*)
            w);
    };
    // (overloadable) auxiliary utilities
    virtual void singleRotation(AVLTreeMap::Node* y, AVLTreeMap::Node* z);
    virtual Node* putNode(int key, int value);
    virtual Node* eraseNode(int key);
    void updateStats(TreeMapStats::Node* w);
    void updateTree(TreeMapStats::Node* w);
};
/*
 # INPUT: a TreeMapStats node
 # PRECONDITION: the stats of the node were not updated after rotation, put or erase
 # POSTCONDITION: the stats of the node are updated
 */
void TreeMapStats::updateStats(TreeMapStats::Node* w) {
    TreeMapStats::Node* l = (TreeMapStats::Node*)w->left;
    TreeMapStats::Node* r = (TreeMapStats::Node*)w->right;
    if (l && r)
        w->setInfo(l->getInfo()->getNum() + r->getInfo()->getNum() + 1, l->getInfo()->getSum() + r->getInfo()->getSum() + w->value, min(l->getInfo()->getMin(), min(w->value, r->getInfo()->getMin())), max(l->getInfo()->getMax(), max(w->value, r->getInfo()->getMax())));
    else if (!l && !r)
        w->setInfo(1, w->value, w->value, w->value);
    else if (l && !r)
        w->setInfo(l->getInfo()->getNum() + 1, l->getInfo()->getSum() + w->value, min(w->value, l->getInfo()->getMin()), max(w->value, l->getInfo()->getMax()));
    else
        w->setInfo(r->getInfo()->getNum() + 1, r->getInfo()->getSum() + w->value, min(w->value, r->getInfo()->getMin()), max(w->value, r->getInfo()->getMax()));
}

/*
 INPUT: a node that has been reblanced
 PRECONDITION: stats of every ancestors of resturctured node is off balance
 POSTCONDITION: every ancestor of restructured node's stats from the input node have been updated
 */
void TreeMapStats::updateTree(TreeMapStats::Node* w) {
    TreeMapStats::Node* x = w;
    while (x) {
        updateStats(x);
        x = (TreeMapStats::Node*)x->parent;
    }
}

/*
 # overload of singleRotation member function of an AVLTreeMap
 # see input, precondition, and postcondition for overloaded function
 # POSTCONDITION: the info/stats of nodes y and z have been properly set; also see
 postcondition for overloaded function
 */
void
TreeMapStats::singleRotation(AVLTreeMap::Node* y, AVLTreeMap::Node* z) {
    AVLTreeMap::singleRotation(y, z);
    TreeMapStats::Node* w = (TreeMapStats::Node*)y;
    TreeMapStats::Node* x = (TreeMapStats::Node*)z;

    updateStats(x);
    updateStats(w);
}
/*
 # overload of putNode member function of an AVLTreeMap
 # see input, precondition, and postcondition for overloaded function
 # POSTCONDITION: see also updateStats member function
 */
TreeMapStats::Node*
TreeMapStats::putNode(int key, int value) {
    TreeMapStats::Node* w = (TreeMapStats::Node*)AVLTreeMap::putNode(key, value);
    updateTree(w);

    return w;
}
/*
 # overload of eraseNode member function of an AVLTreeMap
 # see input, precondition, and postcondition for overloaded function
 # POSTCONDITION: see also updateStats member function
 */
TreeMapStats::Node*
TreeMapStats::eraseNode(int key) {
    TreeMapStats::Node* w = (TreeMapStats::Node*)AVLTreeMap::eraseNode(key);
    updateTree(w);

    return w;
}
/*
 # print utility for tree-like layout of map with stats
 # print entire tree with all map stats
 */
void
TreeMapStats::printTreeMapStats() {
    printTree(root, 0);
}
/*
 # print utility for tree-like layout of map (entry only)
 # INPUT: (optional) a node w
 # print entire tree if w is not given or NULL; otherwise, print only map entry
 for w
 */
void
TreeMapStats::printTreeMap() {
    AVLTreeMap::printTreeMap(root, 0);
}
/*
 # print utility for tree-like layout of map with stats
 # INPUT: (optional) a node w
 # print entire tree if w is not given or NULL; otherwise, print only map entry
 and node stats for w
 */
void
TreeMapStats::printTreeMapStats(TreeMapStats::Node* w) {
    if (w) w->printStats();
    else printTreeMapStats();
}
//  MAIN PROGRAM: DO NOT CHANGE ANYTHING BELOW
int main() {
    string inputFilename = "input.txt";
    // string inputFilename = "input.txt";
    string line;
    bool echo = true;
    TreeMapStats L;
    // open input file
    fstream inputFile;
    loadFile(inputFilename, inputFile);
    while (getline(inputFile, line))
    {
        // trim whitespace
        // echo input
        if (echo) cout << line << endl;
        // parse input using a stringstream
        stringstream lineSS(line);
        string token;
        string command = "";
        // store tokens in a vector
        vector<string> tokens;
        while (getline(lineSS, token, ' '))
        {
            // trim whitespace
            token.erase(token.find_last_not_of(" \n\r\t") + 1);
            tokens.push_back(token);
        }
        if (tokens.size() > 0)
        {
            command = tokens[0]; // first token is the command
        }

        if (tokens.size() > 1)
        {
            if (command == "erase")
            {
                L.erase(stoi(tokens[1]));
            }
            if (command == "put")
            {
                L.put(stoi(tokens[1]), stoi(tokens[2]));
            }
            if (command == "find")
            {
                int k = stoi(tokens[1]);
                TreeMapStats::Node* w = (TreeMapStats::Node*)L.find(k);
                if (w)
                    cout << w->value << endl;
                else
                    cout << "Not found!" << endl;
            }
            if (command == "print_key_stats")
            {
                int k = stoi(tokens[1]);
                TreeMapStats::Node* w = (TreeMapStats::Node*)L.find(k);
                if (w)
                    L.printTreeMapStats(w);
                else
                    cout << "Not found!" << endl;;
            }
        }
        if (command == "size")
        {
            cout << L.size() << endl;
        }
        if (command == "print")
        {
            L.printMap();
        }
        if (command == "print_stats")
        {
            L.print();
        }
        if (command == "print_tree")
        {
            L.printTreeMap();
        }
        if (command == "print_stats_tree")
        {
            L.printTreeMapStats();
        }
        if (command == "noecho")
        {
            echo = false;
        }
    }
    return EXIT_SUCCESS;
}


