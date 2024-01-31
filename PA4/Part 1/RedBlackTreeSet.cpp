#include "Set.h"

/**
 * Implement the RedBlackTreeSet methods correctly
 */
unsigned int RedBlackTreeSet::size() {
    return rbt.size();
}

void RedBlackTreeSet::insert(string s) {
    if( !find(s) ){
        rbt.emplace(s);
    }
}

void RedBlackTreeSet::remove(const string & s) {
    if( find(s) ){
        rbt.erase(s);
    }
}

bool RedBlackTreeSet::find(const string & s) {
    if( rbt.find(s) == rbt.end()){
        return false;
    }
    return true;
}
