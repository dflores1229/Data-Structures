#include <limits>
#include "CountMinSketch.h"
#include "HashFunctions.h"

/**
 * Increment the count of s
 */
void CountMinSketch::increment(const string & s) {

    //i tracks the (i+1)th hash function to use as well as the
    //ith row to "insert" at
    for(int i=0; i<int(K); i++){

        //calculate hash value
        unsigned int hashval = hash_functions[i](s);

        //calculate expected index
        int idx = hashval % M;

        //"insert" element by incrementing count at
        //expected index of insertion of the count-min matrix
        count[i][idx] +=1;
    }
}

/**
 * Return an over-estimate of the count of string s
 */
unsigned int CountMinSketch::find(const string & s) {
    //the estimated count of string s is determined by the max of the counts
    int max;
    for(int i=0; i<int(K); i++){

        //calculate hash value
        unsigned int hashval = hash_functions[i](s);

        //calculate expected index
        int idx = hashval % M;

        //check if count at this index is greater than current max
        if ( int(count[i][idx]) > max){
            max = count[i][idx];
        }
    }

    //returns estimated count as an over-estimate count of string s
    return max;
}
