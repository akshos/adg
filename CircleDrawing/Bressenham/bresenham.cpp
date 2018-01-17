#include <GL/glut.h>
#include <iostream>


#define RADIUS 50

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

void drawCircle(int xCenter, int yCenter, int radius)
{
    int x = 0, y = radius;
    int d = 3 - 2 * radius;
    while (y >= x)
    {
        circlePlotPoints( xCenter, yCenter, x, y );
        x++;
 
        if (d > 0)
        {
            y--; 
            d = d + 4 * (x - y) + 10;
        }
        else
            d = d + 4 * x + 6;
        //circlePlotPoints( xCenter, yCenter, x, y );
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
