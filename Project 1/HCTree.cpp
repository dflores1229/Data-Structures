#include "HCTree.hpp"
#include "Helper.hpp"
#include <vector>
#include <stack>
#include <queue>
#include <iostream>

//destructor: delete nodes in post-order traversal
HCTree::~HCTree(){
    HCNode* currNode = root;
    HCNode* leftChild = root->c0;
    HCNode* rightChild = root->c1;

    //while haven't reached the bottom of the tree
    while( leftChild != nullptr || rightChild != nullptr){

        //recurse on left child if exists
        if(currNode->c0 != nullptr){
            currNode = currNode->c0;
            leftChild = root->c0;
            rightChild = root->c1;
        }
        //recurse on right child if exists
        if(currNode->c1 != nullptr){
            currNode = currNode->c1;
            leftChild = root->c0;
            rightChild = root->c1;
        }
        //visit current node
        delete currNode;
    }
}

//prints tree in pre order traversal
void HCTree::printTree( HCNode* root ){
    HCNode* currNode = root;
    HCNode* leftChild = root->c0;
    HCNode* rightChild = root->c1;

    //while haven't reached the bottom of the tree
    while( leftChild != nullptr || rightChild != nullptr){
        //visit current node
        std::cout<< currNode;
        std::cout<< "/n";

        //recurse on left child if exists
        if(currNode->c0 != nullptr){
            currNode = currNode->c0;
            leftChild = root->c0;
            rightChild = root->c1;
        }
        //recurse on right child if exists
        if(currNode->c1 != nullptr){
            currNode = currNode->c1;
            leftChild = root->c0;
            rightChild = root->c1;
        }
    }
}

/**
* Use the Huffman algorithm to build a Huffman coding tree.
* PRECONDITION: freqs is a vector of ints, such that freqs[i] is the frequency of occurrence of byte i in the input file.
* POSTCONDITION: root points to the root of the trie, and leaves[i] points to the leaf node containing byte i.
*/
void HCTree::build(const vector<int>& freqs){

    //size of vector freqs
    const int sz =256;

    //start with a forest of single node trees
    //priority queue: want low freq nodes at top of stack
    // high freq at bottom of stack
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

    for(int i=0; i < sz; i++){
        //only add nodes with non-zero freq
        if( freqs[i] != 0){

            HCNode* newNode = new HCNode(freqs[i], i);

            //push onto priority queue for later creating tree
            pq.push(newNode);
        }
    }


    //while there is more than 1 tree in the forest:
    while( pq.size() > 1 ){

        //store reference to and then remove 2 trees (T1 & T2) with lowest frequency
        HCNode* T1 = pq.top();
        pq.pop();
        HCNode* T2 = pq.top();
        pq.pop();

        //if not an inner leaf, add to leaves vector
        if(T1->symbol != '%'){
            leaves[T1->symbol] = T1;
        }
        if(T2->symbol != '%'){
            leaves[T2->symbol] = T2;
        }

        //create new node with freq = T1's freq + T2's freq
        //and label node as an inner leaf (%)
        HCNode* newNode = new HCNode( T1->count + T2->count, T1->symbol);
        //node with smallest frequency is left child
        newNode->c0 = T1;
        newNode->c1 = T2;
        //insert this new node into forest as T1 & T2's parent
        T1->p = newNode;
        T2->p = newNode;

        //if pq empty, this will be the root node
        if( pq.size() == 0){
            root = newNode;
        }else{
            //add new Node into priority queue
            pq.push(newNode);
        }
    }

    //for debugging
    //printTree(root);
}

bool findPath(HCNode* currNode, vector<int>* path, unsigned char symbol) {

    if currNode

    //if currNode has a left child, traverse left
    if(currNode->symbol == symbol){
        return true;
    }
    if (currNode->c0 != nullptr){
        //traversing left == '0' child
        path->push_back(0);
        findPath(currNode->c0, path, symbol);
    }
    //else, if currNode has a right child, traverse right
    if(currNode->c1 != nullptr){
        //traversing right == '1' child
        path->push_back(1);
        findPath(currNode->c1, path, symbol);
    }
    //reached bottom of tree but current leaf is not equal to symbol
    else{
        //traverse up tree
        path->pop_back();
        findPath(currNode->p, path, symbol);
    }
    //failed to find
    return false;
}

/**
* Write to the given FancyOutputStream the sequence of bits coding the given symbol.
* Note: only encodes one byte
* PRECONDITION: build() has been called, to create the coding tree, and initialize root pointer and leaves vector.
*/
void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const{

    //first: traverse tree until found symbol, storing the path which gives
    //the encoding along the way

    HCNode* currNode = root;
    //create a stack which holds the encoding path
    vector<int> path;
    
    findPath(currNode, &path, symbol);

    //iterate through encoding path
    for(auto it = path.begin(); it != path.end(); it++){
        //get a reference to the topmost element
        int currBit = *it;
        out.write_bit(currBit);
    }
}

/**
* Return symbol coded in the next sequence of bits from the stream.
* PRECONDITION: build() has been called, to create the coding tree, and initialize root pointer and leaves vector.
*/
unsigned char HCTree::decode(FancyInputStream & in) const{
    bool found = false;
    HCNode* currNode = root;

    //while haven't reached a leaf node;
    while( !found ){
        //check if reached bottom of tree
        if( currNode->c0==nullptr && currNode->c1==nullptr){
            //reached a leaf node
            found = true;
        }else{
            //read a bit and traverse down tree respectively
            int currBit = in.read_bit();
            if(currBit == 0){
                //traverse left
                currNode = currNode->c0;
            }else{
                //traverse right
                currNode = currNode->c1;
            }
        }
    }

    //return symbol stored in currNode
    return currNode->symbol;
}

