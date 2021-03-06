//#include <GL/gl.h>
//#include <GL/glu.h>
#include<iostream>
#include<math.h>
#include <GL/glut.h>
#include <unistd.h>

static double x=0,y=0,sign=1,theta=0;

void init(void) 
{
glClearColor (0.0, 0.0, 0.0, 0.0);
glShadeModel(GL_SMOOTH);
glEnable(GL_DEPTH_TEST);

/*GLfloat light_position[] = { 0, 1.0, 3.0, 0.0 };
//GLfloat light_position1[] = { 4, 1.0, 0.0, 0.0 };
GLfloat white_light[] = { 1.0,1.0, 1.0, 1.0 };

GLfloat lmodel_ambient[] = { 1.1, 1.0, 1.0, 0.0 };

GLfloat light_diffuse[]={1.0, 1.0, 1.0, 0.0};
GLfloat mat_diffuse[]={2.0, 2.0, 1.0, 0.0};
GLfloat mat_shininess[] = { 100.0 };

glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

glLightfv(GL_LIGHT0, GL_POSITION, light_position);
glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);


//glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
//glEnable(GL_LIGHT1);
glColorMaterial(GL_FRONT, GL_DIFFUSE);
glEnable(GL_COLOR_MATERIAL);


GLfloat mat_specular[] = { 2.0, 6.0, 6.0, 6.0 };
glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);*/

}
void calc(void) 
{
	sleep(1);
	//x=x+0.001;
    y=y+sign*0.01;
	theta=theta+0.5;
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
	glBegin(GL_POLYGON);
		glVertex2f(5,5);
		glVertex2f(9,5);
		glVertex2f(7,8);
    glEnd();
	glPopMatrix();

glPushMatrix();
	glTranslatef(x-10,y,0);
	glBegin(GL_POLYGON);
		glVertex2f(5,5);
		glVertex2f(9,5);
		glVertex2f(7,8);
    glEnd();
	glPopMatrix();

glPushMatrix();
	glRotatef(theta,0,0,1);
	glBegin(GL_POLYGON);
		glVertex2f(5,5);
		glVertex2f(9,5);
		glVertex2f(7,8);
    glEnd();
glPopMatrix();

glPushMatrix();

	glRotatef(theta+50,0,0,1);
	glTranslatef(5,5,0);
	/*glBegin(GL_POLYGON);
		glVertex2f(5,5);
		glVertex2f(9,5);
		glVertex2f(7,8);
    glEnd();*/
	glutSolidSphere(5,20,20);
glPopMatrix();

glPushMatrix();
	glRotatef(-theta+50,0,0,1);
	glBegin(GL_POLYGON);
		glVertex2f(5,5);
		glVertex2f(9,5);
		glVertex2f(7,8);
    glEnd();
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
   glutInitWindowSize (300, 200); 
   glutInitWindowPosition (400, 400);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutIdleFunc(calc);
   glutTimerFunc(30,Timer,0);
   glutReshapeFunc(reshape);
   glutMainLoop();
   return 0;
}
