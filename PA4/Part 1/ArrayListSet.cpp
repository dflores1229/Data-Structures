#include "Set.h"

/**
 * Implement the ArrayListSet methods correctly
 */
unsigned int ArrayListSet::size() {
    return arr.size();
}

void ArrayListSet::insert(string s) {

    //if string doesn't already exist, insert string
    if( !find(s) ){
        //add element to end of vector
        arr.push_back(s);
    }
    
}

void ArrayListSet::remove(const string & s) {
    //if string to be removed exists
    if( find(s) ){
        //temporary array 
        vector<string> temp;
        
        //iterate through array vector
        for(string & currStr: arr){
            //make copy of original array not including that element
            if(currStr != s){
                temp.push_back(currStr);
            }
        } //finished making copy of array
        arr = temp;
    }
}

bool ArrayListSet::find(const string & s) {

    //iterate through array vector
    for(string & currStr: arr){
        //if element to be removed does exist in array
        if(currStr == s){
            return true;
        }
    }

    //iterated through vector and failed to find element
    return false;
}
