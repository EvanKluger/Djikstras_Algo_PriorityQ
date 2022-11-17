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

        int valid_starting_vertex(string starting_vertex);

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
        int current_size;

        
};



#endif //_GRAPH_H