#include "Node.h"
using namespace std;

//Initialize node object
Node::Node(string sym){
    symbol = sym;
    visited = false;
    //dist = INFINITY;
}

//return this node's symbol
string Node::get_symbol(){
    return symbol;
}

//return this node's neighbor count
unsigned int Node::get_neighbor_count(){
    return neighbors.size();
}
/*
void Node::set_dist(int distance){
    dist = distance;
}

int Node::get_dist(){
    return dist;
}
*/

//add an edge between this node and new node
void Node::add_neighbor(Node* neighbor_node, double weight, unsigned int hashval){
//hashval parameter is hashvalue of neighbor node to be used as a key

    neighbors.insert( make_pair(hashval, make_pair(neighbor_node, weight)) );
}

//update node's status to visited
void Node::update_visited(bool status){
    visited = status;
}

//checks if this node has been visited
bool Node::has_been_visited(){
    return visited;
}

//return weight between this node and to neighbor v
double Node::get_weight(string const node_v){
    unsigned int hashval = hash_nodeVal( node_v);

    //check if node_v is a neighbor of thisNode
    if( neighbors.find(hashval) != neighbors.end() ){
        //get weight of associated edge
        double edge_weight = (neighbors.find(hashval))->second.second;
        return edge_weight;
    }else{ //otherwise no edge exists between these nodes
       return -1.0;
    }
}

unsigned int Node::hash_nodeVal(string label){
    unsigned int val = 0;
    for (auto it = label.begin(); it!= label.end(); it++){
        val = 13*val + *it;
    }
    return val;
}

/**
* Return a `vector` containing the labels of the neighbors of a given node.
*/
vector<string> Node::get_neighbor_labels(){
    vector<string> neighbor_labels;

    //iterate over thisNodes' neighbors and add the node's labels to a vector
    for(auto it = neighbors.begin(); it!= neighbors.end(); it++){
        string currString = it->second.first->get_symbol();
        neighbor_labels.push_back(currString);
    }
    return neighbor_labels;
}

unordered_map< unsigned int, pair<Node*, double> > Node::get_neighbors(){
    return neighbors;
}

