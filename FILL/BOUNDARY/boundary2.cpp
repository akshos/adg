/*
AUTHOR : Akshay Venugopal
PROGRAM : Boundary Fill
Roll No: 207
*/

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

#define HORIZONTAL 1
#define VERTICAL 2

using namespace std;

struct COLOR
{
	float red, green, blue;
};

const COLOR WHITE = {0.95, 0.95, 0.95};
const COLOR BLACK = {0.0, 0.0, 0.0};
const COLOR RED   = {0.8, 0.0, 0.0};
const COLOR GREEN = {0.0, 0.7, 0.0};
const COLOR BLUE  = {0.0, 0.0, 1.0};
const COLOR GRAY  = {0.6, 0.6, 0.6};

struct POINT
{
	int x, y;
};

struct LINE
{
	POINT beg, end;
};

struct POLYGON
{
	POINT vertex[50];
	int vertexCount;
};

POLYGON poly;

int fillx = 0, filly = 0;
int pad = 1;
void render();

int operator!=(unsigned char color_array[], COLOR &color)
{
	if( ((float)color_array[0]/255.0) == color.red )
		if( ((float)color_array[1]/255.0) == color.green )
			if( ((float)color_array[2]/255.0) == color.blue )
				return 0;
	return 1;
}

int operator!=(float color_array[], COLOR color)
{
	if( color_array[0] == color.red )
		if( color_array[1] == color.green )
			if( color_array[2] == color.blue )
				return 0;
	return 1;
}

int operator==(float color_array[], COLOR color)
{
	if( color_array[0] == color.red )
		if( color_array[1] == color.green )
			if( color_array[2] == color.blue )
				return 1;
	return 0;
}

void drawPolygon(POLYGON poly)
{
	cout << "drawing polygon " << poly.vertexCount <<  endl;
	glColor3f(BLACK.red, BLACK.green, BLACK.blue);
	for(int i = 0; i < poly.vertexCount - pad; i++)
	{
		glBegin(GL_LINES);
			glVertex2i(poly.vertex[i].x, poly.vertex[i].y);
			glVertex2i(poly.vertex[(i+1)%poly.vertexCount].x, poly.vertex[(i+1)%poly.vertexCount].y);
		glEnd();
	}
}

queue<POINT> pixels;
float prevColor[3];

void addScanLine(int x, int y){
    POINT tmp;
    glReadPixels(x-1 , y , 1 , 1 , GL_RGB , GL_FLOAT , prevColor);
    while(x > 0 && prevColor !=  BLACK){
        x = x-1;
        glReadPixels(x-1 , y , 1 , 1 , GL_RGB , GL_FLOAT , prevColor);
    }
    if(x > 0){
        tmp.x = x; tmp.y = y;
        pixels.push(tmp);
    }
}

void boundaryFill(int x, int y, COLOR fillColor){
    POINT curr, tmp;
    int tflag, bflag;
    addScanLine(x, y);
    glColor3f(fillColor.red, fillColor.green, fillColor.blue);
    while( !pixels.empty() ){
        tflag = bflag = 0;
        curr = pixels.front();
        pixels.pop();
        x = curr.x; y = curr.y;
        glReadPixels(x, y , 1 , 1 , GL_RGB , GL_FLOAT , prevColor);
        while( prevColor != BLACK && x < 800 ){
            glBegin(GL_POINTS);
                glVertex2i(x, y);
            glEnd();
            //top pixel
            glReadPixels(x, y+1 , 1 , 1 , GL_RGB , GL_FLOAT , prevColor);
            if(tflag == 0 && prevColor != BLACK && prevColor != fillColor){
                addScanLine(x, y+1);
                tflag = 1;
            }
            else if(tflag == 1 && prevColor == BLACK)
                tflag = 0;
            //bottom pixel
            glReadPixels(x, y-1 , 1 , 1 , GL_RGB , GL_FLOAT , prevColor);
            if(bflag == 0 && prevColor != BLACK && prevColor != fillColor){
                addScanLine(x, y-1);
                bflag = 1;
            }
            else if(bflag == 1 && prevColor == BLACK)
                bflag = 0;
            ++x;
            glReadPixels(x, y , 1 , 1 , GL_RGB , GL_FLOAT , prevColor);
            glFlush();
        }
    }
}

void fill()
{
	if( fillx != 0 )
	{
		cout << "Filling polygon from " << fillx << "  " << filly << endl;
		boundaryFill(fillx, filly, BLUE);
        cout << "Filled" << endl;
	}
} 

POINT begPoint, endPoint;

int polyFlag = 0;

void render()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawPolygon(poly);
	glFlush();
	fill();
	glFlush();
}

void reset()
{
	poly.vertexCount = 0;
	polyFlag = 0;
	fillx = 0;
	filly = 0;
	pad = 1;
	render();
}

void selectPoint(int x, int y)
{
	cout << "selecting point " << poly.vertexCount+1 << " : " << x << " , " << y << endl;
	poly.vertex[poly.vertexCount].x = x;
	poly.vertex[poly.vertexCount].y = y;
	poly.vertexCount++;
}

void mouseClick(int button, int state, int x, int y)
{
	if( button == GLUT_LEFT_BUTTON )
	{
		if( state == GLUT_DOWN )
		{
			if( polyFlag == 0 )
			{
				selectPoint(x, 500-y);
				render();
			}
			else
			{
				cout << "setting fill coordinates.." << endl;
				fillx = x;
				filly = 500-y;
				render();
			}
		}
		
	}
}

void keyboardPress(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'd': polyFlag = 1; pad = 0; render(); break;
		case 'r': reset(); break;
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
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 500);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glutDisplayFunc(render);
	glutMouseFunc(mouseClick);
	glutKeyboardFunc(keyboardPress);
	poly.vertexCount = 0;
	glutMainLoop();
	return 0;
}	


//--------------------------END OF PROGRAM------------------------//


