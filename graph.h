#ifndef _GRAPH_H
#define _GRAPH_H

#include <vector>
#include <string>
#include <tuple> 
#include "hash.h"


using namespace std;

class graph{
    public:

        graph(string input_file);

        void insert_edge(string start_vertex, tuple<std::string,int> temp_edge);

        vector<string> gather_no_edge_vertices(vector<string> vec, string vertice_1, string vertice_2);
        
        void add_no_edge_vertices(vector<string> vec);

        const int infinity = std::numeric_limits<int>::max();

        int dijkstras(string start_vertex, string output_file);

        class Node {
            public:
                string vertex;
                vector<tuple<string,int>> edges;
                int pos;
                };

        vector<Node> data;
        hashTable map_vertex;
        hashTable map_vertex_dj;
        
};



#endif //_GRAPH_H