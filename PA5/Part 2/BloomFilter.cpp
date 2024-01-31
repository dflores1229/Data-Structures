#include "BloomFilter.h"
#include "HashFunctions.h"

/**
 * insert string into bloomfilter
 */
void BloomFilter::insert(const string & s) {

    for(int i=0; i<int(K); i++){
        //calculate hash value
        unsigned int hashval = hash_functions[i](s);

        //calculate expected index
        int idx = hashval % M;

        //"insert" element by changing boolean to true at 
        //expected index of insertion of the bloomfilter
        bits[idx] = true;
    }
}

/**
* Return false if s definitely doesn't exist in this BloomFilter,
* or true if s MIGHT exist in this BloomFilter
*/
bool BloomFilter::find(const string & s) {
    //keeps track of instances where an element might have been "inserted"
    int count = 0;

    for(int i=0; i<int(K); i++){
        //calculate hash value of string to be looked for
        unsigned int hashval = hash_functions[i](s);

        //calculate expected index
        int idx = hashval % M;

        //increase count when boolean at expected index is true
        if( bits[idx] == true){
            count++;
        }
    }

    if( count == int(K) ){
        return true;
    }
    else{
        return false;
    }
}
