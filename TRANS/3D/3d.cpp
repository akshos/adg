/*
AUTHOR : Akshay Venugopal
PROGRAM : 3D Transformations
Roll No: 207
*/

#include <GL/glut.h>
#include <iostream>
#include <queue>
#include <cmath>
#include <unistd.h>
#include <cstdio>

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
	float x, y, z;
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

POINT begPoint, endPoint;

int polyFlag = 0;
void render();

void initializeOpenGL(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(600, 600);
	glutCreateWindow("OpenGL LAB");
	
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
}

void drawAxis()
{
	glColor3f(BLACK.red, BLACK.green, BLACK.blue);
	glBegin(GL_LINES);
		glVertex3f(-1, 0, 0);
		glVertex3f(1, 0, 0);

		glVertex3f(0, -1, 0);
		glVertex3f(0, 1, 0);	

		glVertex3f(0, 0, -1);
		glVertex3f(0, 0.3, 1);	
	glEnd();
}

void drawPolygon()
{
	cout << "drawing polygon" << endl;
	glColor3f(0, 0, 0);
	int n, x, y;
	glPointSize(5);
	//glBegin(GL_POINTS);
	//	glVertex3f(0, 0.2, 0);
	//glEnd();
	glFlush();
	glBegin(GL_LINES);
		for( int i = 0; i < 4; i ++ )
		{
			n = (i+1)%4;
			x = i + 4;
			y = n + 4;
			//front face
			glVertex3f(poly.vertex[i].x, poly.vertex[i].y, poly.vertex[i].z);
			glVertex3f(poly.vertex[n].x, poly.vertex[n].y, poly.vertex[n].z);
			//back face
			glVertex3f(poly.vertex[x].x, poly.vertex[x].y, poly.vertex[x].z);
			glVertex3f(poly.vertex[y].x, poly.vertex[y].y, poly.vertex[y].z);
			//side edges
			glVertex3f(poly.vertex[i].x, poly.vertex[i].y, poly.vertex[i].z);
			glVertex3f(poly.vertex[x].x, poly.vertex[x].y, poly.vertex[x].z);
		}
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

void matMul(float tmat[4][4], float v[])
{
	float tmp[4]  = {0};
	for( int i = 0; i < 4; i++ )
	{
		tmp[i] = 0;
		for(int j = 0; j < 4; j++ )
		{
			tmp[i] += tmat[i][j] * v[j];
		}
	}
	v[0] = tmp[0];
	v[1] = tmp[1];
	v[2] = tmp[2];
}

void translate(POLYGON *poly, float tx, float ty, float tz)
{
	cout << "\nTranslate : ";
	printVertices(*poly);
	float tmat[4][4] = {{1,		0,	0,	tx},
						{0,		1,	0,	ty},
						{0,		0,	1,	tz},
						{0,		0,	0,		1	 } };
	float v[4] = {0, 0, 0, 1};
	for( int i = 0; i < poly->vertexCount; i++ )
	{
		v[0] = poly->vertex[i].x;
		v[1] = poly->vertex[i].y;
		v[2] = poly->vertex[i].z;
		matMul(tmat, v);
		poly->vertex[i].x = v[0];
		poly->vertex[i].y = v[1];
		poly->vertex[i].z = v[2];
	}
	cout << "Translate done";
	printVertices(*poly);
	render();
}                                                                                                                                                       

void rotate(POLYGON *poly, float xCenter, float yCenter, float zCenter, float angle, char axis)
{
	cout << "\nRotate : ";
	if(xCenter != 0 || yCenter != 0)
	{
		translate(poly, -xCenter, -yCenter, -zCenter);
		//sleep(1);
	}
	float rmat[4][4] = {{0, 0, 0, 0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}};
	switch (axis)
	{
		case 'z' :
			rmat[0][0] = cos(angle);
			rmat[0][1] = -1*sin(angle);
			rmat[1][0] = sin(angle);
			rmat[1][1] = cos(angle);
			rmat[2][2] = 1;
			rmat[3][3] = 1;
			break;
		case 'y':
			rmat[0][0] = cos(angle);
			rmat[0][2] = sin(angle);
			rmat[1][1] = 1;
			rmat[2][0] = -1*sin(angle);
			rmat[2][2] = cos(angle);
			rmat[3][3] = 1;
			break;
		case 'x' :
			rmat[0][0] = 1;
			rmat[1][1] = cos(angle);
			rmat[1][2] = -1*sin(angle);
			rmat[2][1] = sin(angle);
			rmat[2][2] = cos(angle);
			rmat[3][3] = 1;
			break;
	} 
	float v[4] = {0, 0, 0, 1};
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
		//sleep(1);
		translate(poly, xCenter, yCenter, zCenter);
	}
}

void scale(POLYGON *poly, float sx, float sy, float sz)
{
	printVertices(*poly);
	float smat[4][4] = {{sx,	0,		0, 	0},
						{0,		sy,		0, 	0},
						{0,		0,		sz,	0},
						{0,		0,		0, 	1} };
	float v[4] = {0, 0, 0, 1};
	for( int i = 0; i < poly->vertexCount; i++ )
	{
		v[0] = poly->vertex[i].x;
		v[1] = poly->vertex[i].y;
		v[2] = poly->vertex[i].z;
		matMul(smat, v);
		poly->vertex[i].x = v[0];
		poly->vertex[i].y = v[1];
		poly->vertex[i].z = v[2];
	}
	printVertices(*poly);
	render();
}

void shear(POLYGON *poly, float shx, float shy, float shz)
{
	printVertices(*poly);
	float shmat[4][4] = {{1,		shy,	shz,	0},
					 	{shx,		1,		shz,	0},
					 	{shx,		shy,	1,		0},
						{0,			0,		0,		1}};
	float v[4] = {0, 0, 1};
	for( int i = 0; i < poly->vertexCount; i++ )
	{
		v[0] = poly->vertex[i].x;
		v[1] = poly->vertex[i].y;
		v[2] = poly->vertex[i].z;
		matMul(shmat, v);
		poly->vertex[i].x = v[0];
		poly->vertex[i].y = v[1];
		poly->vertex[i].y = v[2];
	}
	printVertices(*poly);
	render();
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	
	glLoadIdentity(); //this call should always come before gluOrtho2D
	glClearColor(1.0, 1.0, 1.0, 0.0);
	drawPolygon();
	drawAxis();
	glFlush();
	
}

void reset()
{
	poly.vertexCount = 0;
	polyFlag = 0;
	render();
}

void mouseClick(int button, int state, int x, int y)
{
	if( button == GLUT_LEFT_BUTTON )
	{
		cout << "Mouse click " << endl;
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
				float tx, ty, tz;
				cout << "Enter the translation vectors(tx, ty, tz) : ";
				cin >> tx >> ty >> tz; 
				translate(&poly, tx, ty, tz);
				break;
			case 'r':
				float angle, xCenter, yCenter, zCenter;
				char axis;
				cout << "Enter the axis of rotation (x/y/z) : ";
				cin >> axis;
				if(axis != 'x' && axis != 'y' && axis != 'z')
				{
					cout << "Wrong axis for rotation" << endl;
					continue;
				}
				cout << "Enter the coordinate of rotation (x, y, z) : ";
				cin >> xCenter >> yCenter >> zCenter;
				cout << "Enter the rotation angle : ";
				cin >> angle;
				rotate(&poly, xCenter, yCenter, zCenter, angle, axis);
				break;
			case 's':
				float sx, sy, sz;
				cout << "Enter the scaling factors (sx, sy, sz) : ";
				cin >> sx >> sy >> sz;
				scale(&poly, sx, sy, sz);
				break;
			case 'h':
				float shx, shy, shz;
				cout << "Enter the shear values (x, y, z) : ";
				cin >> shx >> shy >> shz;
				shear(&poly, shx, shy, shz);
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

void createCube()
{
	poly.vertexCount = 8;
	
	poly.vertex[0].x = 0;
	poly.vertex[0].y = 0;
	poly.vertex[0].z = 0;

	poly.vertex[1].x = 0;
	poly.vertex[1].y = 0.4;
	poly.vertex[1].z = 0;

	poly.vertex[2].x = 0.4;
	poly.vertex[2].y = 0.4;
	poly.vertex[2].z = 0;

	poly.vertex[3].x = 0.4;
	poly.vertex[3].y = 0;
	poly.vertex[3].z = 0;

	poly.vertex[4].x = 0;
	poly.vertex[4].y = 0;
	poly.vertex[4].z = -0.4;

	poly.vertex[5].x = 0;
	poly.vertex[5].y = 0.4;
	poly.vertex[5].z = -1;

	poly.vertex[6].x = 0.4;
	poly.vertex[6].y = 0.4;
	poly.vertex[6].z = -1;

	poly.vertex[7].x = 0.4;
	poly.vertex[7].y = 0;
	poly.vertex[7].z = -1;
}

int main(int argc, char **argv)
{
	initializeOpenGL(argc, argv);
	glutDisplayFunc(render);
	glutMouseFunc(mouseClick);
	glutKeyboardFunc(keyboardPress);
	poly.vertexCount = 0;
	createCube();
	glutMainLoop();
	return 0;
}	

//----------------END OF PROGRAM------------------//
