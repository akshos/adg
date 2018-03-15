/*
AUTHOR : Akshay Venugopal
PROGRAM : 2D Transformations
Roll No: 207
*/

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

POLYGON poly;
LINE lineList[20];
int lineCount = 0;

POINT begPoint, endPoint;

int polyFlag = 0;
void render();

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
	gluOrtho2D(-400, 400, -250, 250);
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

void drawAxis()
{
	glColor3f(BLACK.red, BLACK.green, BLACK.blue);
	glBegin(GL_LINES);
		glVertex2i(-400, 0);
		glVertex2i(400, 0);

		glVertex2i(0, -250);
		glVertex2i(0, 250);		
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

void matMul(float tmat[3][3], int v[])
{
	int tmp[3]  = {0};
	for( int i = 0; i < 2; i++ )
	{
		tmp[i] = 0;
		for(int j = 0; j < 3; j++ )
		{
			tmp[i] += tmat[i][j] * (float)v[j];
		}
	}
	v[0] = (int)tmp[0];
	v[1] = (int)tmp[1];
}

void translate(POLYGON *poly, int tx, int ty)
{
	cout << "\nTranslate : ";
	printVertices(*poly);
	float tmat[3][3] = {{1,		0,	(float)tx},
						{0,		1,	(float)ty},
						{0,		0,		1	 } };
	int v[3] = {0, 0, 1};
	for( int i = 0; i < poly->vertexCount; i++ )
	{
		v[0] = poly->vertex[i].x;
		v[1] = poly->vertex[i].y;
		matMul(tmat, v);
		poly->vertex[i].x = v[0];
		poly->vertex[i].y = v[1];
	}
	cout << "Translate done";
	printVertices(*poly);
	render();
}                                                                                                                                                       

void rotate(POLYGON *poly, int xCenter, int yCenter, float angle)
{
	cout << "\nRotate : ";
	if(xCenter != 0 || yCenter != 0)
	{
		translate(poly, -xCenter, -yCenter);
		sleep(1);
	}
	float rmat[3][3] = {{cos(angle),	-1*sin(angle),		0},
					{sin(angle),	cos(angle),		0},
					{0,				0,			1}};
	int v[3] = {0, 0, 1};
	for( int i = 0; i < poly->vertexCount; i++ )
	{
		v[0] = poly->vertex[i].x;
		v[1] = poly->vertex[i].y;
		matMul(rmat, v);
		poly->vertex[i].x = v[0];
		poly->vertex[i].y = v[1];
	}
	render();
	if( xCenter != 0 || yCenter != 0 )
	{
		sleep(1);
		translate(poly, xCenter, yCenter);
	}
}

void scale(POLYGON *poly, float sx, float sy)
{
	printVertices(*poly);
	float smat[3][3] = {{sx,	0,		0},
						{0,		sy,		0},
						{0,		0,		1} };
	int v[3] = {0, 0, 1};
	for( int i = 0; i < poly->vertexCount; i++ )
	{
		v[0] = poly->vertex[i].x;
		v[1] = poly->vertex[i].y;
		matMul(smat, v);
		poly->vertex[i].x = v[0];
		poly->vertex[i].y = v[1];
	}
	printVertices(*poly);
	render();
}

void shear(POLYGON *poly, float shx, float shy)
{
	printVertices(*poly);
	float shmat[3][3] = {{1,		shx,		0},
					 {shy,	1,		0},
					 {0,		0,		1}};
	int v[3] = {0, 0, 1};
	for( int i = 0; i < poly->vertexCount; i++ )
	{
		v[0] = poly->vertex[i].x;
		v[1] = poly->vertex[i].y;
		matMul(shmat, v);
		poly->vertex[i].x = v[0];
		poly->vertex[i].y = v[1];
	}
	printVertices(*poly);
	render();
}

void render()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawPolygon(poly);
	drawAxis();
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
			selectPoint(x - 400, 250-y);
		}
	}
}

void trans()
{
	char op = 'y';
	do
	{
		cout << "(t): Translate (r):Rotate (s):Scale (h):Shear (f):Finish" << endl;
		cout << "Option : ";
		cin >> op;
		switch(op)
		{
			case 't':
				int tx, ty;
				cout << "Enter the translation vectors(tx, ty) : ";
				cin >> tx >> ty; 
				translate(&poly, tx, ty);
				break;
			case 'r':
				int angle, xCenter, yCenter;
				cout << "Enter the axis of rotation (x,y) : ";
				cin >> xCenter >> yCenter;
				cout << "Enter the rotation angle : ";
				cin >> angle;
				rotate(&poly, xCenter, yCenter, angle);
				break;
			case 's':
				float sx, sy;
				cout << "Enter the scaling factors (sx, sy) : ";
				cin >> sx >> sy;
				scale(&poly, sx, sy);
				break;
			case 'h':
				float shx, shy;
				cout << "Enter the shear values (x, y) : ";
				cin >> shx >> shy;
				shear(&poly, shx, shy);
				break;
			case 'f': break;
			default : cout << "\nPlease enter a valid option \n";
						fflush(stdin); 
						break;
		}
	}while(op != 'f' );
}

void keyboardPress(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'd': polyFlag = 1; render(); break;
		case 'r': reset(); break;
		case 't': trans(); break;
		case 'q': exit(0); break;
		default : cout << "Invalid Key" << endl; break;
	}
}

int main(int argc, char **argv)
{
	initializeOpenGL(argc, argv);
	glutDisplayFunc(render);
	glutMouseFunc(mouseClick);
	glutKeyboardFunc(keyboardPress);
	poly.vertexCount = 0;
	glutMainLoop();
	return 0;
}	

//----------------END OF PROGRAM------------------//
