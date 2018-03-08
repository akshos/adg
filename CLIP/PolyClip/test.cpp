#include "primitives.h"

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
	initializeOpenGL(argc, argv);
	glutDisplayFunc(render);
	glutMouseFunc(mouseClick);
	glutKeyboardFunc(keyboardPress);
	glutMainLoop();
	glutIdleFunc(render);
	return 0;
}	
	

