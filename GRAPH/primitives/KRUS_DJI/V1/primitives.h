#include <GL/glut.h>
#include <iostream>
#include <queue>
#include <vector>
#include <cmath>
#include <unistd.h>
#include <stdio.h>

#define RADIUS 30
#define EDGE_WIDTH 2

using namespace std;

struct COLOR
{
	float red, green, blue;
};

const COLOR WHITE = {0.95, 0.95, 0.95};
const COLOR BLACK = {0.0, 0.0, 0.0};
const COLOR RED   = {0.8, 0.0, 0.0};
const COLOR GREEN = {0.0, 1.0, 0.0};
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

struct NODE;
struct EDGE;

struct ADJ_LIST
{
	NODE *node;
	EDGE *edge;
	ADJ_LIST *down, *next, *last;
};

struct NODE
{
	int xCenter, yCenter;
	int d, f;
	COLOR color;
	char id;
	NODE *parent;
	ADJ_LIST *adjListHeader;
	EDGE *pedge;
};

struct EDGE
{
	COLOR color;
	int weight;
	NODE *node1, *node2;
	POINT edgeBeg, edgeEnd;
};

//static variable definitions
static EDGE edgeList[20];
static ADJ_LIST *adjListBeg = NULL, *adjListEnd = NULL;
static int edgeCount = 0, nodeCount = 0;
static char currID = 'A';
static int currTime = 0;

//OpenGL specific functions
void initializeOpenGL(int argc, char **argv);
void render();
void drawLine( POINT a, POINT b, COLOR color, int width = 1);
void drawLine( int x1, int y1, int x2, int y2, COLOR color, int width = 1);
void drawCircle( POINT center, int radius, COLOR fillColor );
void drawCircle( int xCenter, int yCenter, int radius, COLOR fillColor );
void circlePlotPoints(int xCenter, int yCenter, int x, int y );
void drawCharacter(int x, int y, char ch, COLOR color);
void drawString(int x, int y, char *str, COLOR color);
int operator==(unsigned char color_array[], COLOR color);
int operator!=(unsigned char color_array[], COLOR color);
int operator!=(COLOR c1, COLOR c2);
int operator==(COLOR c1, COLOR c2);
int operator!=(float color_array[], COLOR color);
int operator==(float color_array[], COLOR color);

//Graph specific functions
void printNodeList();
void printAdjList();
void printEdgeList();
void drawNode(NODE *node);
void changeNodeColor(NODE *node, COLOR color);
void addNode(int xCenter, int yCenter);
NODE *getNodeByXY(int x, int y);
void addAdjListHeader(NODE *node);
void addAdjListEdge(NODE *node1, NODE *node2, EDGE *edge);
void drawEdge(EDGE *edge, COLOR color, int weight);
void createEdge(NODE *begNode, NODE *endNode, POINT edgeBeg, POINT edgeEnd, int weight);
void changeEdgeColor(EDGE *edge, COLOR color);
void drawAllNodesEdges();

void dijkstra(NODE *s);
