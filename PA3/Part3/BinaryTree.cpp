#include "BinaryTree.h"

/**
 * Implement balanceFactors() correctly
 */
unordered_map<int,int> BinaryTree::balanceFactors() {
    Node* currKey = root;
    //Base Case
    if(currKey->leftChild == nullptr && currKey->rightChild == nullptr){ //leaf
        return [currKey,1];

    }else if (currKey->rightChild == nullptr){ //has only left child
        return [currKey->leftChild, 1 + currKey->leftChild->balanceFactor()[1];
        
    } else if(currKey->leftChild == nullptr){//has only right child
        return {currKey->rightChild, 1 + currKey->rightChild->balanceFactor()[1]};
    }
}
