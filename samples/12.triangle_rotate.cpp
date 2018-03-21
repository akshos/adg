
#include <windows.h>
#include <gl\glut.h>

int theta=0;

void abc(void)
{
	Sleep(10);
	theta=theta+1;
	glutPostRedisplay();
}
void Display(void)
{
glClear(GL_COLOR_BUFFER_BIT);
   
glPushMatrix();

// triagle
/*glColor3f (1.0, 0.0, 0.0);	 
	glBegin(GL_POLYGON);
		glVertex2f(50,50);
		glVertex2f(90,50);
		glVertex2f(70,85);
    glEnd();

glColor3f (0.0, 1.0, 0.0);	 
glRotatef(10,0,0,1);
glBegin(GL_POLYGON);
		glVertex2f(50,50);
		glVertex2f(90,50);
		glVertex2f(70,85);
    glEnd();*/

glColor3f (0.0, 0.0, 1.0);	
glTranslatef(50,50,0);
glRotatef(theta,0,0,1);
glBegin(GL_POLYGON);
		glVertex2f(50,50);
		glVertex2f(90,50);
		glVertex2f(70,85);
    glEnd();

	glTranslatef(-50,-50,0);
glPopMatrix();

//dont wait, start flushing OpenGL calls to display buffer
glFlush();
}
void reshape (int w, int h)
{
// on reshape and on startup, keep the viewport to be the entire size of the window
glViewport (0,0, (GLsizei) w, (GLsizei) h);
glMatrixMode (GL_PROJECTION);
glLoadIdentity();
//keep our world coordinate system constant
gluOrtho2D(0.0,360.0,0.0,360.01);

}
void init(void)
{
//glClearColor(1.0,1.0,0.0,1.0);
// set the point size to be 5.0 pixels
glPointSize(5.01);
}
void main(int argc, char* argv[])
{
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
glutInitWindowSize(800,800);
glutCreateWindow("My First OpenGL Window");
init();
glutDisplayFunc(Display);
glutReshapeFunc(reshape);
glutIdleFunc(abc);
glutMainLoop();
}