#include "primitives.h"

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
	cout << "Filling color at postion x=" << x << " y=" << y << endl;
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

void drawCircle( POINT center, int radius, COLOR fillColor )
{
	drawCircle(center.x, center.y, radius, fillColor);
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
	if( nodeCount == 0 )
	{
		cout << "EMPTY" << endl;
		return;
	}
	for( int i = 0; i < nodeCount; i++ )
	{
		cout << "NODE: " << nodeList[i].id;
		cout << "\tX: " << nodeList[i].xCenter;
		cout << "\tY: " << nodeList[i].yCenter;
		cout << "\tKEY: " << nodeList[i].key;
		cout << "\t\tSTART: " << nodeList[i].startTime;
		cout << "\tFINISH: " << nodeList[i].finishTime;
		cout << "\tPARENT: ";
		if( nodeList[i].parent != NULL )
			cout << nodeList[i].parent->id;
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
	for( int i = 0; i < edgeCount; i++ )
	{
		cout << edgeList[i].begNode->id << " --> " << edgeList[i].endNode->id << endl;
	}
}

void drawNodeKey(NODE *node)
{
	char ch[5];
	sprintf(ch, "%d", node->key);
	drawString(node->xCenter-3, node->yCenter-3, ch, BLACK);
}

void drawNodeTime(NODE *node)
{
	char ch[5];
	sprintf(ch, "%d", node->startTime);
	drawString(node->xCenter-3, node->yCenter-12, ch, BLACK);
	sprintf(ch, "%d", node->finishTime);
	drawString(node->xCenter-3, node->yCenter-22, ch, BLACK);
} 

void drawNode(NODE *node)
{
	drawCircle(node->xCenter, node->yCenter, RADIUS, node->color);
	drawCharacter(node->xCenter-5, node->yCenter+8, node->id, BLACK);
	glFlush();
	drawNodeKey(node);
	drawNodeTime(node);
}

void changeNodeColor(NODE *node, COLOR color)
{
	cout << "Changing node color" << node->id << endl;
	node->color = color;
	drawNode(node);
	cout << "flushing chage.." << endl;
	glFlush();
	cout << "Changed node color" << node->id << endl;
}

void addNode( int xCenter, int yCenter )
{
	nodeList[nodeCount].xCenter = xCenter;
	nodeList[nodeCount].yCenter = yCenter;
	nodeList[nodeCount].color = RED;
	nodeList[nodeCount].id = currID;
	nodeList[nodeCount].startTime = 0;
	nodeList[nodeCount].key = 0;
	nodeList[nodeCount].finishTime = 0;
	nodeList[nodeCount].parent = NULL;
	nodeList[nodeCount].adjListHeader = NULL;
	drawNode(&nodeList[nodeCount]);
	addAdjListHeader(&nodeList[nodeCount]);
	currID = (char)((int)currID + 1);
	nodeCount++;
}	

NODE *getNodeByXY(int x, int y)
{
	int xCenter, yCenter, radius, result;
	for( int i = 0; i < nodeCount; i++ )
	{
		xCenter = nodeList[i].xCenter;
		yCenter = nodeList[i].yCenter;
		radius = RADIUS;
		result = pow((x-xCenter), 2) + pow((y-yCenter), 2) - pow(radius, 2);
		if( result < 0 )
			return &nodeList[i];
	}
	return NULL;
}

void addAdjListHeader(NODE *node)
{
	ADJ_LIST *newAdj = new ADJ_LIST;
	newAdj->node = node;
	newAdj->down = NULL;
	newAdj->next = NULL;
	newAdj->last = newAdj;
	node->adjListHeader = newAdj;
	if( adjListBeg == NULL )
	{
		adjListBeg = adjListEnd = newAdj;
	}
	else
	{
		adjListEnd->down = newAdj;
		adjListEnd = newAdj;
	}
}		

void addAdjListEdge(EDGE *edge)
{
	ADJ_LIST *newAdj = new ADJ_LIST;
	ADJ_LIST *header;
	newAdj->down = NULL;
	newAdj->next = NULL;
	newAdj->last = NULL;
	newAdj->node = edge->endNode;
	header = edge->begNode->adjListHeader;
	header->last->next = newAdj;
	header->last = newAdj;
	
	newAdj = new ADJ_LIST;
	newAdj->down = NULL;
	newAdj->next = NULL;
	newAdj->last = NULL;
	newAdj->node = edge->begNode;
	header = edge->endNode->adjListHeader;
	header->last->next = newAdj;
	header->last = newAdj;		
}

void drawEdge(EDGE *edge, COLOR color)
{
	NODE *begNode = edge->begNode;
	NODE *endNode = edge->endNode;
	drawLine(begNode->xCenter, begNode->yCenter, endNode->xCenter, endNode->yCenter, color, 5);	
}

void changeEdgeColor(NODE *begNode, NODE *endNode, COLOR color)
{
	drawLine(begNode->xCenter, begNode->yCenter, endNode->xCenter, endNode->yCenter, color, 5);
}

void createEdge(NODE *begNode, NODE *endNode)
{
	drawLine(begNode->xCenter, begNode->yCenter, endNode->xCenter, endNode->yCenter, BLACK, 5);
	edgeList[edgeCount].begNode = begNode;
	edgeList[edgeCount].endNode = endNode;
	edgeList[edgeCount].weight = 0;
	drawEdge(&edgeList[edgeCount], BLACK);
	begNode->color = BLUE;
	endNode->color = BLUE;
	drawNode(begNode);
	drawNode(endNode);
	addAdjListEdge(&edgeList[edgeCount]);
	edgeCount++;
}

void drawAllEdges()
{
	for( int i = 0; i < edgeCount; i++ )
	{
		drawEdge( &edgeList[i], BLACK );
	}
}

void drawAllNodes()
{
	for( int i = 0; i < nodeCount; i++ )
	{
		drawNode( &nodeList[i] );
	}
}

void bfs()
{
	cout << "\nStarting BFS\n";
	NODE *start = &nodeList[0], *u, *v;
	ADJ_LIST *adjList;
	cout << "Start Node : " << start->id << endl;
	sleep(1);
	
	cout << "Initializing Nodes...\n";
	for( int i = 0; i < nodeCount; i++ )
	{
		nodeList[i].key = -1;
		nodeList[i].parent = NULL;
		changeNodeColor(&nodeList[i], WHITE); sleep(1);
	}
	changeNodeColor(start, GRAY); sleep(1);
	start->key = 0;
	queue<NODE *> nodeQueue;
	nodeQueue.push(start);
	
	while( !nodeQueue.empty() )
	{
		u = nodeQueue.front();
		nodeQueue.pop();
		changeNodeColor(u, RED); sleep(1);
		for(adjList = u->adjListHeader->next; adjList != NULL; adjList = adjList->next)
		{
			v = adjList->node;
			if( v->color == WHITE )
			{
				v->key = u->key+1;
				v->parent = u;
				changeEdgeColor(u, v, GREEN); usleep(500000);
				changeNodeColor(v, GRAY); sleep(1);
				nodeQueue.push(v);
			}
		}
		changeNodeColor(u, GREEN); sleep(1);
	}
	cout << "BFS finished..\n";
}

void dfs()
{
	cout << "\nStarting DFS\n";
	NODE *u;
	sleep(1);
	
	cout << "Initializing nodes..\n";
	for( int i = 0; i < nodeCount; i++ )
	{
		nodeList[i].parent = NULL;
		changeNodeColor(&nodeList[i], WHITE); sleep(1);
	}
	currTime = 0;
	for( int i = 0; i < nodeCount; i++ )
	{
		if( nodeList[i].color == WHITE )
		{
			dfsVisit(&nodeList[i]);
		}
	}
}

void dfsVisit(NODE *u)
{
	NODE *v; ADJ_LIST *adjList;
	currTime = currTime + 1;
	u->startTime = currTime;
	changeNodeColor(u, GRAY); sleep(1);
	for(adjList = u->adjListHeader->next; adjList != NULL; adjList = adjList->next)
	{
		v = adjList->node;
		if( v->color == WHITE )
		{
			v->parent = u;
			changeEdgeColor(u, v, GREEN); usleep(500000);
			dfsVisit(v);
		}
	}
	currTime = currTime + 1;
	u->finishTime = currTime;
	changeNodeColor(u, GREEN); sleep(1);
}

