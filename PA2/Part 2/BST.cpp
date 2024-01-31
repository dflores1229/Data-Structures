#include "BST.h"
#include <iostream>
string tmp;

/**
 * Implement the BST constructor
 */
BST::BST() {
    //initialize empty tree
    root = nullptr;
    numElements = 0;
}

/**
 * Implement the BST destructor
 */
BST::~BST() {
    clear();
    numElements = 0;
    root = nullptr;
}

/**
 * Implement size() correctly
 */
unsigned int BST::size() const {
    return numElements;
}

//returns whether a node has 0,1, or 2 children
int numChildren(BST::Node* currNode){
    //keeps track of number children
    int count = 0;

    //check if node has a left child
    if(currNode->leftChild != nullptr){
        count++;
    } 
    //check if node has a right child
    if(currNode->rightChild != nullptr){
        count++;
    }
    return count;
}

void removeNodes( BST::Node* currNode ){
    if (currNode->leftChild != nullptr){
        removeNodes(currNode->leftChild);
    }
    if (currNode->rightChild != nullptr){
        removeNodes(currNode->rightChild);
    }
    if(currNode != nullptr){
        currNode = nullptr;
        cout << "NODE DELETED: " << tmp << endl;
    }
}
/**
 * Implement clear() correctly without memory leaks
 */
void BST::clear() {

    //if only root
    if ( size() == 1){
        root = nullptr;
        numElements = 0;
        cout << "ROOT NODE DELETED: " << tmp << endl;
    }
    else if( size() > 1 ){
        //use depth-first search and post-order traversal
        Node* currNode = root;
        removeNodes(currNode);
        numElements = 0;
    }
}

/**
 * Implement insert() correctly
 */
bool BST::insert(int element) {

    //if inserting first node
    if( root == nullptr){
        //create new node
        Node newNode = Node(element);
        cout << "ROOT NODE ADDED: " << tmp << endl;

        //set newNode as root
        root = &newNode;
        numElements = 1;
        return true;
    }

    //check if node to be inserted already exists
    if( find(element) ){
       return false; 
    } else{
        //create new node
        Node newNode = Node(element);
        cout << "NODE CREATED: " << tmp << endl;
        Node* currNode = root;

        //tracks if newNode has been inserted
        bool inserted = false;

        //repeat while newNode has not been inserted into tree
        while(!inserted){

            //if data of newNode is smaller than currNode,
            //traverse down to left branch
            if( abs(newNode.data) < abs(currNode->data) ){

                //if currNode does not have left child, 
                if(currNode->leftChild == nullptr){
                    //found empty space, insert here
                    currNode->leftChild = &newNode;
                    newNode.parent = currNode;
                    
                    //increase size of tree
                    numElements++;
                    cout << "NODE INSERTED: " << tmp << endl;
                    inserted = true;
                }
                else{ //else, iterate to currNode's left child
                    currNode = currNode->leftChild;
                }
            }
            else{ //traverse down right branch

                //if currNode does not have right child, 
                if(currNode->rightChild == nullptr){

                    //insert here
                    currNode->rightChild = &newNode;
                    newNode.parent = currNode;

                    //increase size of tree
                    numElements++;
                    cout << "NODE INSERTED: " << tmp << endl;
                    inserted = true;
                    
                }
                else{ //else, iterate to currNode's right child
                    currNode = currNode->rightChild;
                }
            }
        }//end of while loop
        
        //insert success or fail
        return inserted;

    }//end if-else loop
}

/**
 * Implement find() correctly
 */
bool BST::find(const int & query) const {

    Node* currNode = root;

    //iterate while haven't reached end of tree
    while( currNode != nullptr ){
        if( currNode->data == query){
            return true;
        }else{
            //if query is smaller than currNode, traverse to left branch
            if( abs(query) < abs(currNode->data) ){
                currNode = currNode->leftChild;
            } else{ //traverse to right branch
                currNode = currNode->rightChild;
            }
        }
    }//end while loop

    //failed to find query
    return false;
}

/**
 * Implement the getLeftMostNode() function correctly
 */
BST::Node* BST::getLeftMostNode() {
    //base case: empty tree
    if( size() == 0){
        return nullptr;
    }
    //base case: only root node in tree
    else if( size() == 1 ){
        return root;
    }

    //start at root node
    Node* currNode = root;
    
    //while left currNode has a left child, iterate down left branch
    while( currNode != nullptr){
        if(currNode->leftChild != nullptr){
            currNode = currNode->leftChild;
        }else{
            return currNode;
        }
    }
    return nullptr;
}

/**
 * Implement the BST::Node successor function correctly
 */
BST::Node* BST::Node::successor() {
    Node* currNode = this;

    //only root node has no parents
    if( currNode->parent == nullptr){
        return currNode;
    }
    else if( currNode->rightChild != nullptr){
        while(currNode->rightChild != nullptr){
            currNode = currNode->rightChild;
        }
    }
    else{ //traverse up tree:
        //if current Node is its parent's left childm then
        //parent is our successor
        while(currNode != currNode->parent->leftChild){
            currNode = currNode->parent;
        }
    }

    return currNode;
}
