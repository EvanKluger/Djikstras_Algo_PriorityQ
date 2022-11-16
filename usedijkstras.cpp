#include <cstdio>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cwchar>
#include <cwctype>

#include "hash.h"
#include "heap.h"
#include "graph.h"



int main(){
    //Construct Graph
    string filename_i;
    string filename_o;
    string starting_vertex;
    cout << "Enter name of graph file: ";
    cin >> filename_i;
    
    cout << "Enter name of starting vertex: ";
    cin >> starting_vertex;

    cout << "Enter name of output file: ";
    cin >> filename_o;
    
    graph Graph(filename_i);

    clock_t t1_Dict = clock();
    Graph.dijkstras(starting_vertex, filename_o);
    clock_t t2_Dict = clock();
    double timeDiff_Dict = ((double) (t2_Dict - t1_Dict)) / CLOCKS_PER_SEC;

    cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << timeDiff_Dict << "\n";

    
    
    


    return 0;
}