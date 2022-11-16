#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "hash.h"
#include "heap.h"


using namespace std;

// heap - The constructor allocates space for the nodes of the heap
// and the mapping (hash table) based on the specified capacity
heap::heap(int capacity_heap_1){
    capacity_heap = capacity_heap_1 + 1;
    hashTable mapping(capacity_heap*2);
    data.resize(capacity_heap);
    current_size = 1;
}

// insert - Inserts a new node into the binary heap

// Inserts a node with the specified id string, key,
// and optionally a pointer. They key is used to
// determine the final position of the new node.
//
// Returns:
// 0 on success
// 1 if the heap is already filled to capacity
// 2 if a node with the given id already exists (but the heap
// is not filled to capacity)
//

int heap::insert(const std::string &id, int key, void *pv){
    if(current_size == capacity_heap){
        return 1;
    }
    else{
        if(mapping.contains(id) == true){
            
            return 2;
        }
        data[current_size].key = key;
        data[current_size].id = id;
        data[current_size].pv = pv;
        mapping.insert(id, &data[current_size]);
        percolateUp(current_size);
        current_size++;
        return 0;
        }
}

//
// setKey - set the key of the specified node to the specified value
//

// I have decided that the class should provide this member function
// instead of two separate increaseKey and decreaseKey functions.
//
// Returns:
// 0 on success
// 1 if a node with the given id does not exist
//
//FIX THIS
int heap::setKey(const std::string &id, int key){
    
    if(mapping.contains(id) == true){
    
    node *node_ptr = static_cast<node *>(mapping.getPointer(id));
    node_ptr->key = key;

    percolateUp(getPos(node_ptr));
    percolateDown(getPos(node_ptr));
    return 0;
    }
            
    return 1;
    }
    
//
// deleteMin - return the data associated with the smallest key
// and delete that node from the binary heap
//

// If pId is supplied (i.e., it is not nullptr), write to that address
// the id of the node being deleted. If pKey is supplied, write to
// that address the key of the node being deleted. If ppData is
// supplied, write to that address the associated void pointer.
//
// Returns:
// 0 on success
// 1 if the heap is empty
//
int heap::deleteMin(std::string *pId, int *pKey, void *ppData){

    if(current_size < 2){
        return 1;
    }
    else{
        if(pKey != nullptr){
        *pKey = data[1].key;
        }
        if(pId != nullptr){
        *pId = data[1].id;
        }
        if(ppData != nullptr){
        *(static_cast<void **>(ppData)) = data[1].pv; 
        }

        mapping.remove(data[1].id);
        data[1] = data[current_size-1];
        mapping.setPointer(data[current_size-1].id, &data[1]);
        percolateDown(1);
        current_size--;
        
        //*(static_cast<void **> (ppData)) = data[1].pData;
        return 0;
    }
}
//
// remove - delete the node with the specified id from the binary heap
//

// If pKey is supplied, write to that address the key of the node
// being deleted. If ppData is supplied, write to that address the
// associated void pointer.
//
// Returns:
// 0 on success
// 1 if a node with the given id does not exist
//
int heap::remove(const std::string &id, int *pKey, void *ppData){
//FIX THIS
if(mapping.contains(id) == true){
    
    
    
    node *node_ptr = static_cast<node *>(mapping.getPointer(id));
    int idx = getPos(node_ptr);
    
    if(pKey != NULL){
        *pKey = data[idx].key;
    }
    if(ppData != NULL){
        *(static_cast<void **>(ppData)) = data[idx].pv; 
    }

    data[idx].key = data[1].key-1;
    percolateUp(idx);
    deleteMin(nullptr, pKey, ppData);
    
    return 0;
}
return 1;
}
//Member Function that performs a percolate_down. Takes a node on the Heap and moves it up to the position according to the Binary Heap rules
void heap::percolateUp(int posCur){
    node temp = data[posCur];
    int parent_idx = (posCur)/2;
    
    while(data[parent_idx].key > temp.key){
        if(parent_idx == 0){
            data[posCur] = temp;
            mapping.setPointer(temp.id, &data[posCur]);
            return;
        }
        if(posCur == 1){
            return;;
        }
        mapping.setPointer(data[parent_idx].id, &data[posCur]);
        data[posCur] = data[parent_idx];
        posCur = parent_idx;  
        parent_idx = (posCur)/2;
    }
    data[posCur] = temp;
    mapping.setPointer(temp.id, &data[posCur]);
    return;
}

//Member Function that performs a percolate_down. Takes a node on the Heap and moves it down to the position according to the Binary Heap rules
void heap::percolateDown(int posCur){
    node temp = data[posCur];
    int child_idx_2 = (posCur*2)+1;
    int child_idx_1 = (posCur*2);
    while(temp.key > data[child_idx_1].key ||  temp.key > data[child_idx_2].key){
        if(posCur * 2 >= current_size){
            data[posCur] = temp;
            mapping.setPointer(temp.id, &data[posCur]);
            return;
        }
        if(child_idx_2 == current_size){
            if(temp.key > data[child_idx_1].key){
                data[posCur] = data[child_idx_1];
                mapping.setPointer(data[child_idx_1].id, &data[posCur]);
                posCur = child_idx_1;
                child_idx_2 = (posCur*2)+1;
                child_idx_1 = (posCur*2);
                data[posCur] = temp;
                mapping.setPointer(temp.id, &data[posCur]);
                return;
            }
            data[posCur] = temp;
            mapping.setPointer(temp.id, &data[posCur]);
            return;
        }
        if(data[child_idx_1].key <= data[child_idx_2].key){
                data[posCur] = data[child_idx_1];
                mapping.setPointer(data[child_idx_1].id, &data[posCur]);
                posCur = child_idx_1;
                child_idx_2 = (posCur*2)+1;
                child_idx_1 = (posCur*2);
                continue;
        }
        if(data[child_idx_1].key > data[child_idx_2].key){
            data[posCur] = data[child_idx_2];
            mapping.setPointer(data[child_idx_2].id, &data[posCur]);
            posCur = child_idx_2;
            child_idx_2 = (posCur*2)+1;
            child_idx_1 = (posCur*2);
            continue;
        }
    }
    data[posCur] = temp;
    mapping.setPointer(temp.id, &data[posCur]);
    return;
}
//Member Function that given a pointer return the position
int heap::getPos(node *pn){
    int pos = pn - &data[0];
    return pos;
}

int heap::currentSize(){
    return current_size;
}

int heap::getKey(const std::string &id){
    if(mapping.contains(id) == true){
    
        int key;
        
        node *node_ptr = static_cast<node *>(mapping.getPointer(id));
        key = node_ptr -> key;
        return key;
    } 
    return -1;
}
