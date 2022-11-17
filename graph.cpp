#include <iostream>
#include <iterator>
#include <list>
#include <tuple>
#include <vector>
#include <string>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <limits>

#include "graph.h"
#include "heap.h"
#include "hash.h"

using namespace std;

/*
graph(): Constructor that takes in an input file and parses through it to create the graph. The graph contains an vector of Nodes that each contain the vertex name, 
its edges, and the position inside the array.
*/
graph::graph(string input_file){
    ifstream ifs(input_file);
    vector<string> all_vertices;
    string line;
    string temp_string;
    string temp_start_vertex;
    string temp_end_vertex;
    string edge_string;
    int edge;
    tuple<string, int> temp_edge;
    int count;
    current_size = 0;
    if(ifs.is_open()){

        //While Loop that performs parcing as long as getline returns values
        while(getline(ifs, line)){
            count = 0;
            temp_string.clear();
            temp_start_vertex.clear();
            temp_end_vertex.clear();
            
            edge = 0;
            for (int i = 0; i < line.length(); i++)
                {
                    if(isspace(line[i]) && count == 0){
                        temp_start_vertex = temp_string;
                        temp_string.clear();
                        count = count + 1;
                    }
                    
                    else if(isspace(line[i]) && count == 1){
                        temp_end_vertex = temp_string;
                        temp_string.clear();
                        count = count + 1;
                    }

                    else if(isspace(line[i]) && count == 2){
                        edge_string = temp_string;
                        temp_string.clear();
                        count = count + 1;
                    }
                    else if(!isspace(line[i])){
                        temp_string = temp_string + (line[i]);
                    }
                }
            edge_string = temp_string;   
            edge = stoi(edge_string);
            temp_edge = make_tuple(temp_end_vertex, edge);
            insert_edge(temp_start_vertex, temp_edge);
            //all_vertices = gather_no_edge_vertices(all_vertices, temp_start_vertex, temp_end_vertex);
        }
    }
    ifs.close();
    //add_no_edge_vertices(all_vertices);
}

/*
insert_edge(): Function that takes in a starting vector and a tuple of the currentr edge and adds that edge to the starting vector.
The function determines which of the 4 cases of the starting vector and destination vector being seen or not, adds the vectors to the data array if needed, and adds the edge to the edges
of the starting vector. 
*/
void graph::insert_edge(string start_vertex, tuple<std::string,int> temp_edge){
    int position;
    bool starting = map_vertex.contains(start_vertex);
    bool destination = map_vertex.contains(get<0>(temp_edge));


    //If Starting Vertex and Desitnation Vertex were both seen already
    if(starting == true && destination == true){    
        Node *Node_ptr = static_cast<Node *>(map_vertex.getPointer(start_vertex));
        data[Node_ptr -> pos].edges.push_back(temp_edge);
       
        Node* p1 = new Node(); 
        *p1 = data[Node_ptr -> pos];
        map_vertex.setPointer(start_vertex, p1);
    }
    //If Starting Vertex was seen but Destination Vertex was not
    if(starting == true && destination == false){    
        Node *Node_ptr = static_cast<Node *>(map_vertex.getPointer(start_vertex));
        data[Node_ptr -> pos].edges.push_back(temp_edge);

        Node* p1 = new Node(); 
        *p1 = data[Node_ptr -> pos];
        map_vertex.setPointer(start_vertex, p1);

        position = current_size;
        
        Node newNode3;
        newNode3.vertex = get<0>(temp_edge);
        newNode3.pos = position;

        Node* p2 = new Node(); 
        *p2 = newNode3;
        
        data.push_back(newNode3);
        current_size = current_size + 1;
        map_vertex.insert(get<0>(temp_edge), p2);
    }
    //If Starting Vertex and Desitnation Vertex are both not seen 
    if(starting == false && destination == false){
        position = current_size;
        
        Node newNode;
        newNode.vertex = start_vertex;
        newNode.edges.push_back(temp_edge);
        newNode.pos = position;

        Node* p1 = new Node(); 
        *p1 = newNode;
        
        data.push_back(newNode);
        current_size = current_size + 1;
        map_vertex.insert(start_vertex, p1);

        position = current_size;       
        Node newNode2;
        newNode2.vertex = get<0>(temp_edge);
        newNode2.pos = position;

        Node* p2 = new Node(); 
        *p2 = newNode2;
        
        data.push_back(newNode2);
        current_size = current_size + 1;
        map_vertex.insert(get<0>(temp_edge), p2);
        }
    //If Destination Vertex was seen but Starting Vertex was not
    if(starting == false && destination == true){
        position = current_size;
        
        Node newNode;
        newNode.vertex = start_vertex;
        newNode.edges.push_back(temp_edge);
        newNode.pos = position;

        Node* p1 = new Node(); 
        *p1 = newNode;
        
        data.push_back(newNode);
        current_size = current_size + 1;
        map_vertex.insert(start_vertex, p1);
    }
}


int graph::valid_starting_vertex(string starting_vertex){
    if(map_vertex.contains(starting_vertex) == true){
        return true;
    }
    else{
        return false;
    }
}

/*
dijkstras(): Runs the djikstras akgorithim starting with the given starting vertex and returns the output to a declared output text file.
Takes a starting vector and returns the shortest distance to every other vector in the graph.
If a vector is not reachable, it declares NO PATH. 
*/
int graph::dijkstras(string start_vertex, string output_file){
    //Declarations of Local variables used in dijkstras
    ofstream *outFile;
    ofstream ofs(output_file); 
    outFile = &ofs;
    heap unknown_distances(current_size);
    vector<tuple<string, int, Node*>> final_distances(current_size);
    int current_vertex;
    int distance_deleted;
    Node* associated_pv;
    vector<string> path;
    int distance_final;
    string vertex_next;
    string vertex_final;
    int position_final;
    
   
    //Adding all the vertexes to the Priority Q
    for(int i = 0; i<current_size; i++){
        unknown_distances.insert(data[i].vertex, infinity);
    }
    
    //Initializing The Starting Vertex with a distance of 0
    unknown_distances.setKey(start_vertex, 0);
    
    //WHile Loop that performs operations until there are no more entities on the Priority Q
    while(unknown_distances.currentSize() > 1){
        //Declaration of Local Variables for the While Loop
        string id;
        int v_key;
        Node* v_next;
        tuple<string, int> cost_edge;
        
        //Deletemin the vertex with the shortest distance and add it to the final distances vector
        current_vertex = unknown_distances.deleteMin(&id, &v_key, &v_next);
        Node *Node_ptr = static_cast<Node *>(map_vertex.getPointer(id));
        final_distances[Node_ptr -> pos] = make_tuple(id, v_key, v_next);

        //For all the edges of the vector that has been taken out the Priority Q 
        for(int k = 0; k < Node_ptr -> edges.size(); k++){
            cost_edge = Node_ptr -> edges[k];
            
            int destination_d = unknown_distances.getKey(get<0>(cost_edge));
            bool changed = false;
            
            /*
            Weird errors were happening when trying to compare (value) + infinity < inifnity so had to go through this tedious looking process below.
            */

            //If the destination vector is not in the Priority Q -> Updated return value of -1 to infinity 
            if(destination_d == -1){
                destination_d = infinity;
                changed = true;
            }
            //Updated temp1 to 
            int cost_edge_d = get<1>(cost_edge);
            
            //If both the starting vector and destination vector are equal to infinity. Continue.
            if(v_key == destination_d && destination_d == infinity){
                continue;
            }
            //If the starting vector was not equal to infinity. Return the starting vector back to -1
            if(changed == true){
                destination_d = -1;
            }

            //If starting vertex distabnce plus cost_edge is less than current destination vertex. Then update destination distance. 
            if(v_key +  cost_edge_d < destination_d){
                v_next = &data[Node_ptr->pos];
                unknown_distances.remove(get<0>(cost_edge));
                unknown_distances.insert(get<0>(cost_edge), v_key + get<1>(cost_edge), v_next);
            }
        }
    }

    //After all final distances have been updated and added to the array. Parse effectivley.
    for(int z = 0; z<current_size; z++){
        path.clear();
        Node* nextNode;

        position_final = z;
        vertex_final = get<0>(final_distances[position_final]); 
        distance_final = get<1>(final_distances[position_final]);
        vertex_next = vertex_final;
        path.push_back(vertex_next);

        //If final distance is equal to inifinity. Then there was no route to get to that vertex 
        if(distance_final == infinity){
            *outFile << vertex_final <<": NO PATH" << endl; 
        }

        //If final distance was no inifinity. 
        else{
            //While the vertex is not at the starting vertex. Update path until we get there.
            while(vertex_next != start_vertex){
                nextNode = get<2>(final_distances[position_final]);
                position_final = nextNode -> pos;
                vertex_next = get<0>(final_distances[position_final]); 
                path.push_back(vertex_next);
            }
            
            *outFile << vertex_final <<": " << distance_final;
            *outFile << " [";
            for(int p = path.size()-1; p>-1 ;p--){
                if(p != 0){
                *outFile << path[p] << ", ";
                }
                else{
                    *outFile << path[p];
                }
            }
            *outFile << "]" << endl;
            }
    }
    
    return 0;
}