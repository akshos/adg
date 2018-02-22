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

int operator==(unsigned char color_array[], COLOR color)
{
	if( ((float)color_array[0]/255.0) == color.red )
		if( ((float)color_array[1]/255.0) == color.green )
			if( ((float)color_array[2]/255.0) == color.blue )
				return 1;
	return 0;
}

int operator!=(unsigned char color_array[], COLOR color)
{
	if( ((float)color_array[0]/255.0) == color.red )
		if( ((float)color_array[1]/255.0) == color.green )
			if( ((float)color_array[2]/255.0) == color.blue )
				return 0;
	return 1;
}

int operator==(float color_array[], COLOR color)
{
	if( color_array[0] == color.red )
		if( color_array[1] == color.green )
			if( color_array[2] == color.blue )
				return 1;
	return 0;
}

int operator!=(float color_array[], COLOR color)
{
	if( color_array[0] == color.red )
		if( color_array[1] == color.green )
			if( color_array[2] == color.blue )
				return 0;
	return 1;
}

int operator==(COLOR c1, COLOR c2)
{
	if( c1.red == c2.red && c1.green == c2.green && c1.blue == c2.blue )
		return 1;
	return 0;
}

int operator!=(COLOR c1, COLOR c2)
{
	if( c1.red == c2.red && c1.green == c2.green && c1.blue == c2.blue )
		return 0;
	return 1;
}

/*
NOTE : the exact ordering of the function calls are important
*/
void initializeOpenGL(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(800, 500);
	glutCreateWindow("OpenGL LAB");
	
	//order important for the below calls
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //this call should always come before gluOrtho2D
	gluOrtho2D(0, 800, 0, 500);
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

void drawAllNodesEdges();
void render()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawAllNodesEdges();
	glFlush();
}

void boundaryFill( int x, int y, COLOR fillColor)
{
	float prev_color[3];
	POINT curr, temp;
	curr.x = x; curr.y = y;
	queue<POINT> pixels;
	//push the first point to queue
	pixels.push(curr);
	while( !pixels.empty() )
	{
		curr = pixels.front();
		pixels.pop();
		glReadPixels(curr.x , curr.y , 1 , 1 , GL_RGB , GL_FLOAT , prev_color);
		glColor3f(fillColor.red, fillColor.green, fillColor.blue);
		glBegin(GL_POINTS);
		if( prev_color != BLACK && prev_color != fillColor)
		{
			glVertex2i(curr.x, curr.y);
			temp.x = curr.x+1; temp.y = curr.y;
			pixels.push(temp);
			temp.x = curr.x; temp.y = curr.y+1;
			pixels.push(temp);
			temp.x = curr.x; temp.y = curr.y-1;
			pixels.push(temp);
			temp.x = curr.x-1; temp.y = curr.y;
			pixels.push(temp);
		}
		glEnd();
	}	
}

void floodFill( int x, int y, COLOR fillColor)
{
	float prev_color[3];
	POINT curr, temp;
	curr.x = x; curr.y = y;
	queue<POINT> pixels;
	//push the first point to queue
	pixels.push(curr);
	while( !pixels.empty() )
	{
		curr = pixels.front();
		pixels.pop();
		glReadPixels(curr.x , curr.y , 1 , 1 , GL_RGB , GL_FLOAT , prev_color);
		glColor3f(fillColor.red, fillColor.green, fillColor.blue);
		glBegin(GL_POINTS);
		if( prev_color == WHITE )
		{
			glVertex2i(curr.x, curr.y);
			temp.x = curr.x+1; temp.y = curr.y;
			pixels.push(temp);
			temp.x = curr.x; temp.y = curr.y+1;
			pixels.push(temp);
			temp.x = curr.x; temp.y = curr.y-1;
			pixels.push(temp);
			temp.x = curr.x-1; temp.y = curr.y;
			pixels.push(temp);
		}
		glEnd();
	}	
}

void colorFill( int x, int y, COLOR fillColor )
{
	//boundaryFill(x, y, fillColor);
	floodFill(x, y, fillColor);
}

void circleFillColor(int xCenter, int yCenter, int x, int y, COLOR fillColor) 
{
	int yUp, yDown, limit, i;
	glColor3f(fillColor.red, fillColor.green, fillColor.blue);
	glBegin(GL_POINTS);
		limit = xCenter + x; yUp = yCenter + y; yDown = yCenter - y;
		for( i = xCenter-x+1; i < limit; i++ )
		{
			glVertex2i(i, yUp);
			glVertex2i(i, yDown);
		}
		limit = xCenter + y; yUp = yCenter + x;	yDown = yCenter - x;
		for( i = xCenter-y+1; i < limit; i++ )
		{
			glVertex2i(i, yUp);
			glVertex2i(i, yDown);
		}
	glEnd();
}	

void drawLine( POINT a, POINT b, COLOR color, int width)
{
	glColor3f(color.red, color.green, color.blue);
	glLineWidth(width);
	glBegin(GL_LINES);
		glVertex2i(a.x, a.y);
		glVertex2i(b.x, b.y);
	glEnd();
}

void drawLine( int x1, int y1, int x2, int y2, COLOR color, int width)
{
	glColor3f(color.red, color.green, color.blue);
	glLineWidth(width);
	glBegin(GL_LINES);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
	glEnd();
}

void circlePlotPoints(int xCenter, int yCenter, int x, int y )
{
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
		glVertex2i(xCenter + x, yCenter + y );
		glVertex2i(xCenter - x, yCenter + y );
		glVertex2i(xCenter + x, yCenter - y );
		glVertex2i(xCenter - x, yCenter - y );
		glVertex2i(xCenter + y, yCenter + x );
		glVertex2i(xCenter - y, yCenter + x );
		glVertex2i(xCenter + y, yCenter - x );
		glVertex2i(xCenter - y, yCenter - x );
	glEnd();
}

void drawCircle( int xCenter, int yCenter, int radius, COLOR fillColor )
{
	int x = 0, y = radius, p = 1 - radius;
	circlePlotPoints( xCenter, yCenter, x, y );
	circleFillColor(xCenter, yCenter, x, y, fillColor);
	while( x < y )
	{
		x++;
		if( p < 0 )
		{
			p += (2 * x) + 1;
		}
		else
		{
			y--;
			p += ( 2*(x-y) ) +1;
		}
		circlePlotPoints(xCenter, yCenter, x, y);
		circleFillColor(xCenter, yCenter, x, y, fillColor);
	}
	//colorFill(xCenter, yCenter, fillColor);
}

void drawCharacter(int x, int y, char ch, COLOR color)
{
	glColor3f( color.red, color.green, color.blue );
	glRasterPos2i(x, y);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ch);
}

void drawString(int x, int y, char *str, COLOR color)
{
	glColor3f(color.red, color.green, color.blue);
	glRasterPos2i(x, y);
	for( str; *str != '\0'; str++ )
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *str);
}

//-----------------------------------------------------------------
//GRAPH SPECIFIC FUNCTIONS
//-----------------------------------------------------------------

void addAdjListHeader(NODE *node);

void printAdjList()
{
	ADJ_LIST *header, *temp;
	header = adjListBeg;
	cout << "\nAdjacency List\n";
	if( adjListBeg == NULL )
	{
		cout << "EMPTY" << endl;
		return;
	}
	while( header != NULL )
	{
		temp = header->next;
		cout << header->node->id << " --> ";
		while( temp != NULL )
		{
			cout << temp->node->id << "  ";
			temp = temp->next;
		}
		cout << endl;
		header = header->down;
	}
}

void printNodeList()
{
	cout << "\nNode List : \n";
	if( adjListBeg == NULL )
	{
		cout << "EMPTY" << endl;
		return;
	}
	NODE *node;
	for(ADJ_LIST *tmp = adjListBeg; tmp != NULL; tmp = tmp->down)
	{
		node = tmp->node;
		cout << "NODE: " << node->id;
		cout << "\tX: " << node->xCenter;
		cout << "\tY: " << node->yCenter;
		cout << "\tD: " << node->d;
		cout << "\tF: " << node->f;
		cout << "\tPARENT: ";
		if( node->parent != NULL )
			cout << node->parent->id;
		else
			cout << "none";
		cout << endl;
	}
}

void printEdgeList()
{
	cout << "\nEdge List:\n";
	if(edgeCount == 0)
	{
		cout << "EMPTY" << endl;
		return;
	}
	for(int i = 0; i < edgeCount; i++)
	{
		cout << edgeList[i].node1->id << "---" << edgeList[i].node2->id << endl;
	}
}

void drawNode(NODE *node)
{
	char ch[5];
	//draw node circle
	drawCircle(node->xCenter, node->yCenter, RADIUS, node->color);
	//draw node label
	drawCharacter(node->xCenter-5, node->yCenter+8, node->id, BLACK);
	//draw node d and f values
	sprintf(ch, "%d", node->d);
	drawString(node->xCenter-3, node->yCenter-6, ch, BLACK);
	sprintf(ch, "%d", node->f);
	drawString(node->xCenter-3, node->yCenter-17, ch, BLACK);
}

void changeNodeColor(NODE *node, COLOR color)
{
	node->color = color;
	render();
}

void addNode( int xCenter, int yCenter )
{
	NODE *newNode = new NODE;
	newNode->xCenter = xCenter;
	newNode->yCenter = yCenter;
	newNode->color = RED;
	newNode->id = currID;
	newNode->d = 0;
	newNode->f = 0;
	newNode->parent = NULL;
	newNode->adjListHeader = NULL;
	newNode->pedge = NULL;
	//drawNode(newNode);
	addAdjListHeader(newNode);
	currID = (char)((int)currID + 1);
	nodeCount++;
	render();
}	

NODE *getNodeByXY(int x, int y)
{
	int xCenter, yCenter, radius, result;
	NODE *node;
	for( ADJ_LIST *header = adjListBeg; header != NULL; header = header->down )
	{
		node = header->node;
		xCenter = node->xCenter;
		yCenter = node->yCenter;
		radius = RADIUS;
		result = pow((x-xCenter), 2) + pow((y-yCenter), 2) - pow(radius, 2);
		if( result < 0 )
			return node;
	}
	return NULL;
}

void addAdjListHeader(NODE *node)
{
	ADJ_LIST *newAdjHeader = new ADJ_LIST;
	newAdjHeader->node = node;
	newAdjHeader->edge = NULL;
	newAdjHeader->down = NULL;
	newAdjHeader->next = NULL;
	newAdjHeader->last = newAdjHeader;
	node->adjListHeader = newAdjHeader;
	if( adjListBeg == NULL )
	{
		adjListBeg = adjListEnd = newAdjHeader;
	}
	else
	{
		adjListEnd->down = newAdjHeader;
		adjListEnd = newAdjHeader;
	}
}		

void addAdjListEdge(EDGE *edge)
{
	ADJ_LIST *header;
	//add node2 to adj list of node1
	ADJ_LIST *newAdj = new ADJ_LIST;
	newAdj->down = NULL;
	newAdj->next = NULL;
	newAdj->last = NULL;
	newAdj->node = edge->node2;
	newAdj->edge = edge;
	header = edge->node1->adjListHeader;
	header->last->next = newAdj;
	header->last = newAdj;	
}

void drawEdge(EDGE *edge, COLOR color, int weight)
{
	drawLine(edge->edgeBeg.x, edge->edgeBeg.y, edge->edgeEnd.x, edge->edgeEnd.y, color, EDGE_WIDTH);
	int x, y;
	x = edge->edgeBeg.x + 0.7*(edge->edgeEnd.x - edge->edgeBeg.x);
	y = edge->edgeBeg.y + 0.7*(edge->edgeEnd.y - edge->edgeBeg.y);
	drawCharacter(x+5, y+5, (char)('0' + weight), BLUE);
}

void createEdge(NODE *node1, NODE *node2, POINT edgeBeg, POINT edgeEnd, int weight)
{
	node1->color = BLUE;
	node2->color = BLUE;
	edgeList[edgeCount].node1 = node1;
	edgeList[edgeCount].node2 = node2;
	edgeList[edgeCount].color = BLACK;
	edgeList[edgeCount].weight = weight;
	edgeList[edgeCount].edgeBeg = edgeBeg;
	edgeList[edgeCount].edgeEnd = edgeEnd;
	addAdjListEdge(&edgeList[edgeCount]);
	edgeCount++;
	render();
}

void changeEdgeColor(EDGE *edge, COLOR color)
{
	edge->color = color;
	render();
}

void drawAllNodesEdges()
{
	for(int i = 0 ; i < edgeCount; i++)
		drawEdge(&edgeList[i], edgeList[i].color, edgeList[i].weight);
	for(ADJ_LIST *header = adjListBeg; header != NULL; header = header->down)
		drawNode(header->node);
}

NODE *getMinNode()
{
	if( adjListBeg == NULL )
		return NULL;
	ADJ_LIST *header = adjListBeg;
	NODE *node, *smallest;
	//find first non green node and set it as smallest
	for( header; header != NULL; header = header->down )
	{
		node = header->node;
		if( node->color != GREEN )
		{
			smallest = node;
			break;
		}
	}		
	//find smallest non green node
	for(header; header != NULL; header = header->down)
	{
		node = header->node;
		if( (node->d < smallest->d) && (node->color != GREEN) )
			smallest = node;
	}
	return smallest;
}

void initializeSingleSource(NODE *s)
{
	for(ADJ_LIST *header = adjListBeg; header != NULL; header = header->down)
	{
		header->node->d = 99;
		header->node->parent = NULL;
		changeNodeColor(header->node, WHITE); usleep(500000);
	}
	s->d = 0;
	render(); sleep(1);
}

void relax(EDGE *edge)
{
	NODE *v = edge->node2, *u = edge->node1;	
	if( v->d > (u->d + edge->weight) )
	{
		v->d = (u->d + edge->weight);
		v->parent = u;
		if( v->pedge != NULL )
		{
			changeEdgeColor(v->pedge, BLACK); sleep(1);
		}
		v->pedge = edge;
		changeEdgeColor(edge, GREEN); sleep(1);
		changeNodeColor(v, GRAY); sleep(1);
	}
}

void dijkstra(NODE *s)
{
	NODE *u, *v;
	initializeSingleSource(s);
	for( int i = 0; i < nodeCount; i++ )
	{
		u = getMinNode();
		cout << "Node : " << u->id << endl;
		changeNodeColor(u, RED); sleep(1);
		for(ADJ_LIST *adjList = u->adjListHeader->next; adjList != NULL; adjList = adjList->next)
		{
			relax(adjList->edge);
		}
		changeNodeColor(u, GREEN); sleep(1);
	}
}

NODE *node1 = NULL, *node2 = NULL;
POINT edgeBeg, edgeEnd;
int edgeWeightFlag = 0, dijkstraFlag = 0;

void selectNode(int x, int y)
{
	if( node1 == NULL )
	{
		node1 = getNodeByXY(x, y);
		if(node1 != NULL )
		{
			cout << "\nStart Node : " << node1->id << endl;
			edgeBeg.x = x; edgeBeg.y = y;
		}
	}
	else
	{
		node2 = getNodeByXY(x, y);
		if( node2 != NULL )
		{
			if( node2 == node1 )
				return;
			cout << "End Node : " << node2->id << endl;		
			edgeEnd.x = x; edgeEnd.y = y;	
			edgeWeightFlag = 1;
		}	
	}
}

void mouseClick(int button, int state, int x, int y)
{
	if( button == GLUT_LEFT_BUTTON )
	{
		if( state == GLUT_DOWN )
		{
			if( dijkstraFlag == 1 )
			{
				NODE *start = getNodeByXY(x, 500-y);
				dijkstra(start);
				dijkstraFlag = 0;
				return;
			}
			addNode(x, 500-y);
			glFlush();
		}
	}
	if( button == GLUT_RIGHT_BUTTON )
	{
		if( state == GLUT_DOWN)
		{
			selectNode(x, 500-y);
			glFlush();
		}
	}
}

void keyboardPress(unsigned char key, int x, int y)
{
	if( edgeWeightFlag == 1 && key <= '9' && key >= '0' )
	{
		int weight = (int)(key-'0');
		cout << "Edge weight = " << weight << endl;
		createEdge(node1, node2, edgeBeg, edgeEnd, weight);
		node1 = node2 = NULL;
		edgeWeightFlag = 0;
		return;
	}
	switch(key)
	{
		case 'n' : printNodeList(); break;
		case 'a' : printAdjList(); break;
		case 'e' : printEdgeList(); break;
		case 'd' : dijkstraFlag = 1; break;
		default : cout << "unknown key" << endl;
	}
}


int main(int argc, char **argv)
{
	initializeOpenGL(argc, argv);
	glutDisplayFunc(render);
	glutMouseFunc(mouseClick);
	glutKeyboardFunc(keyboardPress);
	glutMainLoop();
	glutIdleFunc(render);
	return 0;
}
