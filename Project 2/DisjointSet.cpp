#include "DisjointSet.h"
using namespace std;

/*
 * Convert all nodes in graph into separate and disjoint sets
 */
DisjointSet::DisjointSet(vector<string> &elements){
    for(auto it = elements.begin(); it!= elements.end(); it++){
        //add each node to the map with the form
        //key: some node    element: this node's sentinel/parent node
        set_of_elements.insert(make_pair(*it, *it));
        sentinelNodes.insert( make_pair(*it, 0));
    }
}

/*
 * Return the sentinel node of element_u
 */
string DisjointSet::find(string element_u){
    return set_of_elements.find(element_u)->second;
}

/*
 * Join two sets by making element_v's sentinel node point to element_u's sentinel node
 */
void DisjointSet::unionSets(string element_u, string element_v){
    string v_sentinel = set_of_elements.find(element_v)->second;
    string u_sentinel = set_of_elements.find(element_u)->second;
    string child, parent;

    //sentinel node with least amount of descendents becomes the child of the other
    if( sentinelNodes.find(v_sentinel)->second < sentinelNodes.find(u_sentinel)->second ){
        child = v_sentinel;
        parent = u_sentinel;
    }else{
        child = u_sentinel;
        parent = v_sentinel;
    }

    //child will no longer be a sentinel node
    sentinelNodes.erase(child);

    (set_of_elements.find(child))->second = parent;
    //parent's set has gained 1 descendent
    int numDescen = sentinelNodes.find(parent)->second;
    sentinelNodes.find(parent)->second = numDescen + 1;
}

