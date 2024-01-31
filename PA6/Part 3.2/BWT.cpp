#include <string>
#include <algorithm>
#include <vector>
#include "BWT.h"
using namespace std;

/**
 * Implement bwt() correctly
 */
string bwt(const string & s) {
        //array containing indexes of corresponding suffixes
    vector<unsigned int> BWT_arr;
    int sz = s.size();

    //create a vector containing integers 0,1,..., n-1
    for(int i=0; i< sz; i++ ){
        BWT_arr.push_back(i);
    }

    //compare function: if suffix starting at index j is alphabetically 
    //larger than suffix at index i
    std::sort(BWT_arr.begin(), BWT_arr.end(), [&] (int i,int j )
    { return ( s.substr(i, sz-1)+s.substr(0, i-1) < s.substr(j, sz-1)+s.substr(0, j-1) );}
    );

    string BWT_str= "";
    int end_idx;

    for(int i=0; i< sz; i++ ){
        end_idx = BWT_arr[i];

        if( end_idx == 0){
            BWT_str+= s[sz-1];
        }
        else{
            BWT_str+= s[end_idx-1];
        }
    }

    return BWT_str;
}
