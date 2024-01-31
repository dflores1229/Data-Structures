#include "Set.h"

/**
 * Implement the MultiwayTrieSet constructor
 */
MultiwayTrieSet::MultiwayTrieSet() {
    Node newNode;
    root = &newNode;
    numElements = 0;
}

/**
 * Implement the MultiwayTrieSet destructor
 */
MultiwayTrieSet::~MultiwayTrieSet() {
    root = nullptr;
    numElements = 0;
}

/**
 * Implement the MultiwayTrieSet methods correctly
 */
unsigned int MultiwayTrieSet::size() {
    return numElements;
}

void MultiwayTrieSet::insert(string s) {
    //if inserting first word
    if( size() == 0){
        //create a new Node and set as root
        Node currNode = Node();
        root = &currNode;

        //iterate over each character of string
        for(int i=0; i < int(s.length()); i++){
            Node newNode = Node();

            //add child node with current character as key
            currNode.children.insert( {s[i], &newNode} );
            //traverse to node along this path
            currNode = *(currNode.children[ s[i] ]);
        }
        currNode.isWord = true;
        numElements++;
    }
    //if string doesn't already exist in set
    else if( !find(s) ){
        root->isWord = false;
        //otherwise, start at root
        Node* currNode = root;

        //iterate over each character of string
        for(int i=0; i < int(s.length()); i++){
            char currChar = s[i];

            //if current node does not have an edge labeled same as
            //currCharacter in string
            if( currNode->children.find(currChar) == currNode->children.end() ){
                //create a new child of currNode with edge labeled by currChar
                Node newNode = Node();
                currNode->children.insert( {s[i], &newNode} );
            }
            //traverse to node along this path
            currNode = currNode->children[currChar];
        } //reached end of string

        //if currNode is not already a word node, make it a word node
        if( !currNode->isWord ){
            currNode->isWord = true;
            //increment number of elements/words
            numElements++;
        }
    }
}

void MultiwayTrieSet::remove(const string & s) {
    //start at root
    Node* currNode = root;

    //iterate over each character of string
    for(int i=0; i < int(s.length()); i++){
        char currChar = s[i];
        //if current node does have an edge labeled s
        //traverse to node along this path
        if( currNode->children.find(currChar) != currNode->children.end()){
            currNode = currNode->children[currChar];
        }
    }//reached end of string

    //if word path is actually a word, make last node not a word node
    if( currNode->isWord ){
        currNode->isWord = false;
        //decrease number of elements/words
        numElements--;
    }
}

bool MultiwayTrieSet::find(const string & s) {
        //base case: when MWT is empty
    if( size() == 0){
        return false;
    }

    //start at root
    Node* currNode = root;

    //iterate over each character of string
    for(int i=0; i < int(s.length()); i++){
        char currChar = s[i];

        //if current node does not have an edge labeled same as
        //currCharacter in string
        if( currNode->children.find(currChar) != currNode->children.end() ){
            return false;
        }else{ //otherwise, edge labeled s does exist
            //traverse to node along this path
            currNode = currNode->children[currChar];
        }
    }//reached end of string

    //check if word path is actually a word
    if( currNode->isWord ){
        return true;
    }else{
        return false;
    }
}
