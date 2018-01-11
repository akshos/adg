#include <GL/glut.h>
#include <iostream>
#include <queue>
#include <cmath>
#include <unistd.h>
#include <stdio.h>

#define RADIUS 30

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

struct ADJ_LIST
{
	NODE *node;
	ADJ_LIST *down;
	ADJ_LIST *next;
	ADJ_LIST *last;
};

struct NODE
{
	int xCenter, yCenter, key;
	int startTime, finishTime;
	COLOR color;
	char id;
	NODE *parent;
	ADJ_LIST *adjListHeader;
};

struct EDGE
{
	int weight;
	NODE *begNode, *endNode;
};

//static variable definitions
static NODE nodeList[20];
static EDGE edgeList[20];
static ADJ_LIST *adjListBeg, *adjListEnd;
static int nodeCount = 0, edgeCount = 0;
static char currID = 'A';
static int currTime = 0;

//OpenGL specific functions
void initializeOpenGL(int argc, char **argv);
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
void drawNodeTime(NODE *node);
void drawNodeKey(NODE *node);
void changeNodeColor(NODE *node, COLOR color);
void addNode(int xCenter, int yCenter);
NODE *getNodeByXY(int x, int y);
void addAdjListHeader(NODE *node);
void addAdjListEdge(EDGE *edge);
void drawEdge(EDGE *edge, COLOR color);
void changeEdgeColor(NODE *begNode, NODE *endNode, COLOR color);
void createEdge(NODE *begNode, NODE *endNode);
void drawAllEdges();
void drawAllNodes();

//BFS related functions
void bfs();
void dfs();
void dfsVisit(NODE *u);
	
