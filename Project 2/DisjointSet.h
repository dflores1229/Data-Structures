#ifndef DISJOINTSET_H
#define DISJOINTSET_H
#include <unordered_map>
#include <string>
#include <vector>
using namespace std;

class DisjointSet{
private:
    //key: some node    element: this node's sentinel/parent node
    unordered_map<string, string> set_of_elements;
    //key: sentinel node element: number of descendents
    unordered_map<string, int> sentinelNodes;
public:
    DisjointSet(vector<string> &elements);
    string find(string node_u);
    void unionSets(string node_u, string node_v);
};

#endif // DISJOINTSET_H

