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
    vector<string> all_vertices;
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

void graph::insert_edge(string start_vertex, tuple<std::string,int> temp_edge){
    int position;

    if(map_vertex.contains(start_vertex) == true && map_vertex.contains(get<0>(temp_edge)) == true){    
        Node *Node_ptr = static_cast<Node *>(map_vertex.getPointer(start_vertex));
        data[Node_ptr -> pos].edges.push_back(temp_edge);
       
        Node* p1 = new Node(); 
        *p1 = data[Node_ptr -> pos];
        map_vertex.setPointer(start_vertex, p1);
    }

    if(map_vertex.contains(start_vertex) == true && map_vertex.contains(get<0>(temp_edge)) == false){    
        Node *Node_ptr = static_cast<Node *>(map_vertex.getPointer(start_vertex));
        data[Node_ptr -> pos].edges.push_back(temp_edge);

        Node* p1 = new Node(); 
        *p1 = data[Node_ptr -> pos];
        map_vertex.setPointer(start_vertex, p1);

        position = data.size();
        
        Node newNode3;
        newNode3.vertex = get<0>(temp_edge);
        newNode3.pos = position;

        Node* p2 = new Node(); 
        *p2 = newNode3;
        
        data.push_back(newNode3);
        map_vertex.insert(get<0>(temp_edge), p2);
    }

    if(map_vertex.contains(start_vertex) == false && map_vertex.contains(get<0>(temp_edge)) == false){
        position = data.size();
        
        Node newNode;
        newNode.vertex = start_vertex;
        newNode.edges.push_back(temp_edge);
        newNode.pos = position;

        Node* p1 = new Node(); 
        *p1 = newNode;
        
        data.push_back(newNode);
        map_vertex.insert(start_vertex, p1);

        position = position + 1;       
        Node newNode2;
        newNode2.vertex = get<0>(temp_edge);
        newNode2.pos = position;

        Node* p2 = new Node(); 
        *p2 = newNode2;
        
        data.push_back(newNode2);
        map_vertex.insert(get<0>(temp_edge), p2);
        }

    if(map_vertex.contains(start_vertex) == false && map_vertex.contains(get<0>(temp_edge)) == true){
        position = data.size();
        
        Node newNode;
        newNode.vertex = start_vertex;
        newNode.edges.push_back(temp_edge);
        newNode.pos = position;

        Node* p1 = new Node(); 
        *p1 = newNode;
        
        data.push_back(newNode);
        map_vertex.insert(start_vertex, p1);
    }
}
/*
vector<string> graph::gather_no_edge_vertices(vector<string> vec, string vertice_1, string vertice_2){
    vec.push_back(vertice_1);
    vec.push_back(vertice_2);
    return vec;
}

void graph::add_no_edge_vertices(vector<string> vec){
    int position;
    for(int z =0; z<vec.size();z++){
        if(map_vertex.contains(vec[z]) != true){
            position = data.size();
            Node newNode;
        
            newNode.vertex = vec[z];
            newNode.pos = position;

            data.push_back(newNode);
            map_vertex.insert(vec[z], &data[data.size()-1]);
        }
    }
}
*/

int graph::dijkstras(string start_vertex, string output_file){
    ofstream *outFile;
    ofstream ofs(output_file); 
    outFile = &ofs;
    heap unknown_distances(data.size());
    vector<tuple<string, int, Node*>> final_distances(data.size());
    int current_vertex;
    int distance_deleted;
    Node* associated_pv;
    vector<string> path;
    int distance_final;
    string vertex_next;
    string vertex_final;
    int position_final;
    
    int temp3;
    
    for(int i = 0; i<data.size(); i++){
        unknown_distances.insert(data[i].vertex, infinity);
    }
    
    unknown_distances.setKey(start_vertex, 0);
    
    while(unknown_distances.currentSize() > 1){
        string id;
        int v_key;
        Node* v_next;
        tuple<string, int> cost_edge;
        
        current_vertex = unknown_distances.deleteMin(&id, &v_key, &v_next);
        Node *Node_ptr = static_cast<Node *>(map_vertex.getPointer(id));
        final_distances[Node_ptr -> pos] = make_tuple(id, v_key, v_next);

        for(int k = 0; k < Node_ptr -> edges.size(); k++){
            cost_edge = Node_ptr -> edges[k];
            
            int temp = unknown_distances.getKey(get<0>(cost_edge));
            bool changed = false;

            if(temp == -1){
                temp = infinity;
                changed = true;
            }
            
            int temp1 = get<1>(cost_edge);
            
            if(v_key == temp && temp == infinity){
                continue;
            }
            if(changed == true){
                temp = -1;
            }
            if(v_key +  temp1 < temp){
                v_next = &data[Node_ptr->pos];
                unknown_distances.remove(get<0>(cost_edge));
                unknown_distances.insert(get<0>(cost_edge), v_key + get<1>(cost_edge), v_next);
            }
        }
    }

    for(int z = 0; z<final_distances.size(); z++){
        path.clear();
        Node* nextNode;

        position_final = z;
        vertex_final = get<0>(final_distances[position_final]); 
        distance_final = get<1>(final_distances[position_final]);
        vertex_next = vertex_final;
        path.push_back(vertex_next);
        
        if(distance_final == infinity){
            *outFile << vertex_final <<": NO PATH" << endl; 
        }
        
        else{
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
                *outFile << path[p] << ",";
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