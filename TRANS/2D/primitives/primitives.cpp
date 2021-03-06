#include "primitives.h"

/*
*NOTE : the exact ordering of the function calls are important
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
		//sleep(1);
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
		//sleep(1);
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


