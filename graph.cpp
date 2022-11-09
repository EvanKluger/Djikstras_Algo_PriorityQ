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


graph::graph(string input_file){
    ifstream ifs(input_file);
    string line;
    string temp_string;
    string temp_start_vertex;
    string temp_end_vertex;
    string edge_string;
    int edge;
    tuple<string, int> temp_edge;
    int count;
    if(ifs.is_open()){

        //While Loop that performs parcing as long as getline returns values
        while(getline(ifs, line)){
            count = 0;
            temp_string = "";
            temp_start_vertex = "";
            temp_end_vertex = "";
            
            edge = 0;
            for (int i = 0; i < line.length(); i++)
                {
                    if(isspace(line[i]) == true && count == 0){
                        temp_start_vertex = temp_string;
                        temp_string = "";
                        count = count + 1;
                    }
                    if(isspace(line[i]) == true && count == 1){
                        temp_end_vertex = temp_string;
                        temp_string = "";
                        count = count + 1;
                    }

                    if(isspace(line[i]) == true && count == 2){
                        edge_string = temp_string;
                        temp_string = "";
                        count = count + 1;
                    }
                    if(isspace(line[i]) == false){
                        temp_string = temp_string + (line[i]);
                    }
                }
            edge = stoi(temp_string);
            temp_edge = make_tuple(temp_end_vertex, edge);
            insert_edge(temp_start_vertex, temp_edge);

        }
    }
    ifs.close();
}


void graph::insert_edge(string start_vertex, tuple<std::string,int> temp_edge){
    int position;
    if(map_vertex.contains(start_vertex) == true){
        Node *Node_ptr = static_cast<Node *>(map_vertex.getPointer(start_vertex));
        data[Node_ptr -> pos].edges.push_back(temp_edge);
        }

    else{
        position = data.size();
        Node newNode;
        newNode.vertex = start_vertex;
        newNode.edges.push_back(temp_edge);
        newNode.pos = position;

        
        map_vertex.insert(start_vertex, &newNode);
        data.push_back(newNode);
        }
}

int graph::dijkstras(string start_vertex, string output_file){
    ofstream ofs(output_file); 
    outFile = &ofs;
    heap unknown_distances(data.size());
    int current_vertex;
    int distance_deleted;
    string associated_pv;
    

    for(int i = 0; i<data.size(); i++){
        unknown_distances.insert(data[i].vertex, -infinity);
    }
    
    unknown_distances.setKey(start_vertex, 0);
    unknown_distances.remove(start_vertex, &distance_deleted, &associated_pv);


    while(unknown_distances.currentSize() > 1){
        string id;
        int v_key;
        string* v_pv;
        tuple<string, int> cost_edge;
        
        current_vertex = unknown_distances.deleteMin(&id, &v_key, v_pv);
        Node *Node_ptr = static_cast<Node *>(map_vertex.getPointer(id));

        for(int k = 0; k < Node_ptr -> edges.size(); k++){
            cost_edge = Node_ptr -> edges[k];
            if(v_key + get<1>(cost_edge) < unknown_distances.getKey(get<0>(cost_edge))){
                unknown_distances.remove(get<0>(cost_edge));
                unknown_distances.insert(get<0>(cost_edge), v_key + get<1>(cost_edge), &id);
            }
        }
    }
}