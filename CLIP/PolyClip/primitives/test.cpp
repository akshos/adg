#include "primitives.h"

POINT begPoint, endPoint;

int polyFlag = 0;

void render()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawPolygon(poly);
	drawInnerWindow();
	glFlush();
}

void reset()
{
	poly.vertexCount = 0;
	polyFlag = 0;
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
		if( state == GLUT_DOWN && polyFlag == 0)
		{
			selectPoint(x, 500-y);
		}
	}
}

void keyboardPress(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'd': polyFlag = 1; render(); break;
		case 'c': polyClip(&poly); break;
		case 'r': reset(); break;
		default : cout << "Invalid Key" << endl;
	}
}

int main(int argc, char **argv)
{
	initializeOpenGL(argc, argv);
	glutDisplayFunc(render);
	glutMouseFunc(mouseClick);
	glutKeyboardFunc(keyboardPress);
	poly.vertexCount = 0;
	glutMainLoop();
	return 0;
}	
	

