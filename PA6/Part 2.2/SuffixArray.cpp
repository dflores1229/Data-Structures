#include <string>
#include <vector>
#include <algorithm>
#include "SuffixArray.h"
using namespace std;

/**
 * Construct the Suffix Array of a given string
 *
 * @param s The input string
 * @return A vector<unsigned int> representing the Suffix Array of s
 */
vector<unsigned int> suffix_array(const string & s) {

    //array containing indexes of corresponding suffixes
    vector<unsigned int> suff_arr;
    int sz = s.size();

    //create a vector containing integers 0,1,..., n-1
    for(int i=0; i< sz; i++ ){
        suff_arr.push_back(i);
    }

    //compare function: if suffix starting at index j is alphabetically 
    //larger than suffix at index i
    std::sort(suff_arr.begin(), suff_arr.end(), [&] (int i,int j )
    { return ( s.substr(i, sz-1) < s.substr(j, sz-1) );}
    );

    return suff_arr;
}
