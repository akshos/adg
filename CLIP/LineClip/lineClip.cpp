#include <GL/glut.h>
#include <iostream>
#include <queue>
#include <cmath>
#include <unistd.h>
#include <stdio.h>

#define RADIUS 30
#define EDGE_WIDTH 2

#define XW_MIN 170
#define XW_MAX 600
#define YW_MIN 120
#define YW_MAX 400

#define INSIDE 0x00
#define LEFT 0x01
#define RIGHT 0x02
#define BOTTOM 0x04
#define TOP 0x08


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

struct POINT
{
	int x, y;
};

struct LINE
{
	POINT beg, end;
};
LINE lineList[20];
int lineCount = 0;

void render();

void drawLine( POINT a, POINT b, COLOR color)
{
	glColor3f(color.red, color.green, color.blue);
	glLineWidth(1);
	glBegin(GL_LINES);
		glVertex2i(a.x, a.y);
		glVertex2i(b.x, b.y);
	glEnd();
}

void drawLine( int x1, int y1, int x2, int y2, COLOR color)
{
	glColor3f(color.red, color.green, color.blue);
	glLineWidth(1);
	glBegin(GL_LINES);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
	glEnd();
}

void addLine(POINT beg, POINT end)
{
	lineList[lineCount].beg = beg;
	lineList[lineCount].end = end;
	lineCount++;
	render();
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

void drawAllLines()
{
	LINE *line = lineList;
	for( int i = 0; i < lineCount; i++, line++ )
	{
		drawLine(line->beg, line->end, BLACK);
	}
}

void render()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawInnerWindow();
	drawAllLines();
	glFlush();
}

int getMask(POINT pnt)
{
	int mask = 0;
	mask |= (pnt.x<XW_MIN) ? LEFT : INSIDE;
	mask |= (pnt.x>XW_MAX) ? RIGHT : INSIDE;
	mask |= (pnt.y<YW_MIN) ? BOTTOM : INSIDE;
	mask |= (pnt.y>YW_MAX) ? TOP : INSIDE;
	return mask;
}

void clipLine(POINT *p1, POINT *p2, int mask)
{
	if( (mask&LEFT) ) 
	{
		p1->y = p1->y + (p2->y - p1->y)*(float)(XW_MIN - p1->x)/(float)(p2->x - p1->x);
		p1->x = XW_MIN;
	}else if( (mask&RIGHT) )
	{
		p1->y = p1->y + (p2->y - p1->y)*(float)(XW_MAX - p1->x)/(float)(p2->x - p1->x);
		p1->x = XW_MAX;
	}else if( (mask&TOP) )
	{
		p1->x = p1->x + (p2->x - p1->x)*(float)(YW_MAX - p1->y)/(float)(p2->y - p1->y);
		p1->y = YW_MAX;
	}else if( (mask&BOTTOM) )
	{
		p1->x = p1->x + (p2->x - p1->x)*(float)(YW_MIN - p1->y)/(float)(p2->y - p1->y);
		p1->y = YW_MIN;
	}
}

void cohenSutherland(LINE *line)
{
	POINT beg = line->beg;
	POINT end = line->end;
	int begMask = getMask(beg);
	int endMask = getMask(end);

	while(begMask != 0 || endMask != 0)
	{
		if( (begMask|endMask) == 0)
		{
			cout << "Line completly inside, no clip" << endl;
			break;
		}
		else if( (begMask&endMask) != 0)
		{
			cout << "Line completly outside, discard" << endl;
			beg.x = beg.y =  0;
			end.x = end.y = 0;
			break;
		}
		else
		{
			cout << "Beg Point mask : " << begMask << endl;
			cout << "End Point mask : " << endMask << endl;
			if(begMask != 0)
			{
				clipLine(&beg, &end, begMask);
				begMask = getMask(beg);
			}
			if(endMask != 0)
			{
				clipLine(&end, &beg, endMask);
				endMask = getMask(end);
			}
		}
	}
	line->beg = beg;
	line->end = end;
}

void lineClip()
{
	LINE *line = lineList;
	for( int i = 0; i < lineCount; i++, line++ )
	{
		cohenSutherland(line);
	}
	render();
}

POINT begPoint, endPoint;
int flag = 0;

void selectPoint(int x, int y)
{
	if( flag == 0 )
	{
		begPoint.x = x;
		begPoint.y = y;
		flag = (flag+1)%2;
	}
	else if( flag == 1 )
	{
		endPoint.x = x;
		endPoint.y = y;
		if(begPoint.x != endPoint.x && begPoint.y != endPoint.y)
		{
			addLine(begPoint, endPoint);
			flag = (flag+1)%2;
		}
	}
	else
	{
		cout << "ERROR : invalid flag value in selectPoint";
		exit(1);
	}
}

void mouseClick(int button, int state, int x, int y)
{
	if( button == GLUT_LEFT_BUTTON )
	{
		if( state == GLUT_DOWN )
		{
			selectPoint(x, 500-y);
		}
	}
}

void keyboardPress(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'c': lineClip(); break;
		default : cout << "Invalid Key" << endl;
	}
}


int main(int argc, char **argv)
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
	
	glutDisplayFunc(render);
	glutMouseFunc(mouseClick);
	glutKeyboardFunc(keyboardPress);
	glutMainLoop();
	glutIdleFunc(render);
	return 0;
}	
	

