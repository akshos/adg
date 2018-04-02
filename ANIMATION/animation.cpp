#include <GL/glut.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cmath>

#define XMAX 800
#define YMAX 500

#define STOP 0
#define SLOW 1
#define GO   2

using namespace std;

struct COLOR
{
	float red, green, blue;
};

const COLOR WHITE = {0.95, 0.95, 0.95};
const COLOR BLACK = {0.0, 0.0, 0.0};
const COLOR RED = {1.0, 0.0, 0.0};
const COLOR GREEN = {0.0, 1.0, 0.0};
const COLOR ORANGE = {0.8, 0.5, 0.1};
const COLOR GREY = {0.9, 0.9, 0.9};

COLOR offColor = GREY;
COLOR lightGreen = GREEN;
COLOR lightRed = offColor;
COLOR lightOrange = offColor;



float xvelocity, yvelocity;
float velocity, slowVelocity;
int lightStatus;

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

void drawRoad()
{
	glColor3f(0.3, 0.3, 0.3);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_QUADS); //draw the road
		glVertex2i(0, 200);
		glVertex2i(0, 300);
		glVertex2i(800, 300);
		glVertex2i(800, 200);
	glEnd();
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_QUADS); //draw the traffic light pole
		glVertex2i(600, 300);
		glVertex2i(600, 400);
		glVertex2i(610, 400);
		glVertex2i(610, 300);
	glEnd();
	glBegin(GL_QUADS); //draw traffic light housing
		glVertex2i(580, 400);
		glVertex2i(580, 490);
		glVertex2i(630, 490);
		glVertex2i(630, 400);
	glEnd();
	drawCircle(605, 420, 10, lightRed);
	drawCircle(605, 444, 10, lightOrange);
	drawCircle(605, 468, 10, lightGreen);
	glColor3f(0.7, 0.7, 0.7);
	glBegin(GL_QUADS); //draw stripe on road
		glVertex2i(600, 205);
		glVertex2i(600, 295);
		glVertex2i(603, 295);
		glVertex2i(603, 205);
	glEnd();
	glPopMatrix();
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
	glMatrixMode(GL_MODELVIEW);
	drawRoad();
	glTranslatef(0, YMAX/2, 0);
	drawCar();
	glFlush();
}

float coordinates[16];
long delay = 10000;

void moveCar()
{
	glGetFloatv(GL_MODELVIEW_MATRIX, coordinates);
	int xpos = coordinates[12];
	if( lightStatus == GO )
		xvelocity = velocity;
	else if(lightStatus == SLOW)
	{
		if(xpos < 600) //600 is where the traffic light is positioned
			xvelocity = slowVelocity;
		else
			xvelocity = velocity;
	}
	else if(lightStatus == STOP)
	{
		if(xpos <= 520)
			xvelocity = velocity * ((float)(590-xpos)/590);
		if(xpos > 520 && xpos <600)
			xvelocity = 0;
	}
	drawCar();
	if(xpos  > 800)
		glTranslatef(-880.0, 0.0, 0.0);
	if(coordinates[13] > 520)
		glTranslatef(0.0, -580.0, 0.0);
}

void animate()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	drawRoad();
	moveCar();
	glFlush();
	usleep(delay);
}

void lightStop()
{
	lightGreen = offColor;
	lightOrange = offColor;
	lightRed = RED;
	lightStatus = STOP;
}

void lightSlow()
{
	lightGreen = offColor;
	lightOrange = ORANGE;
	lightRed = offColor;
	lightStatus = SLOW;
}

void lightGo()
{
	lightGreen = GREEN;
	lightOrange = offColor;
	lightRed = offColor;
	lightStatus = GO;
}

void keyboardPress(unsigned char key, int x, int y)
{
/*	if(key == 'w')
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
*/
	if(key == 'q')
		exit(0);
	if(key == 'l')
		delay -= 10;
	if(key == 'p')
		delay += 10;
	if(key == 'z')
		lightStop();
	if(key == 'x')
		lightSlow();
	if(key == 'c')
		lightGo();
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
	slowVelocity = 2;
	xvelocity = yvelocity = 0;
	lightStatus = GO;

	glutDisplayFunc(display);
	glutIdleFunc(animate);
	glutKeyboardFunc(keyboardPress);
	glutMainLoop();
	return 0;
}
