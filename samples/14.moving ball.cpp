//#include <GL/gl.h>
//#include <GL/glu.h>
#include <windows.h>
#include<iostream>
#include<math.h>
#include <GL/glut.h>


static double x=0,y=0,sign=1,theta=0;

void init(void) 
{
glClearColor (0.0, 0.0, 0.0, 0.0);
glShadeModel(GL_SMOOTH);
glEnable(GL_DEPTH_TEST);
}
void calc(void) 
{
	//Sleep(1);
	//x=x+0.001;
    y=y+sign*0.01;
	theta=theta+0.1;
	if (y>9)
	{
	sign=-1;
	}
	if (y<0)
	{
		sign=1;
	}
	
}
void set_value(void)
{
	 
}
void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   	glClear (GL_COLOR_BUFFER_BIT);   	
   	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();
	init();
 	
	glShadeModel(GL_SMOOTH);	
   	gluLookAt (0,0, 40,0, 0, 0, 0, 1, 0);
 
	glPushMatrix();	
	glColor3f (0.0, 1.0, 1.0);
	glTranslatef(x,y,0);
	glutSolidSphere(3,30,30);
	glPopMatrix();


	glPushMatrix();	
	glColor3f (0.0, 1.0, 1.0);
	glTranslatef(x+7,y,0);
	glutWireSphere(3,6,6);
	glPopMatrix();


glPopMatrix();
  glutSwapBuffers();
  glFlush ();
}

void reshape (int w, int h)
{
   int l=0;
   	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   	glMatrixMode (GL_PROJECTION);
   	glLoadIdentity ();
   	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 50.0); 
}
void Timer(int extra)
	{
	glutPostRedisplay();
	glutTimerFunc(20,Timer,0);
	}

int main(int argc, char** argv)
{
set_value();
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (1000, 800); 
   glutInitWindowPosition (10, 10);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutIdleFunc(calc);
   glutTimerFunc(30,Timer,0);
   glutReshapeFunc(reshape);
   glutMainLoop();
   return 0;
}
