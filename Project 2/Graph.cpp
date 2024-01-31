#include "Graph.h"
#include "Node.h"
#include "DisjointSet.h"
#include "Node.cpp"
#include "DisjointSet.cpp"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <tuple>

/**
* Initialize a Graph object from a given edge list CSV, where each line `u,v,w` represents an edge between nodes `u` and `v` with weight `w`.
* @param edgelist_csv_fn The filename of an edge list from which to load the Graph.
*/
Graph::Graph(const char* const & edgelist_csv_fn) {

    // open the file
    ifstream my_file(edgelist_csv_fn);
    string curr_line; //store current line

    //while haven't reached the end of the file, read one line from file
    while( getline(my_file, curr_line) ) {
        istringstream ss(curr_line);      // create istringstream of current line
        string first, second, third; // helper vars
        //first column represents thisNode
        getline(ss, first, ',');
        //second column represents neighbor node
        getline(ss, second, ',');
        //third column represents weight of edge between thisNode and neighbor node
        getline(ss, third, '\n');

        Node* newNode1 = new Node("temp");
        Node* newNode2 = new Node("temp");

        //determine hashvalue of "first"
        unsigned int hashval1 = hash_function(first);
        //if node doesn't already exist in table
        if( nodes_map.count(hashval1) == 0 ){
            delete newNode1;
            //create a new node
            newNode1 = new Node(first);
            //insert newNode
            nodes_map.insert( make_pair(hashval1, newNode1) );
        }else{
            delete newNode1;
            newNode1 = (nodes_map.find(hashval1))->second;
        }

        //determine hashvalue of "second"
        unsigned int hashval2 = hash_function(second);
        //if node doesn't already exist in table
        if( nodes_map.count(hashval2) == 0 ){
            delete newNode2;
            //create a new node
            newNode2 = new Node(second);

            nodes_map.insert( make_pair(hashval2, newNode2) );
        }else{
            delete newNode2;
            newNode2 = (nodes_map.find(hashval2))->second;
        }

        //add each node to other's neighbor list and label edge with weight "third"
        newNode1->add_neighbor( newNode2, stod(third), hashval2);
        newNode2->add_neighbor( newNode1, stod(third), hashval1);

        edges.insert( edges.begin(), make_tuple(first, second, stod(third)) );
        edges_count++;

    }//reached end of file

    //close file when done
    my_file.close();
    node_count = nodes_map.size();
}

/*
 * Graph destructor deletes all node objects
 */
Graph::~Graph(){
    for(auto it = nodes_map.begin(); it!= nodes_map.end(); it++){
        Node* currNode = it->second;
        delete currNode;
    }
}

/*
* Calculates the hashvalue of the label to determine its index in the hashtable
*/
unsigned int Graph::hash_function(string label){
    unsigned int val = 0;
    for (auto it = label.begin(); it!= label.end(); it++){
        val = 13*val + *it;
    }
    return val;
}

/**
* Return the number of nodes in this graph.
* @return The number of nodes in this graph.
*/
unsigned int Graph::num_nodes() {
    return node_count;
}

/**
* Return a `vector` of node labels of all nodes in this graph, in any order.
* @return A `vector` containing the labels of all nodes in this graph, in any order.
*/
vector<string> Graph::nodes() {
    vector<string> node_labels;
    string temp;

    for(auto it = nodes_map.begin(); it != nodes_map.end(); it++){
        // it->second gets pointer to Node
        temp = it->second->get_symbol();
        //add string label of node
        node_labels.push_back( temp );
    }

    return node_labels;
}

/*
 * Returns all of the graph's edges in a vector
 * of tuples of the form: (string node_u, string node_v, double edge weight)
 */
vector<tuple<string, string, double>> Graph::get_edges(){
    return edges;
}

/**
* Return the number of (undirected) edges in this graph.
* Example: If our graph has edges "A"<-(0.1)->"B" and "A"<-(0.2)->"C", this function should return 2.
* @return The number of (undirected) edges in this graph.
*/
unsigned int Graph::num_edges() {
    return edges_count;
}

/**
* Return the number of neighbors of a given node.
* @param node_label The label of the query node.
* @return The number of neighbors of the node labeled by `node_label`.
*/
unsigned int Graph::num_neighbors(string const & node_label) {
    //get pointer to node with "node_label"
    Node* thisNode = find_node(node_label);
    return thisNode->get_neighbor_count();
}

/**
* Return the weight of the edge between a given pair of nodes, or -1 if there does not exist an edge between the pair of nodes.
* @param u_label The label of the first node.
* @param v_label The label of the second node.
* @return The weight of the edge between the nodes labeled by `u_label` and `v_label`, or -1 if there does not exist an edge between the pair of nodes.
*/
double Graph::edge_weight(string const & u_label, string const & v_label) {
    Node* node_u = find_node(u_label);
    return node_u->get_weight(v_label);
}

/**
* Return a `vector` containing the labels of the neighbors of a given node.
* The neighbors can be in any order within the `vector`.
* Example: If our graph has edges "A"<-(0.1)->"B" and "A"<-(0.2)->"C", if we call this function on "A", we would return the following `vector`: {"B", "C"}
* @param node_label The label of the query node.
* @return A `vector` containing the labels of the neighbors of the node labeled by `node_label`.
*/
vector<string> Graph::neighbors(string const & node_label) {
    Node* currNode = find_node(node_label);
    return currNode->get_neighbor_labels();
}

Node* Graph::find_node(string node_label){

    //determine where node labeled "node_label" is supposed to be indexed to
    unsigned int hashval = hash_function(node_label);

    //returns a pointer to the <hashval, Node*> pair
    auto it = nodes_map.find(hashval);
    //return pointer to Node with "node_label"
    return it->second;
}

void Graph::reset_visited(){
    //iterate over elements in hashtable
    for(auto it = nodes_map.begin(); it != nodes_map.end(); it++){
        //change every node's visited status back to false
        it->second->update_visited(false);
    }
}

/**
 * Return the shortest unweighted path from a given start node to a given end node as a `vector` of `node_label` strings, including the start node.
 * If there does not exist a path from the start node to the end node, return an empty `vector`.
 * If there are multiple equally short unweighted paths from the start node to the end node, you can return any of them.
 * If the start and end are the same, the vector should just contain a single element: that node's label.
 * Example: If our graph has edges "A"<-(0.1)->"B", "A"<-(0.5)->"C", "B"<-(0.1)->"C", and "C"<-(0.1)->"D", if we start at "A" and end at "D", we would return the following `vector`: {"A", "C", "D"}
 * Example: If we start and end at "A", we would return the following `vector`: {"A"}
 * @param start_label The label of the start node.
 * @param end_label The label of the end node.
 * @return The shortest unweighted path from the node labeled by `start_label` to the node labeled by `end_label`, or an empty `vector` if no such path exists.
 */
vector<string> Graph::shortest_path_unweighted(string const & start_label, string const & end_label) {
    //keeps track of nodes we need to visit (queue -> FIFO)
    //queue stores pairs of the form: (currNode,dist_from_start)
    queue<pair<Node*, int>> to_visit;

    //key: hashval of currNode  element: pair(pointer to previous node, distance from start)
    unordered_map<unsigned int, pair<Node*, int>> nodes_visited;

    //keeps track of shortest path until found endNode
    vector<string> path;
    int d = 0;

    //consider case when start and end node are the same
    if( start_label == end_label ){
        path.push_back(start_label);
        return path;
    }

    //add starting node to be visited with distance from start 0
    to_visit.push( make_pair( find_node(start_label), d) );

    while( !to_visit.empty() ){
        //get pointer to next node in to_visit
        pair<Node*, int> current = to_visit.front();
        d = current.second;
        //remove "oldest" node from queue
        to_visit.pop();

        //if haven't visited next Node
        if( ! current.first->has_been_visited() ){
            //visit next Node
            current.first->update_visited(true);

            //found end node
            if(current.first->get_symbol() == end_label){
                //first add endNode to path
                path.push_back( current.first->get_symbol() );
                //trace back path using previous nodes
                while(d>0){
                    unsigned int hash = hash_function(current.first->get_symbol());
                    Node* prevNode = (nodes_visited.find(hash))->second.first;
                    path.insert( path.begin(), prevNode->get_symbol() );
                    d--;
                    current = make_pair(prevNode, d);
                }
                //reset visited status
                reset_visited();
                return path;
            }

            unordered_map<unsigned int, pair<Node*, double>> nextNode_neighbors = current.first->get_neighbors();

            //iterate over thisNode's neighbors
            for(auto it = nextNode_neighbors.begin() ; it!= nextNode_neighbors.end(); it++){

                Node* nbrNode = it->second.first;

                //for each unvisited neighbor of nextNode
                if( !nbrNode->has_been_visited()){
                    unsigned int nbrNodeHash = hash_function(nbrNode->get_symbol());

                    //add neighbor to to_visit with distance 1 more than previous node
                    to_visit.push( make_pair(nbrNode, d+1) );
                    //key: hashval of currNode  element: pair(pointer to previous node, distance from start)
                    nodes_visited.insert( make_pair(nbrNodeHash, make_pair(current.first,d+1)) );
                }
            }
        }
    }
    reset_visited();

    //if traversed entire tree and endNode was not found then we have found the case
    //when given two nodes in separate trees (no path exists between them)
    return vector<string>();
}


/**
 * Return the shortest weighted path from a given start node to a given end node as a `vector` of (`from_label`, `to_label`, `edge_weight`) tuples.
 * If there does not exist a path from the start node to the end node, return an empty `vector`.
 * If there are multiple equally short weighted paths from the start node to the end node, you can return any of them.
 * If the start and end are the same, the vector should just contain a single element: (`node_label`, `node_label`, -1)
 * Example: If our graph has edges "A"<-(0.1)->"B", "A"<-(0.5)->"C", "B"<-(0.1)->"C", and "C"<-(0.1)->"D", if we start at "A" and end at "D", we would return the following `vector`: {("A","B",0.1), ("B","C",0.1), ("C","D",0.1)}
 * Example: If we start and end at "A", we would return the following `vector`: {("A","A",-1)}
 * @param start_label The label of the start node.
 * @param end_label The label of the end node.
 * @return The shortest weighted path from the node labeled by `start_label` to the node labeled by `end_label`, or an empty `vector` if no such path exists.
 */
vector<tuple<string,string,double>> Graph::shortest_path_weighted(string const & start_label, string const & end_label) {
    //stores tuples of the form: (prevNode, currNode, total edge weight)
    priority_queue<tuple<Node*, Node*, int>, vector<tuple<Node*, Node*, double>>, EdgePtrComp> to_visit;

    //key: hash value of currNode    element: total edge weight, string of currNode, string of prevNode
    unordered_map< unsigned int, tuple<double, string, string>> edges_traversed;
    //int dist = -1;

    //stores shortest weighted path
    //form: (startNode, EndNode, weight of this edge)
    vector<tuple<string,string,double>> path;

    //consider case when start and end node are the same
    if( start_label == end_label ){
        path.push_back( make_tuple(start_label, end_label, -1) );
        return path;
    }

    //add starting node to be visited with edge weight from start 0
    //stores tuples of the form: (prevNode, currNode, total edge weight)
    to_visit.push( make_tuple( find_node(start_label), find_node(start_label), 0) );

    while( !to_visit.empty() ){
        //get pointer to next edge in to_visit
        //form: (prevNode, currNode, total edge weight)
        tuple<Node*, Node*, double> currentEdge = to_visit.top();
        //Node* prevNode = get<0>(currentEdge);
        Node* currNode = get<1>(currentEdge);
        double totalWeight = get<2>(currentEdge);
        //remove "oldest" node from queue
        to_visit.pop();

        //if haven't visited currNode
        if( ! currNode->has_been_visited() ){

            //visit next currNode
            currNode->update_visited(true);
            //dist++;

            //found end node
            if(currNode->get_symbol() == end_label){

                //get hash of currNode
                unsigned int currNodeHash = hash_function(currNode->get_symbol());
                string currNodeLabel = get<1>(edges_traversed.find(currNodeHash)->second);
                unsigned int prevNodeHash;
                string prevNodeLabel;
                double edgeWeight;

                //trace back path using previous nodes
                while(currNodeLabel != start_label){
                    prevNodeLabel = get<2>(edges_traversed.find(currNodeHash)->second);
                    prevNodeHash = hash_function(prevNodeLabel);
                    edgeWeight = edge_weight(currNodeLabel, prevNodeLabel);

                    //form: (string prevNode, string currNode, double weight of this edge)
                    path.insert( path.begin(), make_tuple(prevNodeLabel, currNodeLabel, edgeWeight));

                    //find edge associated with current Node and previously visted node in unordered_map
                    //has form: (hashvalue of prevNode, string of currNode, string of prevNode)

                    //update currNode to backtrack
                    currNodeHash = prevNodeHash;
                    currNodeLabel = prevNodeLabel;
                }

                //reset visited status
                reset_visited();
                return path;
            }

            unordered_map<unsigned int, pair<Node*, double>> nextNode_neighbors = currNode->get_neighbors();

            //iterate over thisNode's neighbors
            for(auto it = nextNode_neighbors.begin() ; it!= nextNode_neighbors.end(); it++){

                Node* nbrNode = it->second.first;
                double edgeWeight = it->second.second;

                //for each unvisited neighbor of nextNode
                if( ! nbrNode->has_been_visited()){
                    unsigned int nbrNodeHash = hash_function(nbrNode->get_symbol());

                    //stores tuples of the form: (prevNode, currNode, edge weight)
                    //add neighbor to visit and add edge weight to prevNode's edge weight
                    to_visit.push( make_tuple(currNode, nbrNode, totalWeight + edgeWeight) );

                    //unsigned int currNodeHash = hash_function(currNode->get_symbol());

                    //if not already in map, add new pair
                    if(edges_traversed.count(nbrNodeHash) == 0){
                        //key: hash value of currNode
                        //element: total edge weight, string of currNode, string of prevNode
                        edges_traversed.insert( make_pair(nbrNodeHash, make_tuple(totalWeight + edgeWeight,
                                                                                  nbrNode->get_symbol(),
                                                                                  currNode->get_symbol()) ));
                    }else{
                    //else, if this path's total edge weight is smaller than the path on the map
                        if( (totalWeight + edgeWeight) < get<0>(edges_traversed.find(nbrNodeHash)->second)){
                            //update total edge weight and prevNodeString
                            edges_traversed.find(nbrNodeHash)->second = make_tuple(totalWeight + edgeWeight,
                                                                                   nbrNode->get_symbol(),
                                                                                   currNode->get_symbol());
                        }
                    }
                }
            }
        }//finished visiting this node
    } //iterated over entire map

    reset_visited();
    return vector<tuple<string,string,double>>();
}

/**
 * Given a threshold, ignoring all edges with a weight greater than the threshold, return the connected components of the resulting graph as a `vector` of `vector` of `string` (i.e., each connected component is a `vector` of `string`, and you return a `vector` containing all of the connected components).
 * The components can be in any order, and the node labels within a component can be in any order.
 * Example: If our graph has edges "A"<-(0.1)->"B", "B"<-(0.2)->"C", "D"<-(0.3)->"E", and "E"<-(0.4)->"F", if our threshold is 0.3, we would output the following connected components: {{"A","B","C"}, {"D","E"}, {"F"}}
 * @param threshold The maximum edge weight to consider
 * @return The connected components of this graph, if we ignore edges with weight greater than `threshold`, as a `vector<vector<string>>`.
 */
vector<vector<string>> Graph::connected_components(double const & threshold) {
    queue<string> unvisitedNodes;
    vector<vector<string>> graphComponents;
    vector<string> currComponent;

    vector<string> graphNodeLabels = nodes();
    queue<string> graphQueue;

    //graph queue helps keep track of all nodes in graph that need to be iterated over
    for(auto it = graphNodeLabels.begin(); it != graphNodeLabels.end(); it++){
        graphQueue.push(*it);
    }

    unvisitedNodes.push( graphQueue.front() );
    graphQueue.pop();

    //while there are still unvisited nodes
    while( !unvisitedNodes.empty() ){

        //get the next univisited node in the queue
        Node* u_node = find_node( unvisitedNodes.front() );
        unvisitedNodes.pop();

        //add this node to current component
        currComponent.push_back(u_node->get_symbol());

        //if u_node is not yet visited
        if( !u_node->has_been_visited() ){
            //visit u_node
            u_node->update_visited(true);

            //if has no more neighbors, reached end of this connected component
            if(u_node->get_neighbor_count() == 0){
                graphComponents.push_back(currComponent);
                //reset currComponents to indicate a new connected component
                currComponent.clear();
            }else{
                unordered_map<unsigned int, pair<Node*, double>> u_node_neighbors = u_node->get_neighbors();
                //keeps track of number of neighbors we were able to visit
                int neighbors_visited = 0;
                for(auto it = u_node_neighbors.begin() ; it!= u_node_neighbors.end(); it++){
                    Node* nbrNode = it->second.first;
                //for each unvisited neighbor node of u_node
                    if( !nbrNode->has_been_visited() ){
                        neighbors_visited++;
                    //if edge between u_node and neighbor node <= threshold
                        if( it->second.second <= threshold ){
                            //add nbrNode to queue to be visited
                            unvisitedNodes.push(nbrNode->get_symbol());
                        }
                    }
                }//finished iterating over neighbors

                //if all of its neighbors havve been visited, go to next component
                if(neighbors_visited == 0){
                    graphComponents.push_back(currComponent);
                    //reset currComponents to indicate a new connected component
                    currComponent.clear();
                }
            }
        }
        //if haven't finished iterating over all nodes in graph
        while( unvisitedNodes.empty() ){

            //find next node in graphQueue that hasn't been visited and add to univisitedNodes
            while(!graphQueue.empty() && (find_node(graphQueue.front()))->has_been_visited() ){
                //remove this node from queue
                graphQueue.pop();
            }
            if(graphQueue.empty()){
                break;
            }

            //found next unvisited node, so add to unvisited nodes
            unvisitedNodes.push( graphQueue.front() );
            graphQueue.pop();

            if( currComponent.size() != 0){
                graphComponents.push_back(currComponent);
                //reset currComponents to indicate a new connected component
                currComponent.clear();
            }
        }
    }
    reset_visited();
    //Once BFS is complete, output c as a component of the graph
    return graphComponents;
}

/**
 * Return the smallest `threshold` such that, given a start node and an end node, if we only considered all edges with weights <= `threshold`, there would exist a path from the start node to the end node.
 * If there does not exist such a threshold (i.e., it's impossible to go from the start node to the end node even if we consider all edges), return -1.
 * Example: If our graph has edges "A"<-(0.2)->"B", "B"<-(0.4)->"C", and "A"<-(0.5)->"C", if we start at "A" and end at "C", we would return 0.4.
 * Example: If we start and end at "A", we would return 0
 * Note: The smallest connecting threshold isn't necessarily part of the shortest weighted path (such as in the first example above)
 * @param start_label The label of the start node.
 * @param end_label The label of the end node.
 * @return The smallest `threshold` such that, if we only considered all edges with weights <= `threshold, there would exist a path connecting the nodes labeled by `start_label` and `end_label`, or -1 if no such threshold exists.
 */
double Graph::smallest_connecting_threshold(string const & start_label, string const & end_label) {
    if(start_label == end_label){
        return 0;
    }
    //store all edges in a queue with minimum weight having higher priority
    //and for edges with same weight, smaller ascii values having higher priority
    priority_queue<tuple<string,string, double>, vector<tuple<string,string, double>>, EdgeTupleComp> orderedEdges;
    vector<tuple<string, string, double>> list_of_edges = get_edges();
    vector<string> graphNodes = nodes();

    for(auto it= list_of_edges.begin(); it != list_of_edges.end(); it++){
        orderedEdges.push(*it);
    }

    DisjointSet graphSet = DisjointSet(graphNodes);

    //while queue with edge lists is not empty
    while( !orderedEdges.empty() ){
        tuple<string, string, double> currEdge = orderedEdges.top();
        orderedEdges.pop();
        string element_u = get<0>(currEdge);
        string element_v = get<1>(currEdge);
        double edgeWeight = get<2>(currEdge);

        graphSet.unionSets(element_u, element_v);

        //if the nodes are in the same set:
        if( graphSet.find(start_label) == graphSet.find(end_label) ){
            //found threshold
            return edgeWeight;
        }
    }
    //no threshold exists
    return -1.0;
}

//want smaller weights to have higher priority
bool EdgePtrComp::operator()(tuple<Node*, Node*, double> lhs, tuple<Node*, Node*, double> rhs) const{

    //if have same weight compare symbols
    if( get<2>(lhs) == get<2>(rhs)){
        return get<1>(lhs) > get<1>(rhs);
    }

    //use weights to break tie
    return get<2>(lhs) > get<2>(rhs);
}

bool EdgeTupleComp::operator()(tuple<string, string, double> lhs, tuple<string, string, double> rhs) const{

    //if have same weight compare symbols
    if( get<2>(lhs) == get<2>(rhs)){
        return get<0>(lhs) > get<0>(rhs);
    }

    //use weights to break tie
    return get<2>(lhs) > get<2>(rhs);
}

