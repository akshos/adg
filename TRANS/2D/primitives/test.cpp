#include "primitives.h"

POINT begPoint, endPoint;

int polyFlag = 0;

void render()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawPolygon(poly);
	drawAxis();
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
			selectPoint(x - 400, 250-y);
		}
	}
}

void trans()
{
	char op = 'y';
	do
	{
		cout << "(t): Translate (r):Rotate (s):Scale (f):Finish" << endl;
		cout << "Option : ";
		cin >> op;
		switch(op)
		{
			case 't':
				int tx, ty;
				cout << "Enter the translation vectors(tx, ty) : ";
				cin >> tx >> ty; 
				translate(&poly, tx, ty);
				break;
			case 'r':
				int angle, xCenter, yCenter;
				cout << "Enter the axis of rotation (x,y) : ";
				cin >> xCenter >> yCenter;
				cout << "Enter the rotation angle : ";
				cin >> angle;
				rotate(&poly, xCenter, yCenter, angle);
				break;
			case 's':
				float sx, sy;
				cout << "Enter the scaling factors (sx, sy) : ";
				cin >> sx >> sy;
				scale(&poly, sx, sy);
				break;
			case 'f': break;
			default : cout << "\nPlease enter a valid option \n";
						fflush(stdin); 
						break;
		}
	}while(op != 'f' );
}

void keyboardPress(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'd': polyFlag = 1; render(); break;
		case 'r': reset(); break;
		case 't': trans(); break;
		case 'q': exit(0); break;
		default : cout << "Invalid Key" << endl; break;
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
	

