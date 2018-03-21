#include<iostream>
#include<gl/glut.h>
void Draw()
{
	//glClear(GL_COLOR_BUFFER_BIT);

	int x=120,y=120,x1,y1;
	double theta=70*3.14/180;
	glBegin(GL_LINES);
	{
		glColor3f(1,0,0);
		glVertex2f(-200,0);
		glVertex2f(200,0);
		glVertex2f(0,-200);
		glVertex2f(0,200);		
	}
	glEnd();

	//glRotatef(45,0,0,1);
	glBegin(GL_LINES);
	{
		glColor3f(1,0,0);
		glVertex2f(0,0);
		glVertex2f(x,y);
	}
	glEnd();

	x1=x*cos(theta)+y*sin(theta);
	y1=-x*sin(theta)+y*cos(theta);
	glBegin(GL_LINES);
	{
		glColor3f(1,0,0);
		glVertex2f(0,0);
		glVertex2f(x1,y1);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glColor3f(1,0,0);
		glVertex2f(40,0);
		glVertex2f(60,0);
		glVertex2f(60,20);
		glVertex2f(40,20);		
	}	
	glEnd();
	glFlush();


}
void main()
{
	
	glutInitWindowSize(600,600);
	glutInitWindowPosition(0,0);
	glutCreateWindow("my screen");
	glClearColor(1.0, 1.0, 0.0, 0.0);// for backgroud color
	gluOrtho2D(-200,200,-200,200);
	glutDisplayFunc(Draw);
	glutMainLoop();

}