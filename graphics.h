#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <cmath>
#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>
#if WIN32
#include "glut.h"
#else
#include <GL/freeglut.h>
#endif
#include "grid.h"
#include "unistd.h"

enum Algorithms {DIJKSTRA, BIDIRECTIONAL};

int const gW = 60;
int const gH = 40;
int const gMargin = 3;

void DrawRectangle(double x1, double y1, double x2, double y2);
void DrawLine(double x1, double y1, double x2, double y2);
void DrawText(double x, double y, const char *string);

#endif //GRAPHICS_H
