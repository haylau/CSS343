/**
 * @file BinTree.cpp
 * @author haydenlauritzen@gmail.com
 * @brief Implementation file for BinTree
 * @date 2022-04-04
 * @copyright Copyright (c) 2022
 */

#include"bintree.h"

/*** Constructors ***/

BinTree::BinTree() {
    this->root = nullptr;
}    

BinTree::BinTree(const BinTree& bst) {
    this->root = nullptr;
    this->operator=(bst);  
}   

BinTree::~BinTree() {
    this->makeEmpty();
}

/* Overloaded Operators */

BinTree& BinTree::operator=(const BinTree& bst) {
    if(*this == bst) return *this;
    // If tree is empty, makeEmpty() will immediately return.
    this->makeEmpty();
    auto h_assign = [this](const BinNode* rhs, auto&& h_assign) {
        if(rhs == nullptr) return; // Base Case
        NodeData* newData = new NodeData(*(rhs->data));
        this->insert(newData);
        h_assign(rhs->left, h_assign);
        h_assign(rhs->right, h_assign);
    };
    // Begin recursion.
    h_assign(bst.root, h_assign);
    return *this;
}

bool BinTree::operator==(const BinTree& bst) const {
    bool equal = true;
    auto h_equivalence = [&](const BinNode* lhs, const BinNode* rhs, auto&& h_equivalence) -> void {
        if(lhs == nullptr || rhs == nullptr) { // Base Case
            // If only one Node is a nullptr, the BSTs are not equivalent
            // If both nodes are nullptr, we have reached a leaf node's branches.
            if(lhs != rhs) equal = false;
        }
        else if(*(lhs->data) != *(rhs->data)) { 
            equal = false;
        }
        else {
            h_equivalence(lhs->left, rhs->left, h_equivalence);
            h_equivalence(lhs->right, rhs->right, h_equivalence);
        }
    };
    h_equivalence(this->root, bst.root, h_equivalence);
    return equal;
}

bool BinTree::operator!=(const BinTree& bst) const {
    return !(this->operator==(bst));
}

/* Accessors */

bool BinTree::retrieve(const NodeData& nd, NodeData*& out) {
    BinNode* temp = findNode(nd);
    // If search fails, nullptr is returned
    if(temp == nullptr) return false;
    out = temp->data;
    // Returns true if the found node == nd
    return (nd == *out); 
}

/* Mutators */

bool BinTree::insert(NodeData* nd) {
    auto h_insert = [&](BinNode*& cur, auto&& h_insert) mutable {
        if(cur == nullptr) {
            cur = new BinNode();
            cur->data = nd;
            return true;
        }
        // Data to insert is larger -> go right
        else if(*nd > *(cur->data)) {
            return h_insert(cur->right, h_insert);
        }
        // Data to insert is smaller -> go left
        else if(*nd < *(cur->data)) {
            return h_insert(cur->left, h_insert);
        }
        // Otherwise node is a duplicate, function will return
        return false;
    };
    return h_insert(this->root, h_insert);
}

/* Auxilary Functions */

std::ostream& operator<<(ostream& os, const BinTree& bst) {
    bool first = true;
    auto h_operatoros = [&](BinTree::BinNode* cur, auto&& h_operatoros) {
        if(cur == nullptr) return;
        h_operatoros(cur->left, h_operatoros);
        if(!first) {
            os << " ";
        }
        else {
            first = false;
        }
        os << *cur->data; 
        h_operatoros(cur->right, h_operatoros);
    };
    h_operatoros(bst.root, h_operatoros);
    os << std::endl;
    return os;
}

BinTree::BinNode* BinTree::findNode(const NodeData& nd) const {
    BinNode* result = nullptr;
    auto h_findNode = [&](BinNode* cur, auto&& h_findNode) {
        if(cur == nullptr) return; // Base Case
        if(*(cur->data) == nd) { // Node matches NodeData, end recursion
            result = cur;
            return;
        }
        // Otherwise, continue recursion.
        h_findNode(cur->left, h_findNode);
        h_findNode(cur->right, h_findNode);
    };
    h_findNode(this->root, h_findNode);
    return result;
}

int BinTree::getHeight(const NodeData& nd) const {
    // Tree is not assumed to be a BST
    // BinNode* node = findNode(nd); // Finds the node with 'nd'
    // Therefore this function above is not used
    auto h_findNode = [&](BinNode* cur, auto&& h_findNode) -> BinNode* {
        if(cur == nullptr) return cur; // Base Case
        if(nd == *(cur->data)) return cur;  // Node matches NodeData, end recursion
        BinNode* left = h_findNode(cur->left, h_findNode);
        BinNode* right = h_findNode(cur->right, h_findNode);
        // Return the pointer that is not nullptr
        // If both are nullptr, nullptr gets returned
        return left == nullptr ? right : left;
    };
    BinNode* node = h_findNode(this->root, h_findNode);
    if(node == nullptr) return 0;       
    int height = 1;
    int depth = 1;
    auto h_getHeight = [&](BinNode* cur, int depth, auto&& h_getHeight) mutable {
        if(cur == nullptr) return;
        if(depth > height) {
            height = depth;
        }
        h_getHeight(cur->left, depth+1, h_getHeight);
        h_getHeight(cur->right, depth+1, h_getHeight);
    };
    h_getHeight(node, depth, h_getHeight);
    return height;
}

void BinTree::bstreeToArray(NodeData* nd[]) {
    if(this->isEmpty()) return;
    int index = 0;
    auto h_bstreeToArray = [&](BinNode* cur, auto&& h_bstreeToArray) mutable {
        if(cur == nullptr) return;
        h_bstreeToArray(cur->left, h_bstreeToArray);
        // Array is assumed to handle ownership of values
        NodeData* newData = new NodeData(*(cur->data));
        nd[index++] = newData;
        h_bstreeToArray(cur->right, h_bstreeToArray);
    };
    h_bstreeToArray(this->root, h_bstreeToArray);
    this->makeEmpty(); // Tree should be empty; deallocates memory for tree
}

void BinTree::arrayToBSTree(NodeData* nd[]) {
    if(nd[0] == nullptr) return;
    if(this->root == nullptr) {
        this->root = new BinNode();
    }
    int numElements = 0;
    for(int i = 0; i < 100; i++) { //Array is fixed to 100 elements
        if(nd[i] == nullptr) break; // Rest of Array is nullptr 
        ++numElements;
    }
    auto h_arrayToBSTree = [](BinNode* cur, NodeData* subArray[], int size, auto&& h_arrayToBSTree) mutable -> void {
        /* 
         * [(1)]   -> base case -> set value and return
         * [1, (2)]          --> [1], [2], [nullptr] // [size/2](1)
         * [1, 2, (3), 4]    --> [1, 2], [3], [4] // [size/2](2), [1], [size - (size/2) - 1](1)
         * [1, 2, (3), 4, 5] --> [1, 2], [3], [4, 5] // [size/2](2), [1], [size - (size/2) - 1](2)
         */
        if(size > 1) {
            // Left Branch
            int leftSize = size/2;
            NodeData* leftArray[leftSize];
            for(int i = 0; i < leftSize; i++) {
                leftArray[i] = subArray[i];
            }
            cur->left = new BinNode();
            h_arrayToBSTree(cur->left, leftArray, leftSize, h_arrayToBSTree);
            // Root
            // BST is assumed to handle ownership of values
            // Root value will be midpoint
            NodeData* newData = new NodeData(*(subArray[size/2]));
            cur->data = newData; 
            // Right Branch
            if(size > 2) { 
                int rightSize = size - (size/2) - 1;
                NodeData* rightArray[rightSize];
                for(int i = 0; i < size-(rightSize); i++) {
                    rightArray[i] = subArray[(size/2)+1+i];
                }
                cur->right = new BinNode();
                h_arrayToBSTree(cur->right, rightArray, rightSize, h_arrayToBSTree);
            }
        }
        else if(size == 1) {
            NodeData* newData = new NodeData(*(subArray[0]));
            cur->data = newData; // Root value will be midpoint
        }
        else {
            throw new std::logic_error("Recursion Out of Bounds");
        }
    };
    h_arrayToBSTree(this->root, nd, numElements, h_arrayToBSTree);
    // Delete values in array.
    for(int i = 0; i < 100; i++) { 
        if(nd[i] == nullptr) break; // Rest of array is nullptr
        delete nd[i];   
        nd[i] = nullptr;
    }
}

bool BinTree::isEmpty() const {
    return this->root == nullptr || this->root->data == nullptr;
}

void BinTree::makeEmpty() {
    if(this->root == nullptr) return;
    auto h_makeEmpty = [](BinNode*& cur, auto&& h_makeEmpty) mutable {
        if(cur == nullptr) return;
        h_makeEmpty(cur->left, h_makeEmpty);
        h_makeEmpty(cur->right, h_makeEmpty);
        delete cur->data;
        cur->data = nullptr;
        delete cur;
        cur = nullptr;
    };
    h_makeEmpty(this->root, h_makeEmpty);
}

void BinTree::displaySideways() const { 
    if(this->root == nullptr) return;
    auto h_sideways = [](BinNode* cur, int level, auto&& h_sideways) -> void {
        if (cur != nullptr) {
            level++;
            h_sideways(cur->right, level, h_sideways);
            // indent for readability, 4 spaces per depth level 
            for (int i = level; i >= 0; i--) {
                cout << "    ";
            }
            cout << *cur->data << endl;        // display information of object
            h_sideways(cur->left, level, h_sideways);
	    }
    };
	h_sideways(this->root, 0, h_sideways);
}