#include <GL/glut.h>
#include <iostream>


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

void drawCircle( int xCenter, int yCenter, int radius )
{
	int x = 0, y = radius, p = 1 - radius;
	circlePlotPoints( xCenter, yCenter, x, y );
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
	}
}

void mouseClick( int button, int state, int x, int y)
{
	if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
	{
		drawCircle(x, 500-y, RADIUS);
		glFlush();
	}
}

void display()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

int main( int argc, char **argv )
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
	
	glutDisplayFunc(display);
	glutMouseFunc(mouseClick);
	glutMainLoop();
	return 0;
}




