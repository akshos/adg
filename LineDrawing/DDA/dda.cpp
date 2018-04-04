/*
Author : Akshay Venugopal
Program : DDA Line Drawing
Roll No : 207
*/ 


#include <GL/glut.h>
#include <iostream>
#include <cmath>

#define ROUND(x) ((int)(x+0.5))


using namespace std;


void put_point(int x, int y)
{
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
		glVertex2i(x,y);
	glEnd();
}

void drawLine( int x1, int y1, int x2, int y2 )
{
	int dx = x2-x1, dy = y2 - y1, steps, k;
	float xIncrement, yIncrement, x, y;
	x = (float)x1;
	y = (float)y1;
	if( abs(dx) > abs(dy) )
		steps = abs(dx);
	else
		steps = abs(dy);
	xIncrement = dx/(float)steps;
	yIncrement = dy/(float)steps;
	
	put_point( ROUND(x), ROUND(y) );
	
	for( k = 0; k < steps; k++ )
	{
		x += xIncrement;
		y += yIncrement;
		put_point( ROUND(x), ROUND(y) );
	}
	cout << "\nDDA End Point : " << ROUND(x) << ", " << ROUND(y) << endl;
}

void mouseClick( int button, int state, int x, int y)
{
	static int flag = 0, x1, y1, x2, y2;
	if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
	{
		if( flag == 0 )
		{
			x1 = x; y1 = 500 - y;
			flag = 1;
			cout << "\nStart Point : " << x1 << ", " << y1 << endl; 
		}
		else if( flag == 1 )
		{
			x2 = x; y2 = 500 - y;
			cout << "\nEnd Point : " << x2 << ", " << y2 << endl;
			drawLine(x1, y1, x2, y2);
			flag = 0;
			glFlush();
		}
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

