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
	//drawNode(newNode);
	addAdjListHeader(newNode);
	currID = (char)((int)currID + 1);
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

void addAdjListEdge(NODE *node1, NODE *node2, EDGE *edge, int weight)
{
	ADJ_LIST *header;
	//add node2 to adj list of node1
	ADJ_LIST *newAdj = new ADJ_LIST;
	newAdj->down = NULL;
	newAdj->next = NULL;
	newAdj->last = NULL;
	newAdj->weight = weight;
	newAdj->node = node2;
	newAdj->edge = edge;
	header = node1->adjListHeader;
	header->last->next = newAdj;
	header->last = newAdj;
	//add node1 to adj list of node2
	newAdj = new ADJ_LIST;
	newAdj->down = NULL;
	newAdj->next = NULL;
	newAdj->last = NULL;
	newAdj->weight = weight;
	newAdj->node = node1;
	newAdj->edge = edge;
	header = node2->adjListHeader;
	header->last->next = newAdj;
	header->last = newAdj;		
}

void drawEdge(NODE *node1, NODE *node2, COLOR color, int weight)
{
	drawLine(node1->xCenter, node1->yCenter, node2->xCenter, node2->yCenter, color, EDGE_WIDTH);
	int x, y;
	x = node1->xCenter + (node2->xCenter - node1->xCenter)/2;
	y = node1->yCenter + (node2->yCenter - node1->yCenter)/2;
	drawCharacter(x+5, y+5, (char)('0' + weight), BLUE);
}

void createEdge(NODE *node1, NODE *node2, int weight)
{
	node1->color = BLUE;
	node2->color = BLUE;
	edgeList[edgeCount].node1 = node1;
	edgeList[edgeCount].node2 = node2;
	edgeList[edgeCount].color = BLACK;
	edgeList[edgeCount].weight = weight;
	addAdjListEdge(node1, node2, &edgeList[edgeCount], weight);
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
		drawEdge(edgeList[i].node1, edgeList[i].node2, edgeList[i].color, edgeList[i].weight);
	for(ADJ_LIST *header = adjListBeg; header != NULL; header = header->down)
		drawNode(header->node);
}
		

void bfs()
{
	cout << "\nStarting BFS\n";
	NODE *start = adjListBeg->node, *u, *v;
	ADJ_LIST *adjList;
	cout << "Start Node : " << start->id << endl;
	sleep(1);
	
	cout << "Initializing Nodes...\n";
	for( ADJ_LIST *header = adjListBeg; header != NULL; header = header->down)
	{
		u = header->node;
		u->d = -1;
		u->parent = NULL;
		changeNodeColor(u, WHITE); sleep(1);
	}
	changeNodeColor(start, GRAY); sleep(1);
	start->d = 0;
	queue<NODE *> nodeQueue;
	nodeQueue.push(start);
	
	while( !nodeQueue.empty() )
	{
		u = nodeQueue.front();
		nodeQueue.pop();
		cout << u->id << " --> ";
		changeNodeColor(u, RED); sleep(1);
		for(adjList = u->adjListHeader->next; adjList != NULL; adjList = adjList->next)
		{
			v = adjList->node;
			if( v->color == WHITE )
			{
				v->d = u->d+1;
				cout << v->id << " ";
				v->parent = u;
				changeEdgeColor(adjList->edge, GREEN); usleep(50000);
				changeNodeColor(v, GRAY); sleep(1);
				nodeQueue.push(v);
			}
		}
		cout << endl;
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
	for( ADJ_LIST *header = adjListBeg; header != NULL; header = header->down )
	{
		u = header->node;
		u->parent = NULL;
		u->d = 0;
		u->f = 0;
		changeNodeColor(u, WHITE); sleep(1);
	}
	currTime = 0;
	for( ADJ_LIST *header = adjListBeg; header != NULL; header = header->down )
	{
		u = header->node;
		if( u->color == WHITE )
		{
			cout << u->id;
			fflush(stdout);
			dfsVisit(u);
			cout << endl;
		}
	}
	cout << "DFS finished..." << endl;
}

void dfsVisit(NODE *u)
{
	NODE *v; ADJ_LIST *adjList;
	currTime = currTime + 1;
	u->d = currTime;
	
	changeNodeColor(u, GRAY); sleep(1);
	for(adjList = u->adjListHeader->next; adjList != NULL; adjList = adjList->next)
	{
		v = adjList->node;
		if( v->color == WHITE )
		{
			v->parent = u;
			cout << " --> " << v->id;
			fflush(stdout);
			changeEdgeColor(adjList->edge, GREEN); usleep(50000);
			dfsVisit(v);
		}
	}
	currTime = currTime + 1;
	u->f = currTime;
	changeNodeColor(u, GREEN); sleep(1);
}

