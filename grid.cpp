#include "grid.h"
extern Algorithms current_algorithm;

Cell::Cell(int x, int y, bool start, bool target){
    mX = x;
    mY = y;
    fX = x;
    fY = y;
    mSize = 1;
    mWeight = 1;
    isStart = start;
    isTarget = target;
    isWall = false;
    mDistance = gW*gH+10;
    inDijkstra = false;
    inShortPath = false;
    isVisited = false;
}
Cell::Cell(){
    mX = 0;
    mY = 0;
    mSize = 1;
    mWeight = 1;
    isStart = false;
    isTarget = false;
    isWall = false;
    mDistance = gW*gH+10;
    inDijkstra = false;
    isVisited = false;
}

void Cell::DrawDijkstra(){
    glColor3d(1,1,0);
    DrawRectangle(mX, mY, mX+mSize, mY+mSize);
}

void Cell::DrawCell(){
    if(mSize < 1){
        mSize += .1;
        if(mSize < 1){
            fX -= .05;
            fY -= .05;
        }
    }
    if(isWall){
        glColor3ub(0,51,102);
    } else if(inDijkstra){
        glColor3d(0,1,.8);
    }else{
        //glColor3ub(220, 220, 220);
        glColor3d(1,1,1);
    }
    if(inDijkstra && isWall){
        glColor3ub(255,127,80);
    }
    if (inShortPath)
        glColor3d(1,1,0);
    if(inDijkstra && isWall && inShortPath){
        glColor3ub(255,215,0);
    }
    if(isStart)
        glColor3d(0,.9,.3);
    if(isTarget)
        glColor3d(1,0,0);
    DrawRectangle(fX, fY, fX+mSize, fY+mSize);
}

void Cell::ClearCell(){
    mDistance = gW*gH+10;
    inDijkstra = false;
    isVisited = false;
    inShortPath = false;
    mWeight = 1;
    isWall = false;
}
void Cell::Reset(){
    mDistance = gW*gH+10;
    inDijkstra = false;
    isVisited = false;
    inShortPath = false;
    if(!isWall){
        mWeight = 1;
    }
}

int Cell::GetY(){
    return mY;
}

int Cell::GetX(){
    return mX;
}

bool Cell::GetStart(){
    return isStart;
}

bool Cell::GetTarget(){
    return isTarget;
}

bool Cell::GetWall(){
    return isWall;
}

void Cell::SetWall(int w){
    if(isWall){
        isWall = false;
        mWeight = 1;
    } else {
        if(isStart || isTarget){
            return;
        }
        isWall = true;
        mWeight = w;
        mSize = .1;
        fX += .45; 
        fY += .45; 
    }
}

void Cell::SetDistance(int d){
    mDistance = d;
}

int Cell::GetDistance(){
    return mDistance;
}

void Cell::CalculateDistance(int sX, int sY){
    int y = abs(sY - mY);
    int x = abs(sX - mX);
    SetDistance(x+y);
}


Grid::Grid() :mSlider(Slider(gW*3/4-.4,gH-(gMargin/2)-1, 5, .7))
{
    WWeight = gW*gH+10;
    double w = 1.5;
    double h = 1;
    Button VisualizeButton(gW/2-w, gH-(gMargin/2)-h, gW/2+w, gH-(gMargin/2));
    w = 1;
    Button ClearButton(5+gW/2-w, gH-(gMargin/2)-h, 5+gW/2+w, gH-(gMargin/2));
    w = 1.1;
    Button ResetButton(8+gW/2-w, gH-(gMargin/2)-h, 8+gW/2+w, gH-(gMargin/2));
    w = 1.25;
    Button DijkstraButton(gW/6-w, gH-(gMargin/2)-h, gW/6+w, gH-(gMargin/2));
    Button BidirectionalButton(2+gW/6, gH-(gMargin/2)-h, -1.9+gW/6, gH-(gMargin/2));
    mButtons.push_back(VisualizeButton);
    mButtons.push_back(ClearButton);
    mButtons.push_back(ResetButton);
    mButtons.push_back(DijkstraButton);
    mButtons.push_back(BidirectionalButton);

    mlastx = -1;
    mlasty = -1;
    for(int x=0; x<gW; x++){
        std::vector<Cell> temp;
        std::vector<std::vector<int>> temp2;
        for(int y=0; y<gH-gMargin; y++){
            std::vector<int> h = {-1, -1};
            temp2.push_back(h);
            if(y == (gH-gMargin)/2){
                if(x == gW/4){
                    Cell c(x, y, true, false);
                    temp.push_back(c);
                    sY = y;
                    sX = x;
                }
                else if(x == (gW * 3)/4){
                    Cell c(x, y, false, true);
                    temp.push_back(c);
                    tX = x;
                    tY = y;
                } else {
                    Cell c(x, y, false, false);
                    temp.push_back(c);
                }
            } else{
                Cell c(x, y, false, false);
                    temp.push_back(c);
            }
        }
        mPrev.push_back(temp2);
        mGrid.push_back(temp);
    }
}


void Grid::SetSize1(){
    for(int x=0; x<gW; x++){
        for(int y=0; y<gH-gMargin; y++){
            mGrid[x][y].mSize = 1;
            mGrid[x][y].fX = x;
            mGrid[x][y].fY = y;
        }
    }
}
void Grid::SetWeight(int w){
    for(int x=0; x<gW; x++){
        for(int y=0; y<gH-gMargin; y++){
            if(mGrid[x][y].isWall){
                mGrid[x][y].mWeight = w;
            }
        }
    }
    WWeight = w;
}

void Grid::DrawGrid(){
    for(int x=0; x<gW; x++){
        for(int y=0; y<gH-gMargin; y++){
            mGrid[x][y].DrawCell();
        }
    }
    glColor3ub(0,51,102);

    for(int i=0; i<=gH-gMargin; i++){
        for(int j=0; j<=gW; j++){
            DrawLine(j, 0, j, gH-gMargin);
        }
        DrawLine(0, i, gW, i);
    }
}

Cell Grid::GetStartCell(){
    return mGrid[sX][sY];
}
Cell Grid::GetTargetCell(){
    return mGrid[tX][tY];
}

void Grid::DrawWall(int x, int y){ 
    if(mlastx!=x || mlasty!=y){
        mGrid[x][y].SetWall(WWeight);
    }
    mlastx = x;
    mlasty = y;
}


std::vector<Cell> Grid::Dijkstra(){
    mGrid[sX][sY].SetDistance(0);
    MinHeap mh(gW*(gH-gMargin));
    std::vector<Cell> vInOrder;
    bool visited[gW][gH-gMargin];
    for(int x=0; x<gW; x++){
        for(int y=0; y<gH-gMargin; y++){
            mh.insertKey(mGrid[x][y]);
            visited[x][y] = false;
        }
    }
    visited[sX][sY] = false;
    while(!mh.Empty()){
        Cell u = mh.extractMin();
        int uX = u.GetX();
        int uY = u.GetY();

        //checks if visited
        if (visited[uX][uY]){
            continue;
        }
        //checks if at target
        if (u.GetTarget()){
            ShowShortestPath();
            return vInOrder;
        }
        //checks if walled in
        if(mGrid[uX][uY].mDistance > gW*gH){
            return vInOrder;
        }

        mGrid[uX][uY].inDijkstra = true;
        mGrid[uX][uY].mSize = .1;
        mGrid[uX][uY].fX += .45;
        mGrid[uX][uY].fY += .45;
        visited[uX][uY] = true;
        DrawGrid();
        DrawUI();
        glutSwapBuffers();
        vInOrder.push_back(u);

        //Checks if there is a top Cell
        if(uY < gH-gMargin-1){
            if(mGrid[uX][uY+1].mDistance > u.mDistance){
                mGrid[uX][uY+1].mDistance = u.mDistance + mGrid[uX][uY+1].mWeight;
                mh.decreaseKey(mGrid[uX][uY+1], mGrid[uX][uY+1].mDistance);
                mPrev[uX][uY+1][0] = uX;
                mPrev[uX][uY+1][1] = uY;
            }
        }
        //Checks if there is a right Cell
        if(uX < gW-1){
            if(mGrid[uX+1][uY].mDistance > u.mDistance){
                mGrid[uX+1][uY].mDistance = u.mDistance + mGrid[uX+1][uY].mWeight;
                mh.decreaseKey(mGrid[uX+1][uY], mGrid[uX+1][uY].mDistance);
                mPrev[uX+1][uY][0] = uX;
                mPrev[uX+1][uY][1] = uY;
            }
        }
        //Checks if there is a bottom Cell
        if(uY > 0){
            if(mGrid[uX][uY-1].mDistance > u.mDistance){
                mGrid[uX][uY-1].mDistance = u.mDistance + mGrid[uX][uY-1].mWeight;
                mh.decreaseKey(mGrid[uX][uY-1], mGrid[uX][uY-1].mDistance);
                mPrev[uX][uY-1][0] = uX;
                mPrev[uX][uY-1][1] = uY;
            }
        }
        //Checks if there is a left Cell
        if(uX > 0){
            if(mGrid[uX-1][uY].mDistance > u.mDistance){
                mGrid[uX-1][uY].mDistance = u.mDistance + mGrid[uX-1][uY].mWeight;
                mh.decreaseKey(mGrid[uX-1][uY], mGrid[uX-1][uY].mDistance);
                mPrev[uX-1][uY][0] = uX;
                mPrev[uX-1][uY][1] = uY;
            }
        }

    }
    return vInOrder;
}

bool operator >(const Cell &a, const Cell &b){
    return a.mDistance > b.mDistance;
}

void Grid::ShowShortestPath(){
    int x = mPrev[tX][tY][0];
    int y = mPrev[tX][tY][1];
    bool atStart = false;
    while(!atStart){
        if(x==sX && y==sY){
            return;
        }
        mGrid[x][y].inShortPath = true;
        int tempx = mPrev[x][y][0];
        int tempy = mPrev[x][y][1];
        x = tempx;
        y = tempy;
        if(x==sX && y==sY){
            atStart = true;
        }
        DrawGrid();
        glutSwapBuffers();
    }
}

void Grid::MoveStart(int x, int y){
    if(!mGrid[x][y].isWall && !mGrid[x][y].isTarget){
        mGrid[sX][sY].isStart = false;
        mGrid[x][y].isStart = true;
        sX = x;
        sY = y;
    }
}

void Grid::MoveTarget(int x, int y){
    if(!mGrid[x][y].isWall && !mGrid[x][y].isStart){
        mGrid[tX][tY].isTarget = false;
        mGrid[x][y].isTarget = true;
        tX = x;
        tY = y;
    }
}

void Grid::Reset(){
    for(int x=0; x<gW; x++){
        for(int y=0; y<gH-gMargin; y++){
            mGrid[x][y].Reset();
        }
    }

}

void Grid::ClearAll(){
    for(int x=0; x<gW; x++){
        for(int y=0; y<gH-gMargin; y++){
            mGrid[x][y].ClearCell();
        }
    }
    //reset Start/Target Nodes
    mGrid[sX][sY].isStart = false;
    mGrid[tX][tY].isTarget = false;

    sX = gW/4;
    sY = (gH-gMargin)/2;
    tX = 3*gW/4;
    tY = (gH-gMargin)/2;

    mGrid[sX][sY].isStart = true;
    mGrid[tX][tY].isTarget = true;
    glutPostRedisplay();
}

Button::Button(double x1, double y1, double x2, double y2){
    mX = x1;
    mY = y1;
    mW = abs(x2-x1);
    mH = abs(y2-y1);
};

void Button::Draw(){
    DrawRectangle(mX, mY, mX+mW, mY+mH);
}

bool Button::CheckPushed(double x, double y){
    if(x >= mX && x <= mX+mW){
        if(y >= mY && y <= mY+mH){
            return true;
        }
    }
    return false;
}

void Grid::DrawUI(){
    //Draws UI Background
    glColor3ub(20,29,37);
    DrawRectangle(0, gH-gMargin, gW, gH);

    //Algorithm header
    glColor3d(1,1,1);
    DrawText(.5+gW/6,gH-(gMargin/2)+.4, "Algorithms");

    //WallWeight header
    glColor3d(1,1,1);
    DrawText(.5+gW*3/4,gH-(gMargin/2)+.4, "Wall Weight");
    std::string s = std::to_string(WWeight);
    char const *weight = s.c_str(); 
    if(WWeight == 2410)
        weight = "MAX";
    DrawText(5+gW*3/4,gH-(gMargin/2)-.85, weight);
    mSlider.Draw();

    //Draws Visualize Button
    glColor3ub(0,130,51);
    mButtons[0].Draw();
    glColor3d(1,1,1);
    DrawText(mButtons[0].mX+.3,mButtons[0].mY+.25, "Visualize");

    glColor3ub(204,0,0);
    mButtons[1].Draw();
    glColor3d(1,1,1);
    DrawText(mButtons[1].mX+.3,mButtons[1].mY+.25, "Clear");

    glColor3d(0,.8,.5);
    mButtons[2].Draw();
    glColor3d(1,1,1);
    DrawText(mButtons[2].mX+.3,mButtons[2].mY+.25, "Reset");

    if(current_algorithm == DIJKSTRA)
        glColor3d(0,.8,.5);
    else
        glColor3d(1,0,0);
    mButtons[3].Draw();
    glColor3d(1,1,1);
    DrawText(mButtons[3].mX+.3,mButtons[3].mY+.25, "Dijkstra");

    if(current_algorithm == BIDIRECTIONAL)
        glColor3d(0,.8,.5);
    else
        glColor3d(1,0,0);
    mButtons[4].Draw();
    glColor3d(1,1,1);
    DrawText(mButtons[4].mX+.3,mButtons[4].mY+.25, "Bidirectional");
}

std::vector<Cell> Grid::Bidirectional(){
    std::vector<std::vector<Cell>> StartGrid(mGrid);
    std::vector<std::vector<Cell>> TargetGrid(mGrid);
    MinHeap StartHeap(gW*(gH-gMargin));
    MinHeap TargetHeap(gW*gH-gMargin);
    std::vector<Cell> vInOrder;
    bool StartVisited[gW][gH-gMargin];
    bool TargetVisited[gW][gH-gMargin];
    std::vector<std::vector<std::vector<int>>> StartPrev;
    std::vector<std::vector<std::vector<int>>> TargetPrev;

    std::vector<int> h = {sX,sY};//Helps with visualizing shortest path
    std::vector<int> p = {tX,tY};
    std::vector<std::vector<int>> temp1;
    std::vector<std::vector<int>> temp2;

    StartGrid[sX][sY].SetDistance(0);
    TargetGrid[tX][tY].SetDistance(0);
    for(int x=0; x<gW; x++){
        for(int y=0; y<gH-gMargin; y++){
            StartHeap.insertKey(StartGrid[x][y]);
            TargetHeap.insertKey(TargetGrid[x][y]);
            StartVisited[x][y] = false;
            TargetVisited[x][y] = false;
            temp1.push_back(h);
            temp2.push_back(p);
        }
        StartPrev.push_back(temp1);
        TargetPrev.push_back(temp2);
    }

    while(!StartHeap.Empty() && !TargetHeap.Empty()){
        Cell a = StartHeap.extractMin();
        int aX = a.GetX();
        int aY = a.GetY();

        Cell b = TargetHeap.extractMin();
        int bX = b.GetX();
        int bY = b.GetY();

        //checks if at target
        StartVisited[aX][aY] = true;
        TargetVisited[bX][bY] = true;
        if (StartVisited[aX][aY] && TargetVisited[aX][aY]){
            ShowBiShortestPath(StartPrev, TargetPrev, aX, aY);
            return vInOrder;
        }
        if (StartVisited[bX][bY] && TargetVisited[bX][bY]){
            ShowBiShortestPath(StartPrev, TargetPrev, bX, bY);
            return vInOrder;
        }
        //checks if walled in
        if(StartGrid[aX][aY].mDistance > gW*gH || TargetGrid[bX][bY].mDistance > gW*gH ){
            return vInOrder;
        }

        mGrid[aX][aY].inDijkstra = true;
        mGrid[bX][bY].inDijkstra = true;
        mGrid[aX][aY].mSize = .1;
        mGrid[aX][aY].fX += .45;
        mGrid[aX][aY].fY += .45;
        mGrid[bX][bY].mSize = .1;
        mGrid[bX][bY].fX += .45;
        mGrid[bX][bY].fY += .45;
        DrawGrid();
        DrawUI();
        glutSwapBuffers();
        vInOrder.push_back(a);
        vInOrder.push_back(b);

        //Checks if there is a top Cell
        if(aY < gH-gMargin-1){
            if(StartGrid[aX][aY+1].mDistance > a.mDistance){
                StartGrid[aX][aY+1].mDistance = a.mDistance + StartGrid[aX][aY+1].mWeight;
                StartHeap.decreaseKey(StartGrid[aX][aY+1], StartGrid[aX][aY+1].mDistance);
                StartPrev[aX][aY+1][0] = aX;
                StartPrev[aX][aY+1][1] = aY;
            }
        }
        //Checks if there is a right Cell
        if(aX < gW-1){
            if(StartGrid[aX+1][aY].mDistance > a.mDistance){
                StartGrid[aX+1][aY].mDistance = a.mDistance + StartGrid[aX+1][aY].mWeight;
                StartHeap.decreaseKey(StartGrid[aX+1][aY], StartGrid[aX+1][aY].mDistance);
                StartPrev[aX+1][aY][0] = aX;
                StartPrev[aX+1][aY][1] = aY;
            }
        }
        //Checks if there is a bottom Cell
        if(aY > 0){
            if(StartGrid[aX][aY-1].mDistance > a.mDistance){
                StartGrid[aX][aY-1].mDistance = a.mDistance + StartGrid[aX][aY-1].mWeight;
                StartHeap.decreaseKey(StartGrid[aX][aY-1], StartGrid[aX][aY-1].mDistance);
                StartPrev[aX][aY-1][0] = aX;
                StartPrev[aX][aY-1][1] = aY;
            }
        }
        //Checks if there is a left Cell
        if(aX > 0){
            if(StartGrid[aX-1][aY].mDistance > a.mDistance){
                StartGrid[aX-1][aY].mDistance = a.mDistance + StartGrid[aX-1][aY].mWeight;
                StartHeap.decreaseKey(StartGrid[aX-1][aY], StartGrid[aX-1][aY].mDistance);
                StartPrev[aX-1][aY][0] = aX;
                StartPrev[aX-1][aY][1] = aY;
            }
        }

        //Checks if there is a top Cell
        if(bY < gH-gMargin-1){
            if(TargetGrid[bX][bY+1].mDistance > b.mDistance){
                TargetGrid[bX][bY+1].mDistance = b.mDistance + TargetGrid[bX][bY+1].mWeight;
                TargetHeap.decreaseKey(TargetGrid[bX][bY+1], TargetGrid[bX][bY+1].mDistance);
                TargetPrev[bX][bY+1][0] = bX;
                TargetPrev[bX][bY+1][1] = bY;
            }
        }
        //Checks if there is a right Cell
        if(bX < gW-1){
            if(TargetGrid[bX+1][bY].mDistance > b.mDistance){
                TargetGrid[bX+1][bY].mDistance = b.mDistance + TargetGrid[bX+1][bY].mWeight;
                TargetHeap.decreaseKey(TargetGrid[bX+1][bY], TargetGrid[bX+1][bY].mDistance);
                TargetPrev[bX+1][bY][0] = bX;
                TargetPrev[bX+1][bY][1] = bY;
            }
        }
        //Checks if there is a bottom Cell
        if(bY > 0){
            if(TargetGrid[bX][bY-1].mDistance > b.mDistance){
                TargetGrid[bX][bY-1].mDistance = b.mDistance + TargetGrid[bX][bY-1].mWeight;
                TargetHeap.decreaseKey(TargetGrid[bX][bY-1], TargetGrid[bX][bY-1].mDistance);
                TargetPrev[bX][bY-1][0] = bX;
                TargetPrev[bX][bY-1][1] = bY;
            }
        }
        //Checks if there is a left Cell
        if(bX > 0){
            if(TargetGrid[bX-1][bY].mDistance > b.mDistance){
                TargetGrid[bX-1][bY].mDistance = b.mDistance + TargetGrid[bX-1][bY].mWeight;
                TargetHeap.decreaseKey(TargetGrid[bX-1][bY], TargetGrid[bX-1][bY].mDistance);
                TargetPrev[bX-1][bY][0] = bX;
                TargetPrev[bX-1][bY][1] = bY;
            }
        }

    }
    return vInOrder;
}

void Grid::ShowBiShortestPath(std::vector<std::vector<std::vector<int>>> StartPrev,std::vector<std::vector<std::vector<int>>> TargetPrev, int x, int y){
    if((x == sX && y == sY) || (x == tX && y == tY)){
        return;     
    }
    int aX = StartPrev[x][y][0];
    int aY = StartPrev[x][y][1];
    int bX = TargetPrev[x][y][0];
    int bY = TargetPrev[x][y][1];

    mGrid[x][y].inShortPath = true;
    mGrid[aX][aY].inShortPath = true;
    mGrid[bX][bY].inShortPath = true;

    bool atStart = false;
    bool atTarget = false;
    while(!atStart || !atTarget){
        if(!atStart){
            int tAx = StartPrev[aX][aY][0];
            int tAy = StartPrev[aX][aY][1];
            aX = tAx;
            aY = tAy;
        }
        if(!atTarget){
            int tBx = TargetPrev[bX][bY][0];
            int tBy = TargetPrev[bX][bY][1];
            bX = tBx;
            bY = tBy;
        }
        mGrid[aX][aY].inShortPath = true;
        mGrid[bX][bY].inShortPath = true;

        if(aX==sX && aY==sY){
            atStart = true;
        }
        if(bX==tX && bY==tY){
            atTarget = true;
        }
        DrawGrid();
        glutSwapBuffers();
    }
}

Slider::Slider(double x, double y, double w, double h){
    mX = x;
    mY = y;
    mW = w;
    mH = h;
    sX = mX + mW;
}

void Slider::Draw(){
    glColor3ub(255,127,80);
    DrawRectangle(mX,mY,mX+mW,mY+mH);
    glColor3ub(255,255, 255);
    DrawRectangle(sX-.1,mY-.2,sX+.1,mY+mH+.2);
}

bool Slider::CheckPushed(double x, double y){
    if(x >= mX && x <= mX+mW){
        if(y >= mY-.2 && y <= mY+mH+.2){
            sX = x;
            return true;
        }
    }
    return false;
}
int Slider::GetWeight(double x, double y){
    double dif = sX+.2 - mX;
    double ratio = dif/mW;
    int wall_weight = ratio*10;
    if(ratio > .99){
        wall_weight = gW*gH+10;
    }
    if(wall_weight < 1){
        wall_weight = 1;
    }
    return wall_weight;
}