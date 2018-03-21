/*
AUTHOR : Akshay Venugopal
PROGRAM : Scan Line Fill
Roll No: 207
*/

#include <GL/glut.h>
#include <iostream>
#include <queue>
#include <cmath>
#include <unistd.h>
#include <stdio.h>

#define EDGE_WIDTH 2

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

POINT startPoint, endPoint;
LINE edgeList[50];
int processed[50] = {0};
LINE scanLine[500][10];
int scanLineCount[500] = {0};
int edgeCount = 0;
int flag = 0;

void render();

int operator!=(unsigned char color_array[], COLOR &color)
{
	if( ((float)color_array[0]/255.0) == color.red )
		if( ((float)color_array[1]/255.0) == color.green )
			if( ((float)color_array[2]/255.0) == color.blue )
				return 0;
	return 1;
}

int operator!=(float color_array[], COLOR color)
{
	if( color_array[0] == color.red )
		if( color_array[1] == color.green )
			if( color_array[2] == color.blue )
				return 0;
	return 1;
}

void drawPolygon()
{
	//cout << "drawing polygon with edges : " << edgeCount <<  endl;
	glColor3f(BLACK.red, BLACK.green, BLACK.blue);
	if(flag == 0)
	 return;
	for(int i = 0; i < edgeCount; i++)
	{
		glBegin(GL_LINES);
			glVertex2i(edgeList[i].beg.x, edgeList[i].beg.y);
			glVertex2i(edgeList[i].end.x, edgeList[i].end.y);
		glEnd();
	}
	
}

void reset()
{
	edgeCount = 0;
	flag = 0;
	render();
}

void printEdgeList()
{
	for( int i = 0; i < edgeCount; i++ )
	{
		cout << "(" << edgeList[i].beg.x << "," << edgeList[i].beg.y << ")";
		cout << "->(" << edgeList[i].end.x << "," << edgeList[i].end.y << ")" << endl;
	}
}

void sortEdgeList()
{
	LINE tmp;
	int x1, x2;
	for( int i = 0; i < edgeCount - 1; i++ )
	{
		for( int j = 0; j < edgeCount - i - 1; j++ )
		{
			x1 = (edgeList[j].beg.x<edgeList[j].end.x)?edgeList[j].beg.x:edgeList[j].end.x;
			x2 = (edgeList[j+1].beg.x<edgeList[j+1].end.x)?edgeList[j+1].beg.x:edgeList[j+1].end.x;
			if(x1 > x2)
			{
				tmp = edgeList[j];
				edgeList[j] = edgeList[j+1];
				edgeList[j+1] = tmp;
			}
		}
	}
}

void addEdge(POINT beg, POINT end)
{
	if( beg.y > end.y )
	{
		POINT tmp = end;
		end = beg;
		beg = tmp;
	}
	edgeList[edgeCount].beg = beg;
	edgeList[edgeCount].end = end;
	edgeCount++;
}



int findOppEdge(int i, int y)
{
	for( int j = i+1; j < edgeCount; j++ )
	{
		if(edgeList[j].beg.y <= y && edgeList[j].end.y >= y)
		{
			processed[j] = 1;
			return j;
		}
	}
	return -1;
}

int findBetweenEdge(int i, int j, int y1, int y2)
{
	int xmax = (edgeList[j].end.x<edgeList[j].beg.x)?edgeList[j].end.x:edgeList[j].beg.x;
	int xmin = (edgeList[i].end.x>edgeList[i].beg.x)?edgeList[i].end.x:edgeList[i].beg.x;
	for( int k = i + 1; k < edgeCount; k++ )
	{
		if(edgeList[k].beg.y > y1 && edgeList[k].beg.y < y2)
			if(edgeList[k].beg.x > xmin && edgeList[k].beg.x < xmax)
				if(k != j)
					return k;
	}
	return -1;
}

void scanFill(int &x1, int &y, float islope, float islope2, int ymax)
{
	float X1 = x1;
	float X2 = (float)((float)y * islope2);
	glColor3f(0, 0, 0);
	while(y < ymax)
	{
		glBegin(GL_LINES);
			glVertex2i((int)X1, y);
			glVertex2i((int)X2, y);
		glEnd();
		glFlush();
		y++;
		X1 += islope;
		X2 += islope2;
	}
}

void processEdge(LINE edge, int i)
{
	float slope = (float)(edge.end.y - edge.beg.y)/(float)(edge.end.x - edge.beg.x);
	if(slope == 0)
	{
		processed[i] = 1;
		return;
	}
	float islope = 1/slope;
	int x = edge.beg.x;
	int y = edge.beg.y;
	while(y < edge.end.y)
	{
		int n = findOppEdge(i, y);
		if(n == -1)
		{
			cout << "ERROR"; exit(1);
		}
		int m = findBetweenEdge(i, n, y, edgeList[i].end.y);
		int ymax = (edgeList[i].end.y<edgeList[n].end.y)?edgeList[i].end.y:edgeList[n].end.y;
		if(m != -1)
		{
			ymax = edgeList[m].beg.y;
		}
		float slope2 = (float)(edgeList[n].end.y - edgeList[n].beg.y)/(float)(edgeList[n].end.x - edgeList[n].beg.x);
		float islope2 = 1/slope2;
		scanFill(x, y, islope, islope2, ymax);
		x = (int)(y * islope);
	}
}

void polyfill()
{
	cout << "Sorting the edges" << endl;
	sortEdgeList();
	printEdgeList();
	for( int i = 0; i < edgeCount; i++ )
	{
		if(processed[i] == 0)
		{
			cout << "Processing edge : " << i << endl;
			processEdge(edgeList[i], i);
		}
	}

} 

void render()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawPolygon();
	glFlush();
	//polyfill();
	glFlush();
}

void selectPoint(int x, int y)
{
	static POINT beg, end;
	if( flag == 0 )
	{
		beg.x = x; beg.y = y;
		startPoint = beg;
		flag = 1;
		return;
	}
	end.x = x;
	end.y = y;
	endPoint = end;
	addEdge(beg, end);
	beg = end;
}

void mouseClick(int button, int state, int x, int y)
{
	if( button == GLUT_LEFT_BUTTON )
	{
		if( state == GLUT_DOWN )
		{
			selectPoint(x, 500-y);
			render();
		}
		
	}
}

void keyboardPress(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'd': addEdge(startPoint, endPoint); render(); break;
		case 'r': reset(); break;
		case 'p': printEdgeList(); break;
		case 'f': polyfill(); break;
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
	edgeCount = 0;
	glutMainLoop();
	return 0;
}	


//--------------------------END OF PROGRAM------------------------//


