#include <GL/glut.h>
#include <iostream>
#include <cstdlib>
#include <cmath>

#define XMAX 800
#define YMAX 500

using namespace std;

struct COLOR
{
	float red, green, blue;
};

const COLOR WHITE = {0.95, 0.95, 0.95};
const COLOR BLACK = {0.0, 0.0, 0.0};

float xvelocity, yvelocity;
float velocity;

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
}

void drawCar()
{
	glTranslatef(xvelocity, yvelocity, 0.0);
	glPushMatrix();
	glTranslatef(20.0, 0.0, 0.0);
	drawCircle( 0, 0, 15, BLACK );
	glTranslatef(40.0, 0.0, 0.0);
	drawCircle( 0, 0, 15, BLACK );
	glTranslatef(-60.0, 0.0, 0.0);
	glColor3f(0.0, 0.4, 0.5);
	glBegin(GL_QUADS);
		glVertex2i(0 , 0);
		glVertex2i(0 , 40);
		glVertex2i(80, 40);
		glVertex2i(80, 0);
	glEnd();
	glTranslatef(20.0, 40.0, 0.0);
	glColor3f(0.3, 0.4, 0.5);
	glBegin(GL_QUADS);
		glVertex2i(0 , 0);
		glVertex2i(0 , 20);
		glVertex2i(40, 20);
		glVertex2i(40, 0);
	glEnd();
	glPopMatrix();
}

void display()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glTranslatef(0, 100, 0);
	drawCar();
	glFlush();
}

void animate()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawCar();
	glFlush();
}

void keyboardPress(unsigned char key, int x, int y)
{
	if(key == 'w')
		yvelocity = abs(velocity);		
	if(key == 's')
		yvelocity = -1*abs(velocity);
	if(key != 'w' && key != 's' )
		yvelocity = 0;
	if(key == 'd')
		xvelocity = abs(velocity);		
	if(key == 'a')
		xvelocity = -1*abs(velocity);
	if(key != 'a' && key != 'd' )
		xvelocity = 0;
	if(key == 'q')
		exit(0);
	//cout << "xv : " << xvelocity << " yv : " << yvelocity << endl;
	animate();
}

int main( int argc, char **argv )
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(XMAX, YMAX);
	glutCreateWindow("OpenGL LAB");
	
	//order important for the below calls
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //this call should always come before gluOrtho2D
	gluOrtho2D(0, XMAX, 0, 500);
	
	velocity = 5;
	xvelocity = yvelocity = 0;
	
	glutDisplayFunc(display);
	//glutIdleFunc(animate);
	glutKeyboardFunc(keyboardPress);
	glutMainLoop();
	return 0;
}
