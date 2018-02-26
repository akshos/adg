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

void drawLine( POINT a, POINT b, COLOR color, int width)
{
	glColor3f(color.red, color.green, color.blue);
	glLineWidth(width);
	glBegin(GL_LINES);
		glVertex2i(a.x, a.y);
		glVertex2i(b.x, b.y);
	glEnd();
}

void drawLine( int x1, int y1, int x2, int y2, COLOR color, int width)
{
	glColor3f(color.red, color.green, color.blue);
	glLineWidth(width);
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

void lineClip()
{
	LINE *line = lineList;
	for( int i = 0; i < lineCount; i++, line++ )
	{
		cohenSutherland(line);
	}
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

void cohenSutherland(LINE *line)
{
	POINT beg = line->beg;
	POINT end = line->end;
	int begMask = getMask(beg);
	int endMask = getMask(end);
	cout << "Beg Point mask : " << begMask << endl;
	cout << "End Point mask : " << endMask << endl;
}