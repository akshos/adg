#include <GL/glut.h>
#include <iostream>
#include <queue>
#include <cmath>
#include <unistd.h>
#include <stdio.h>

#define RADIUS 30
#define EDGE_WIDTH 2

#define XW_MIN 170
#define XW_MAX 600
#define YW_MIN 120
#define YW_MAX 400

#define HORIZONTAL 1
#define VERTICAL 2

using namespace std;

struct COLOR
{
	float red, green, blue;
};

const COLOR WHITE = {0.95, 0.95, 0.95};
const COLOR BLACK = {0.0, 0.0, 0.0};
const COLOR RED   = {0.8, 0.0, 0.0};
const COLOR GREEN = {0.0, 0.7, 0.0};
const COLOR BLUE  = {0.0, 0.0, 0.8};
const COLOR GRAY  = {0.6, 0.6, 0.6};

struct POINT
{
	int x, y;
};

struct LINE
{
	POINT beg, end;
};

struct POLYGON
{
	POINT vertex[10];
	int vertexCount;
};

static POLYGON poly;
static LINE lineList[20];
static int lineCount = 0;

//OpenGL specific functions
void initializeOpenGL(int argc, char **argv);
void render();
void drawLine( POINT a, POINT b, COLOR color, int width = 1);
void drawLine( int x1, int y1, int x2, int y2, COLOR color, int width = 1);

void addLine(POINT beg, POINT end);
void drawInnerWindow();
void drawPolygon(POLYGON poly);
void polyClip(POLYGON *poly);
void cohenSutherland(LINE *line);
