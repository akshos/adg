//#include <GL/gl.h>
//#include <GL/glu.h>
#include<iostream>
#include<math.h>
#include <GL/glut.h>


static double x=0,y=0,sign=1,theta=0;


void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   	glClear (GL_COLOR_BUFFER_BIT);   	
   	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();
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

int main(int argc, char** argv)
{

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (1000, 800); 
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("Sample");
   glutDisplayFunc(display);  
   glutReshapeFunc(reshape);
   glutMainLoop();
   return 0;
}
