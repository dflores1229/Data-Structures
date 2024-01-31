#include "Set.h"

/**
 * Implement the HashTableSet methods correctly
 */
unsigned int HashTableSet::size() {
    return ht.size();
}

void HashTableSet::insert(string s) {
    //if string doesn't already exist in set
    if( !find(s) ){
        ht.emplace( s );
    }
}

void HashTableSet::remove(const string & s) {
    if( find(s) ){
        ht.erase(s);
    }
}

bool HashTableSet::find(const string & s) {
    if( ht.find(s) == ht.end() ){
        return false;
    }
    return true;
}
