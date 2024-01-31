#ifndef NODE_H
#define NODE_H
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
using namespace std;

class Node{
    private:
        string symbol;
        unordered_map< unsigned int, pair<Node*, double> > neighbors;
		bool visited;
		
	public:
        Node(string symbol); //Initialize node object
        string get_symbol(); //return this node's symbol
        unsigned int get_neighbor_count(); //return this node's neighbor count
        //void set_dist(int distance);
        //int get_dist();
        void add_neighbor(Node* neighbor_node, double weight, unsigned int hashval); //add an edge between this node and new node
        void update_visited(bool status); //update node's status to visited
		bool has_been_visited(); //checks if this node has been visited
        double get_weight(string const node_v); //return weight between this node and to neighbor v
        unsigned int hash_nodeVal(string label);
        vector<string> get_neighbor_labels();
        unordered_map< unsigned int, pair<Node*, double> > get_neighbors();
};

#endif

