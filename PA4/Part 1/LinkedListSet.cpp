#include "Set.h"

/**
 * Implement the LinkedListSet methods correctly
 */
unsigned int LinkedListSet::size() {
    return linked.size();
}

void LinkedListSet::insert(string s) {
    //if string doesn't already exist
    if( !find(s) ){
        //add element to end
        linked.push_back(s);
    }
}

void LinkedListSet::remove(const string & s) {
    //check if element to be removed exists
    if( find(s) ){
        linked.remove(s);
    }
}

bool LinkedListSet::find(const string & s) {
    //iterate through entire list
    for(auto curr = linked.begin(); curr != linked.end(); ++curr){
        if( *curr == s){
            return true;
        }
    }
    return false; 
}
