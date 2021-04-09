#include "grid.h"
// Prototype of a utility function to swap two integers 
void swap(Cell *x, Cell *y); 
  
// A class for Min Heap 

  
// Constructor: Builds a heap from a given array a[] of given size 
MinHeap::MinHeap(int cap) 
{ 
    heap_size = 0; 
    capacity = cap; 
    harr = new Cell[cap]; 
} 

MinHeap::~MinHeap(){
    delete(harr);
}
  
// Inserts a new key 'k' 
void MinHeap::insertKey(Cell k) 
{ 
    if (heap_size == capacity) 
    { 
        cout << "\nOverflow: Could not insertKey\n"; 
        return; 
    } 
  
    // First insert the new key at the end 
    heap_size++; 
    int i = heap_size - 1; 
    harr[i] = k; 
  
    // Fix the min heap property if it is violated 
    while (i != 0 && harr[parent(i)] > harr[i]) 
    { 
       swap(&harr[i], &harr[parent(i)]); 
       i = parent(i); 
    } 
} 
  
// Decreases value of key at index 'i' to new_val.  It is assumed that 
// new_val is smaller than harr[i]. 
void MinHeap::decreaseKey(Cell u, int new_val) 
{ 
    int i = GetIndex(u);
    harr[i].mDistance = new_val; 
    while (i != 0 && harr[parent(i)] > harr[i]) 
    { 
       swap(&harr[i], &harr[parent(i)]); 
       i = parent(i); 
    } 
} 
  
// Method to remove minimum element (or root) from min heap 
Cell MinHeap::extractMin() 
{ 
    if (heap_size == 1) 
    { 
        heap_size--; 
        return harr[0]; 
    } 
  
    // Store the minimum value, and remove it from heap 
    Cell root = harr[0]; 
    harr[0] = harr[heap_size-1]; 
    heap_size--; 
    MinHeapify(0); 
  
    return root; 
} 
  
  
// This function deletes key at index i. It first reduced value to minus 
// infinite, then calls extractMin() 
void MinHeap::deleteKey(Cell u) 
{ 
    decreaseKey(u, INT_MIN); 
    extractMin(); 
} 
  
// A recursive method to heapify a subtree with the root at given index 
// This method assumes that the subtrees are already heapified 
void MinHeap::MinHeapify(int i) 
{ 
    int l = left(i); 
    int r = right(i); 
    int smallest = i; 
    if (l < heap_size && harr[l].mDistance < harr[i].mDistance) 
        smallest = l; 
    if (r < heap_size && harr[r].mDistance < harr[smallest].mDistance) 
        smallest = r; 
    if (smallest != i) 
    { 
        swap(&harr[i], &harr[smallest]); 
        MinHeapify(smallest); 
    } 
} 

int MinHeap::GetIndex(Cell u){
    for(int i=0; i<capacity; i++){
        if(harr[i].GetX() == u.GetX() && harr[i].GetY() == u.GetY()){
            return i;
        }
    }
    return -1;
}

bool MinHeap::Empty(){
    return (heap_size == 0);
}
  
// A utility function to swap two elements 
void swap(Cell *x, Cell *y) 
{ 
    Cell temp = *x; 
    *x = *y; 
    *y = temp; 
} 
  