/*
Author : Akshay Venugopal
Program : Polygon Clipping
Roll No : 207
*/ 

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

POLYGON poly;

void render();

void drawInnerWindow()
{
	glColor3f(BLACK.red, BLACK.green, BLACK.blue);
	glBegin(GL_LINES);
		glVertex2i(XW_MIN, YW_MIN);
		glVertex2i(XW_MIN, YW_MAX);
		
		glVertex2i(XW_MIN, YW_MAX);
		glVertex2i(XW_MAX, YW_MAX);
		
		glVertex2i(XW_MAX, YW_MAX);
		glVertex2i(XW_MAX, YW_MIN);

		glVertex2i(XW_MAX, YW_MIN);
		glVertex2i(XW_MIN, YW_MIN);
	glEnd();
}

void drawPolygon(POLYGON poly)
{
	cout << "drawing polygon" << endl;
	glColor3f(GREEN.red, GREEN.green, GREEN.blue);
	glBegin(GL_POLYGON);
		for(int i = 0; i < poly.vertexCount; i++)
			glVertex2i(poly.vertex[i].x, poly.vertex[i].y);
	glEnd();
}

void printVertices(POLYGON poly)
{
	int vCount = poly.vertexCount;
	cout << "\nPolygon vertices : " << endl;
	for( int i = 0; i < vCount; i++ )
	{
		cout << "(" << poly.vertex[i].x << " , " << poly.vertex[i].y << ")" << endl;
	}
}

int inside(POINT p, int edge, int mul, int axis  )
{
	if( axis == HORIZONTAL && mul*p.x >= mul*edge )
		return true;
	if( axis == VERTICAL && mul*p.y >= mul*edge )
		return true;
	return false;
}

void copyPolygon(POLYGON *poly, const POLYGON src)
{
	int vCount = src.vertexCount;
	for( int i = 0; i < vCount; i++ )
	{
		poly->vertex[i] = src.vertex[i];
	}
	poly->vertexCount = src.vertexCount;
}

void addVertex(POLYGON *dst, POINT v)
{
	dst->vertex[dst->vertexCount] = v;
	dst->vertexCount++;
}                                                                                                                                                       

POINT intersect(POINT v1, POINT v2, int edge, int axis)
{
	POINT tmp;
	float slope = (float)(v2.y - v1.y)/(float)(v2.x - v1.x) ;
	if( axis == HORIZONTAL )
	{
		tmp.x = edge;
		tmp.y = v1.y + (edge - v1.x)*slope;
	}
	else if( axis == VERTICAL )
	{
		tmp.x = v1.x + (edge - v1.y)/slope;
		tmp.y = edge;
	}
	return tmp;
}

void clipper(POLYGON *poly, int edge, int mul, int axis)
{
	POLYGON temp;
	POINT u, v, inter;
	int vCount = poly->vertexCount;
	temp.vertexCount = 0;
	for( int i = 0; i < vCount; i++ )
	{
		u = poly->vertex[i];
		v = poly->vertex[( (i+1)%vCount )];
		if( inside(u, edge, mul, axis) )
		{
			if( inside(v, edge, mul, axis ) )
				addVertex(&temp, v);
			else
			{
				inter = intersect(u, v, edge, axis);
				addVertex(&temp, inter);
			}
		}
		else if( inside(v, edge, mul, axis) )
		{
			inter = intersect(u, v, edge, axis);
			addVertex(&temp, inter);
			addVertex(&temp, v);
		}
	}
	copyPolygon(poly, temp);
}

void polyClip(POLYGON *poly)
{
	printVertices(*poly);
	cout << "\nLeft Clipper" << endl; 
	clipper(poly, XW_MIN, 1, HORIZONTAL);
	printVertices(*poly);
	sleep(2); render();
	cout << "\nRight Clipper" << endl;
	clipper(poly, XW_MAX, -1, HORIZONTAL);
	printVertices(*poly);
	sleep(2); render();
	cout << "\nBottom Clipper" << endl;
	clipper(poly, YW_MIN, 1, VERTICAL);
	printVertices(*poly);
	sleep(2); render();
	cout << "\nTop Clipper" << endl;
	clipper(poly, YW_MAX, -1, VERTICAL);
	printVertices(*poly);
	render();
}

POINT begPoint, endPoint;

int polyFlag = 0;

void render()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawPolygon(poly);
	drawInnerWindow();
	glFlush();
}

void reset()
{
	poly.vertexCount = 0;
	polyFlag = 0;
	render();
}

void selectPoint(int x, int y)
{
	cout << "selecting point " << poly.vertexCount+1 << " : " << x << " , " << y << endl;
	poly.vertex[poly.vertexCount].x = x;
	poly.vertex[poly.vertexCount].y = y;
	poly.vertexCount++;
}

void mouseClick(int button, int state, int x, int y)
{
	if( button == GLUT_LEFT_BUTTON )
	{
		if( state == GLUT_DOWN && polyFlag == 0)
		{
			selectPoint(x, 500-y);
		}
	}
}

void keyboardPress(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'd': polyFlag = 1; render(); break;
		case 'c': polyClip(&poly); break;
		case 'r': reset(); break;
		default : cout << "Invalid Key" << endl;
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(800, 500);
	glutCreateWindow("OpenGL LAB");
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 500);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glutDisplayFunc(render);
	glutMouseFunc(mouseClick);
	glutKeyboardFunc(keyboardPress);
	poly.vertexCount = 0;
	glutMainLoop();
	return 0;
}	


//--------------------------END OF PROGRAM------------------------//
