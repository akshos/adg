#include <iostream>
#include <GL/glut.h>
#include <unistd.h>
#include <math.h>

using namespace std;

struct COLOR {
	GLfloat r, g, b;
};

COLOR *blue = new COLOR{0.0, 0.0, 1.0};
int xv, yv;
int padx, pady, padw, padh;
int padv;
int ballx, bally;
bool moveBall;


void circlePlotPoints(int xc, int yc, int x, int y, COLOR *c){
	glColor3f(c->r, c->g, c->b);
	glBegin(GL_LINES);
		glVertex2i(xc + x, yc + y);
		glVertex2i(xc - x, yc + y);
		
		glVertex2i(xc + x, yc - y);
		glVertex2i(xc - x, yc - y);
		
		glVertex2i(xc + y, yc + x);
		glVertex2i(xc - y, yc + x);
		
		glVertex2i(xc + y, yc - x);
		glVertex2i(xc - y, yc - x);
	glEnd();
}

void drawCircle(int xc, int yc, int r, COLOR *c){
	int x, y, p;
	y = r;
	x = 0;
	p = 1 - r;
	circlePlotPoints(xc, yc, x, y, c);
	while(x < y){
		x++;
		if( p < 0 )
			p += 2*x + 1;
		else {
			y--;
			p += 2 * (x - y) + 1;
		}
		circlePlotPoints(xc, yc, x, y, c);
	}
}

void drawBall(){
	drawCircle(ballx, bally, 20, blue);
}

void drawPad(){
	glBegin(GL_QUADS);
		glVertex2i(padx, pady);
		glVertex2i(padx, pady+padh);
		glVertex2i(padx+padw, pady+padh);
		glVertex2i(padx+padw, pady);
	glEnd();
}		

void ballLost(){
	cout << "GAME OVER" << endl;
	xv = 0; yv = 0;
	moveBall = false;
	padv = 0;
	sleep(3);
	glutPostRedisplay();
}
	

void animate() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	padx += padv;
	if(padx + padw > 790){
		padv = -1 * padv;
	}
	if(padx < 10){
		padv = -1 * padv;
	}
	drawPad();
	ballx += xv;
	bally += yv;
	drawBall();
	if(ballx > 780){
		xv *= -1;
	}
	if(ballx < 20){
		xv *= -1;
	}
	if(bally > 780){
		yv *= -1;
	}
	if(bally < padh + 10){
		if( ballx > padx && ballx < padx+padw ) {
			yv *= -1;
			xv += padv/2;
		}
	}
	if(bally < 10){
		ballLost();
	}
	glutSwapBuffers();
	usleep(500);
}

void display() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	xv = 0; yv = 0;
	ballx = 400; bally = 50;
	padw = 80;
	padh = 20;
	padx = 400 - padw/2;
	pady = 0;
	moveBall = false;
	drawBall();
	drawPad();
	glutSwapBuffers();
}

void mouseClick(int button, int state, int x, int y) {
}

void keyPress(unsigned char key, int x, int y) {
	if(key == 's' && moveBall == false) {
		ballx = 400; bally = 50;
		padx = 400 - padw/2;
		pady = 0;
		xv = yv = 3;
		padv = 4;
		moveBall = true;
	}
	
}

void specialKey(int key, int x, int y){
	if(key == GLUT_KEY_RIGHT) {
		cout << "Right key..." << endl;
		padv = abs(padv);
	}
	if(key == GLUT_KEY_LEFT) {
		cout << "Left key..." << endl;
		padv = -1 * abs(padv);
	}
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Animation");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 800);
	glutDisplayFunc(display);
	glutMouseFunc(mouseClick);
	glutKeyboardFunc(keyPress);
	glutSpecialFunc(specialKey);
	glutIdleFunc(animate);
	
	glutMainLoop();
	return 0;
}
