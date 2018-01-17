#include <GL/glut.h>
#include <iostream>


#define RADIUS 30

using namespace std;

void put_point(int x, int y)
{
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
		glVertex2i(x,y);
	glEnd();
}

void draw_vertical( int x0, int y0, int xn, int yn)
{
	int x=x0, y=y0, ycount=1, yend=yn;
	if(y0>yn)
		ycount=-1;
	put_point(x, y);
	while(y!= yend)
	{
		y=y+ycount;
		put_point(x, y);
	}
}

void draw_horizontal( int x0, int y0, int xn, int yn)
{
	int x=x0,y=y0,xcount=1,xend=xn;
	if(x0>xn)
		xcount=-1;
	put_point(x, y);
	while( x != xend)
	{
		x=x+xcount;
		put_point(x, y);
	}
}

void draw_diagonal( int x0,int y0,int xn,int yn)
{
	int x=x0,y=y0,ycount=1,xcount=1,xend=xn;
	if(x0>xn)
		xcount=-1;
	if(y0>yn)
		ycount=-1;
	put_point(x, y);
	while( x!= xend)
	{
		y=y+ycount;
		x=x+xcount;
		put_point(x, y);
	}
}

void draw_less( int x0, int y0, int xn, int yn, float m)
{
	int dx,dy,x,y,p,xend,xcount=1,ycount=1;
	dx=abs(xn-x0);
	dy=abs(yn-y0);
	p=2*dy - dx;
	if(x0>xn)
	{
		x=xn;
		y=yn;
		xend=x0;
	}
	else
	{
		x=x0;
		y=y0;
		xend=xn;
	}
	if( m < 0 )
		ycount = -1;
	put_point(x, y);
	while( x != xend)
	{
		x=x+xcount;
		if(p < 0)
			p=p+2*dy;
		else
		{
			y=y+ycount;
			p=p+2*(dy-dx);
		}
		put_point(x, y);
	}
}

void draw_great( int x0, int y0, int xn, int yn, float m)
{
	int dx,dy,x,y,p,yend,xcount=1,ycount=1;
	dx=abs(xn-x0);
	dy=abs(yn-y0);
	p=2*dx - dy;
	if(y0>yn)
	{
		x=xn;
		y=yn;
		yend=y0;
	}
	else
	{
		x=x0;
		y=y0;
		yend=yn;
	}
	if( m < 0 )
		xcount = -1;
	put_point(x, y);
	while( y != yend)
	{
		y=y+ycount;
		if(p < 0)
			p=p+2*dx;
		else
		{
			x=x+xcount;
			p=p+2*(dx-dy);
		}
		put_point(x, y);
	}
}

void drawLine( int x1, int y1, int x2, int y2 )
{
	float dx,dy,m;
	dx=x2-x1;
	dy=y2-y1;
	m=dy/dx;
	if (dx == 0)
		draw_vertical(x1,y1,x2,y2);
	if (dy == 0)
		draw_horizontal(x1,y1,x2,y2);
	else if( abs(m) > 0 && abs(m) < 1 )
		draw_less(x1,y1,x2,y2,m);
	else if ( abs(m) > 1)
		draw_great(x1,y1,x2,y2,m);
	else
		draw_diagonal(x1,y1,x2,y2);	
}

int flag = 0;
int x1, y1, x2, y2;

void mouseClick( int button, int state, int x, int y)
{
	if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
	{
		if( flag == 0 )
		{
			x1 = x; y1 = 500 - y;
			flag = 1;
		}
		else if( flag == 1 )
		{
			x2 = x; y2 = 500 - y;
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
