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
	gluOrtho2D(0, 800, 0, 500);
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

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
	cout << "\nRight Clipper" << endl;
	clipper(poly, XW_MAX, -1, HORIZONTAL);
	printVertices(*poly);
	cout << "\nBottom Clipper" << endl;
	clipper(poly, YW_MIN, 1, VERTICAL);
	printVertices(*poly);
	cout << "\nTop Clipper" << endl;
	clipper(poly, YW_MAX, -1, VERTICAL);
	printVertices(*poly);
	render();
}

