#include "Queue.h"

/**
 * Implement Queue constructor
 */
Queue::Queue() {
    //initialize empty Queue
    head = nullptr;
    tail = nullptr;
    numElements = 0;
}

/**
 * Implement the Queue destructor
 */
Queue::~Queue() {
    //deallocate anything dynamic
    clear();
    delete head;
    delete tail;
}

/**
 * Implement size() correctly
 */
unsigned int Queue::size() {
    return numElements;
}

/**
 * Implement clear() correctly without memory leaks
 */
void Queue::clear() {
    //remove all elements from queue
    while( size() != 0 ){
        pop();
    }
}

/**
 * Implement push() correctly
 */
void Queue::push(string s) {
    //create new node which stores string
    Node* newNode = new Node(s);
    
    //if node is first in queue, then it is the head node
    if (size() == 0){
        head = newNode;
    }
    else{
        //if more than 2 nodes in queue
        if( size() > 1 ){
            //old tail's next points to new node 
            tail->next = newNode;
        }

        //add element to end of queue: tail pointer points
        //to new node
        tail = newNode;   
    }

    //increase number of elements in queue
    numElements++;
}

/**
 * Implement pop() correctly without memory leaks
 */
string Queue::pop() {
    //remove element from front of queue: head pointer points to
    //second in queue

    //store string stored in curr head to be returned
    string stringPopped = head->data;

    //point to node which will be new head
    Node* temp = head->next;
    //free memory of "old" head
    delete head;

    //new head is node pointed to by temp node
    head = temp;

    //decrease number of elements in queue
    numElements--;

    return stringPopped;
}
