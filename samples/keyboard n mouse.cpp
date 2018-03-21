//#include <GL/gl.h>
//#include <GL/glu.h>

#include <math.h>
#include<iostream>
#include<cstdlib>
#include<stdio.h>
#include <fstream>
#include <gl/glut.h>


using namespace std;


float x=0,y=0,z=0;
void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (0.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(x,y,z);
	glutSolidSphere(0.2,10,10);
	glPopMatrix();
	glutSwapBuffers();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (0.0, 0.0, 3.01, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void mouse(int button, int state, int x, int y)
{  
	int l=0;
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	GLint realy; /* OpenGL y coordinate position */
	GLdouble wx, wy, wz; /* returned world x, y, z coords             */
	switch (button) 
	{
		case GLUT_LEFT_BUTTON:
    			    if (state == GLUT_DOWN) 
    			    {
						glGetIntegerv(GL_VIEWPORT, viewport);
						glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
						glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
						realy = viewport[3] - (GLint) y - 1;
						gluUnProject((GLdouble) x, (GLdouble) realy, 0.0,
						mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
					}
					std::cout<<" you have pressed Left button"<<wx<<"\n";
					x=x+0.01;y=wy;
					glutPostRedisplay();
					break;

		case GLUT_RIGHT_BUTTON:
					if (state == GLUT_DOWN)
					{
						glGetIntegerv(GL_VIEWPORT, viewport);
						glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
						glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
						/* note viewport[3] is height of window in pixels */
						realy = viewport[3] - (GLint) y - 1;
						gluUnProject((GLdouble) x, (GLdouble) realy, 0.0,
						mvmatrix, projmatrix, viewport, &wx, &wy, &wz);            	      
					} 
					glutPostRedisplay();
					break;             
		default:
		break;
	}
}

//keyboard function
void keyboard (unsigned char key, int p, int q)
{      
	switch (key) 
	{
		case 'l':
		case 'L':	std::cout<<"you have pressed the key l";			         
				glutPostRedisplay();         
				break;  		
		case 27:
				exit(0);
				break;

		case 's':
		case 'S':
				
				break;  
		default:
				break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (800, 800); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);	
	glutMainLoop();
	return 0;
}
