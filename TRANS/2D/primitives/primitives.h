#include <GL/glut.h>
#include <iostream>
#include <queue>
#include <cmath>
#include <unistd.h>
#include <cstdio>

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

void drawAxis();
void drawPolygon(POLYGON poly);
void translate(POLYGON *poly, int tx, int ty);
void rotate(POLYGON *poly, int xCenter, int yCenter, float angle);
void scale(POLYGON *poly, float sx, float sy);
void render();