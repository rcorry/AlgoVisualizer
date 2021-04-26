#include <fstream>
#include "graphics.h"

Grid gGrid;


// Global Variables (Only what you need!)
double screen_x = 3000;
double screen_y = 850;
double gXConv = screen_x/gW;
double gYConv = screen_y/gH;

bool gMoveStart;
bool gMoveTarget;

Algorithms current_algorithm = DIJKSTRA;



// 
// Functions that draw basic primitives
//
void DrawCircle(double x1, double y1, double radius)
{
	glBegin(GL_POLYGON);
	for(int i=0; i<32; i++)
	{
		double theta = (double)i/32.0 * 2.0 * 3.1415926;
		double x = x1 + radius * cos(theta);
		double y = y1 + radius * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();
}

void DrawRectangle(double x1, double y1, double x2, double y2)
{
	glBegin(GL_QUADS);
	glVertex2d(x1,y1);
	glVertex2d(x2,y1);
	glVertex2d(x2,y2);
	glVertex2d(x1,y2);
	glEnd();
}

void DrawLine(double x1, double y1, double x2, double y2)
{
	glBegin(GL_LINES);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glEnd();
}

void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
{
	glBegin(GL_TRIANGLES);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glVertex2d(x3,y3);
	glEnd();
}

// Outputs a string of text at the specified location.
void DrawText(double x, double y, const char *string)
{
	void *font = GLUT_BITMAP_HELVETICA_18;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	
	int len, i;
	glRasterPos2d(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) 
	{
		glutBitmapCharacter(font, string[i]);;
	}

    glDisable(GL_BLEND);
}


//
// GLUT callback functions
//

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	gGrid.DrawGrid();
	gGrid.DrawUI();
	glutSwapBuffers();
}

void UIdisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyboard(unsigned char c, int x, int y)
{
	switch (c) 
	{
		case 27: // escape character means to quit the program
			exit(0);
			break;
		case 'd':
			{
				std::vector<Cell> test = gGrid.Dijkstra();
				break;
			}
		case 'b':
			{
				std::vector<Cell> test = gGrid.Bidirectional();
				break;
			}
		case 'c':
			{
				gGrid.ClearAll();
				break;
			}
		case 'r':
			{
				gGrid.Reset();
				break;
			}
		default:
			return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
	// Reset our global variables to the new width and height.

	screen_x = w;
	screen_y = h;
	gXConv = screen_x/gW;
	gYConv = screen_y/gH;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	// Set the projection mode to 2D orthographic, and set the world coordinates:
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, gW, 0, gH);
	glMatrixMode(GL_MODELVIEW);

}


// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void passiveMouse(int x, int y){
	gXConv = screen_x/gW;
	gYConv = screen_y/gH;
	double t = x/gXConv;
	x = x/gXConv;
	y = y/gYConv;
	y = gH-y-1;
	if(gMoveStart){
		gGrid.MoveStart(x, y);
	}
	if(gMoveTarget){
		gGrid.MoveTarget(x, y);
	}

	if(y < gH-gMargin){
		gGrid.DrawWall(x, y);
	}
	if(gGrid.mSlider.CheckPushed(t,y)){
		int w = gGrid.mSlider.GetWeight(t,y);
		gGrid.SetWeight(w);
	}
	glutPostRedisplay();
}
void mouse(int mouse_button, int state, int x, int y)
{
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		Cell start = gGrid.GetStartCell();
		Cell target = gGrid.GetTargetCell();

		gXConv = screen_x/gW;
		gYConv = screen_y/gH;
		double t = x/gXConv;
		x = x/gXConv;
		y = y/gYConv;
		y = gH-y-1;

		if(x==start.GetX() && y==start.GetY()){
			gMoveStart = true;
		}
		if(x==target.GetX() && y==target.GetY()){
			gMoveTarget = true;
		}

		if(y < gH-gMargin){
			gGrid.DrawWall2(x, y);
		} else{
			if(gGrid.mButtons[0].CheckPushed(x,y)){
				gGrid.Reset();
				if(current_algorithm == BIDIRECTIONAL)
					gGrid.Bidirectional();
				else
					gGrid.Dijkstra();
			}
			if(gGrid.mButtons[1].CheckPushed(x,y)){
				gGrid.ClearAll();
			}
			if(gGrid.mButtons[2].CheckPushed(x,y)){
				gGrid.Reset();
			}
			if(gGrid.mButtons[3].CheckPushed(x,y)){
				current_algorithm = DIJKSTRA;
			}
			if(gGrid.mButtons[4].CheckPushed(x,y)){
				current_algorithm = BIDIRECTIONAL;
			}
			if(gGrid.mSlider.CheckPushed(t,y)){
				int w = gGrid.mSlider.GetWeight(t,y);
				gGrid.SetWeight(w);
			}
		}
	}

	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
		gMoveStart = false;
		gMoveTarget = false;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
	}
	gGrid.SetSize1();
	glutPostRedisplay();
}

// Your initialization code goes here.
void InitializeMyStuff()
{
	screen_x = glutGet(GLUT_WINDOW_WIDTH);
	screen_y = glutGet(GLUT_WINDOW_HEIGHT);

	gXConv = screen_x/gW;
	gYConv = screen_y/gH;


}

void save_to_file(std::string filename){
	ofstream fout;
	fout.open(filename);
	fout << gGrid.sX << " " << gGrid. sY << "\n";
	fout << gGrid.tX << " " << gGrid. tY << "\n";
	for(int x=0; x<gW; x++){
		for(int y=0; y<gH-gMargin; y++){
			if(gGrid.mGrid[x][y].isWall){
				fout << x << " " << y << " 1 " << gGrid.mGrid[x][y].mWeight << "\n";
			} else {
				fout << x << " " << y << " 0 " << gGrid.mGrid[x][y].mWeight << "\n";
			}
		}
	}
	fout.close();

}
void load_from_file(std::string filename){
	ifstream fin;
	fin.open(filename);
	int sx, sy, tx, ty;
	fin >> sx >> sy >> ws;
	fin >> tx >> ty >> ws;
	gGrid.MoveStart(sx,sy);
	gGrid.MoveTarget(tx,ty);
	while(!fin.eof()){
		int x, y, wall, weight;
		fin >> x >> y >> wall >> weight >> ws;
		gGrid.mGrid[x][y].isWall = wall;
		gGrid.mGrid[x][y].mWeight = weight;
	}
	fin.close();
}

void save_menu(int id){
	std::string filename = "./saves/save" + std::to_string(id) + ".txt";
	save_to_file(filename);
}
void load_menu(int id){
	gGrid.ClearAll();
	std::string filename = "./saves/save" + std::to_string(id) + ".txt";
	load_from_file(filename);

}
void top_menu(int id){}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("AlgoVisualizer");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(passiveMouse);

	int sub_menu1 = glutCreateMenu(save_menu);
	glutAddMenuEntry("save1",1);
	glutAddMenuEntry("save2",2);
	glutAddMenuEntry("save3",3);
	glutAddMenuEntry("save4",4);
	glutAddMenuEntry("save5",5);

	int sub_menu2 = glutCreateMenu(load_menu);
	glutAddMenuEntry("load1",1);
	glutAddMenuEntry("load2",2);
	glutAddMenuEntry("load3",3);
	glutAddMenuEntry("load4",4);
	glutAddMenuEntry("load5",5);

	glutCreateMenu(top_menu);
	glutAddSubMenu("Save", sub_menu1);
	glutAddSubMenu("Load", sub_menu2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glColor3d(0,0,0); // forground color
	glClearColor(1, 1, 1, 0); // background color
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}