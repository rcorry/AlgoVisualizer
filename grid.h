#ifndef GRID_H
#define GRID_H
#include <vector>
#include <queue>
#include <limits.h>
#include <cmath>
#include <algorithm>
#include "graphics.h"
using namespace std;

class Cell {

public:
    Cell(int x, int y, bool start, bool target);
    Cell();
    void DrawCell();
    void DrawDijkstra();
    int GetY();
    int GetX();
    bool GetStart();
    bool GetTarget();
    bool GetWall();
    void SetWall(int w);
    void ClearCell();
    void Reset();
    void SetDistance(int d);
    int GetDistance();
    void CalculateDistance(int srow, int scol);
    int mDistance;
    bool inDijkstra;
    bool isVisited;
    bool inShortPath;
    int mWeight;
    bool isStart, isTarget, isWall;
    double mX, mY, mSize, fX, fY;


private:

};

class Button {
public:
    Button(double x1,double y1,double x2,double y2);
    void Draw();
    bool CheckPushed(double x, double y);
    double mW, mH, mX, mY;
};

class Slider{
public:
    Slider(double x,double y,double w,double h);
    void Draw();
    bool CheckPushed(double x, double y);
    int GetWeight(double x, double y);
    double mW, mH, mX, mY;
    double sX;
};

class Grid {

public:
    Grid();
    Grid(int w, int h);
    void DrawGrid();
    void DrawWall(int row, int col);
    void CalcDist();
    void ShowShortestPath();
    void ShowBiShortestPath(std::vector<std::vector<std::vector<int>>> StartPrev,std::vector<std::vector<std::vector<int>>> TargetPrev, int x, int y);
    void MoveStart(int x, int y);
    void MoveTarget(int x, int y);
    std::vector<Cell> Dijkstra();
    std::vector<Cell> Bidirectional();
    Cell GetStartCell();
    Cell GetTargetCell();
    std::vector<std::vector<Cell>> mGrid;
    std::vector<std::vector<std::vector<int>>> mPrev;
    std::vector<Button> mButtons;
    Slider mSlider;
    void ClearAll();
    void Reset();
    void DrawUI();
    void SetSize1();
    void SetWeight(int w);
    int sX, sY, tX, tY;
private:
    int mlastx, mlasty;
    int WWeight;
    
    

};

bool operator >(const Cell &a, const Cell &b);

class MinHeap 
{ 
    Cell *harr; // pointer to array of elements in heap 
    int capacity; // maximum possible size of min heap 
    int heap_size; // Current number of elements in min heap 
public: 
    // Constructor 
    MinHeap(int capacity); 
    ~MinHeap();
  
    // to heapify a subtree with the root at given index 
    void MinHeapify(int ); 

    bool Empty();

    int GetIndex(Cell u);
  
    int parent(int i) { return (i-1)/2; } 
  
    // to get index of left child of node at index i 
    int left(int i) { return (2*i + 1); } 
  
    // to get index of right child of node at index i 
    int right(int i) { return (2*i + 2); } 
  
    // to extract the root which is the minimum element 
    Cell extractMin(); 
  
    // Decreases key value of key at index i to new_val 
    void decreaseKey(Cell u, int new_val); 
  
    // Returns the minimum key (key at root) from min heap 
    Cell getMin() { return harr[0]; } 
  
    // Deletes a key stored at index i 
    void deleteKey(Cell u); 
  
    // Inserts a new key 'k' 
    void insertKey(Cell k); 
}; 



#endif //GRID_H