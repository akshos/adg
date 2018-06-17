/*
Author : Akshay Venugopal
Program : Red-Black Tree
Roll No : 207
*/ 

#include <GL/glut.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iostream>

using namespace std;

#define MAX(x,y) (x>y)?x:y
#define YDIFF 30
#define XDIFF 40
#define XSTART 400
#define YSTART 460
#define RADIUS 20
#define XPAD 10

struct POINT
{
	int x, y;
};

struct COLOR
{
	float red, green, blue;
};

const COLOR WHITE = {1.0, 1.0, 1.0};
const COLOR BLACK = {0.0, 0.0, 0.0};
const COLOR RED   = {0.9, 0.0, 0.0};
const COLOR GREEN = {0.0, 1.0, 0.0};
const COLOR BLUE  = {0.0, 0.0, 0.8};
const COLOR GRAY  = {0.6, 0.6, 0.6};

struct NODE
{
	int xCenter, yCenter;
	COLOR color, originalColor;
	int value;
	int nil;
	NODE *parent, *rchild, *lchild;
};

NODE *nil;
NODE *treeRoot;

void display();

int operator==(COLOR c1, COLOR c2)
{
	if( c1.red == c2.red && c1.green == c2.green && c1.blue == c2.blue )
		return 1;
	return 0;
}

int operator!=(COLOR c1, COLOR c2)
{
	if( c1.red == c2.red && c1.green == c2.green && c1.blue == c2.blue )
		return 0;
	return 1;
}

void drawLine( int x1, int y1, int x2, int y2, COLOR color)
{
	glColor3f(color.red, color.green, color.blue);
	glBegin(GL_LINES);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
	glEnd();
}

void circleFillColor(int xCenter, int yCenter, int x, int y, COLOR fillColor) 
{
	int yUp, yDown, limit, i;
	glColor3f(fillColor.red, fillColor.green, fillColor.blue);
	glBegin(GL_POINTS);
		limit = xCenter + x; yUp = yCenter + y; yDown = yCenter - y;
		for( i = xCenter-x+1; i < limit; i++ )
		{
			glVertex2i(i, yUp);
			glVertex2i(i, yDown);
		}
		limit = xCenter + y; yUp = yCenter + x;	yDown = yCenter - x;
		for( i = xCenter-y+1; i < limit; i++ )
		{
			glVertex2i(i, yUp);
			glVertex2i(i, yDown);
		}
	glEnd();
}

void circlePlotPoints(int xCenter, int yCenter, int x, int y )
{
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
		glVertex2i(xCenter + x, yCenter + y );
		glVertex2i(xCenter - x, yCenter + y );
		glVertex2i(xCenter + x, yCenter - y );
		glVertex2i(xCenter - x, yCenter - y );
		glVertex2i(xCenter + y, yCenter + x );
		glVertex2i(xCenter - y, yCenter + x );
		glVertex2i(xCenter + y, yCenter - x );
		glVertex2i(xCenter - y, yCenter - x );
	glEnd();
}

void drawCircle( int xCenter, int yCenter, int radius, COLOR fillColor )
{
	int x = 0, y = radius, p = 1 - radius;
	circlePlotPoints( xCenter, yCenter, x, y );
	circleFillColor(xCenter, yCenter, x, y, fillColor);
	while( x < y )
	{
		x++;
		if( p < 0 )
		{
			p += (2 * x) + 1;
		}
		else
		{
			y--;
			p += ( 2*(x-y) ) +1;
		}
		circlePlotPoints(xCenter, yCenter, x, y);
		circleFillColor(xCenter, yCenter, x, y, fillColor);
	}
	//colorFill(xCenter, yCenter, fillColor);
}

void drawString(int x, int y, char *str, COLOR color)
{
	glColor3f(color.red, color.green, color.blue);
	glRasterPos2i(x, y);
	for( str; *str != '\0'; str++ )
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *str);
}

void leftRotate(NODE *x)
{
	NODE *y = x->rchild;
	x->rchild = y->lchild;
	if(y->lchild != nil)
		y->lchild->parent = x;
	y->parent = x->parent;
	if(x->parent == nil)
		treeRoot = y;
	else if( x == x->parent->lchild )
		x->parent->lchild = y;
	else
		x->parent->rchild = y;
	y->lchild = x;
	x->parent = y;
	display();
	usleep(500000);
}

void rightRotate(NODE *x)
{
	NODE *y = x->lchild;
	x->lchild = y->rchild;
	if(y->rchild != nil)
		y->rchild->parent = x;
	y->parent = x->parent;
	if(x->parent == nil)
		treeRoot = y;
	else if( x == x->parent->lchild )
		x->parent->lchild = y;
	else
		x->parent->rchild = y;
	y->rchild = x;
	x->parent = y;
	display();usleep(500000);
}

void insertFixUp(NODE *z)
{
	NODE *y;
	while(z->parent->color == RED)
	{
		if(z->parent == z->parent->parent->lchild)
		{
			y = z->parent->parent->rchild;
			if(y->color == RED)
			{
				z->parent->color = BLACK;
				display();usleep(500000);
				y->color = BLACK;
				display();usleep(500000);
				z->parent->parent->color = RED;
				display();usleep(500000);
				z = z->parent->parent;
			}
			else 
			{
				if(z == z->parent->rchild)
				{
					z = z->parent;
					leftRotate(z);
				}
				z->parent->color = BLACK;
				display();usleep(500000);
				z->parent->parent->color = RED;
				display();usleep(500000);
				rightRotate(z->parent->parent);
			}
		}
		else
		{
			y = z->parent->parent->lchild;
			if(y->color == RED)
			{
				z->parent->color = BLACK;
				display();usleep(500000);
				y->color = BLACK;
				display();usleep(500000);
				z->parent->parent->color = RED;
				display();usleep(500000);
				z = z->parent->parent;
			}
			else 
			{
				if(z == z->parent->lchild)
				{
					z = z->parent;
					rightRotate(z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				display();usleep(500000);
				leftRotate(z->parent->parent);
			}
		}
	}
	treeRoot->color = BLACK;
}

void insertTree(NODE *z)
{
	cout << "Inserting node..." << endl;
	NODE *y = nil;
	NODE *x = treeRoot;
	while(x != nil)
	{
		y = x;
		if(z->value < x->value)
			x = x->lchild;
		else
			x = x->rchild;
	}
	z->parent = y;
	if(y == nil)
		treeRoot = z;
	else if(z->value < y->value)
		y->lchild = z;
	else
		y->rchild = z;
	z->lchild = nil;
	z->rchild = nil;
	z->color = RED;
	display();
	cout << "Node inserted..." << endl;
	sleep(1);
	cout << "Fixing tree..." << endl;
	insertFixUp(z);
	cout << "done" << endl;
}

void addElement()
{
	int value = 0;
	NODE *node;
	cout << "Inserting nodes..." << endl;
	while(value != -1)
	{
		cout << "Enter the node value : ";
		cin >> value;
		if(value == -1)
			break;
		node = new NODE;
		node->value = value;
		node->lchild = nil;
		node->rchild = nil;
		node->nil = 0;
		insertTree(node);
		display();
	}
}

NODE *minimum(NODE *node)
{
	if(node->lchild != nil)
		return minimum(node->lchild);
	return node;
}

void transplant(NODE *u, NODE *v)
{
	if(u->parent == nil)
		treeRoot = v;
	else if( u == u->parent->lchild )
		u->parent->lchild = v;
	else
		u->parent->rchild = v;
	v->parent = u->parent;
}

void deleteFixUp(NODE *x)
{
	NODE *w;
	while( x != treeRoot && x->color == BLACK )
	{
		if(x == x->parent->lchild)
		{
			w = x->parent->rchild;
			if( w->color == RED )
			{
				w->color = BLACK;
				display();usleep(500000);
				x->parent->color = RED;
				display();usleep(500000);
				leftRotate(x->parent);
				w = x->parent->rchild;
			}
			if(w->lchild->color == BLACK && w->rchild->color == BLACK)
			{
				w->color = RED;
				display();usleep(500000);
				x = x->parent;
			}
			else
			{
				if(w->rchild->color == BLACK)
				{
					w->lchild->color = BLACK;
					display();usleep(500000);
					w->color = RED;
					display();usleep(500000);
					rightRotate(w);
					w = x->parent->rchild;
				}
				w->color = x->parent->color;
				display();usleep(500000);
				x->parent->color = BLACK;
				display();usleep(500000);
				w->rchild->color = BLACK;
				display();usleep(500000);
				leftRotate(x->parent);
				x = treeRoot;
			}
		}
		else
		{
			w = x->parent->lchild;
			if( w->color == RED )
			{
				w->color = BLACK;
				display();usleep(500000);
				x->parent->color = RED;
				display();usleep(500000);
				rightRotate(x->parent);
				w = x->parent->lchild;
			}
			if(w->rchild->color == BLACK && w->lchild->color == BLACK)
			{
				w->color = RED;
				display();usleep(500000);
				x = x->parent;
			}
			else
			{
				if(w->lchild->color == BLACK)
				{
					w->rchild->color = BLACK;
					display();usleep(500000);
					w->color = RED;
					display();usleep(500000);
					leftRotate(w);
					w = x->parent->lchild;
				}
				w->color = x->parent->color;
				display();usleep(500000);
				x->parent->color = BLACK;
				display();usleep(500000);
				w->lchild->color = BLACK;
				rightRotate(x->parent);
				x = treeRoot;
			}
		}
	}
	x->color = BLACK;
	display();usleep(500000);
}

void deleteTreeNode(NODE *z)
{
	NODE *y = z;
	NODE *x;
	y->originalColor = y->color;
	if( z->lchild == nil )
	{
		x = z->rchild;
		transplant(z, z->rchild);
	}
	else if( z->rchild == nil )
	{
		x = z->lchild;
		transplant(z, z->lchild);
	}
	else
	{
		y = minimum(z->rchild);
		y->originalColor = y->color;
		x = y->rchild;
		if( y->parent == z )
			x->parent = y;
		else
		{
			transplant(y, y->rchild);
			y->rchild = z->rchild;
			y->rchild->parent = y;
		}
		transplant(z, y);
		y->lchild = z->lchild;
		y->lchild->parent = y;
		y->color = z->color;
	}
	cout << "Node removed..." << endl;
	if( y->originalColor == BLACK )
	{
		cout << "Fixing Tree..." << endl;
		deleteFixUp(x);
	}
	cout << "done" << endl;
}

NODE* getNodeByValue(NODE *node, int value)
{
	if(node == nil)
		return nil;
	if(value < node->value)
		return getNodeByValue(node->lchild, value);
	else if(value > node->value)
		return getNodeByValue(node->rchild, value);
	return node;
}
	
void deleteElement()
{
	int value = 0;
	NODE *delNode;
	while( value != -1 )
	{
		cout << "Enter the value to delete : ";
		cin >> value;
		if(value == -1)
			break;
		delNode = getNodeByValue(treeRoot, value);
		if(delNode == nil)
		{
			cout << "Please enter a valid node value..." << endl;
			continue;
		}
		cout << "Deleting node with value : " << delNode->value << endl;
		deleteTreeNode(delNode);
		display();
	}
}

int treeHeight(NODE *root)
{
	if(root == nil)
		return 0;
	int h = MAX(treeHeight(root->lchild)+1, treeHeight(root->rchild)+1);
	return h;
}

void drawEdge(NODE *node, int x, int y)
{
	drawLine(node->xCenter, node->yCenter, x, y, BLACK);
}

void drawNode(NODE *node, int x, int y)
{
	node->xCenter = x;
	node->yCenter = y;
	drawCircle(x, y, RADIUS, node->color);
	char str[5];
	sprintf(str, "%d", node->value);
	drawString(x-5, y+5, str, WHITE);
}

void drawSubTree(NODE *root, int height, int currLevel)
{
	int x, y;
	if(root == nil || root == NULL) 
		return;
	if(root->lchild != nil)
	{
		x = root->xCenter - ( pow(3,height) + XPAD );
		y = root->yCenter - (height*YDIFF);
		drawEdge(root, x, y);
		drawNode(root->lchild, x, y);
	}
	if(root->rchild != nil)
	{
		x = root->xCenter + ( pow(3,height) + XPAD );
		y = root->yCenter - (height*YDIFF);
		drawEdge(root, x, y);
		drawNode(root->rchild, x, y);
	}
	drawSubTree(root->lchild, height-1, currLevel+1);
	drawSubTree(root->rchild, height-1, currLevel+1);
}

void drawTree()
{
	if(treeRoot == nil || treeRoot == NULL)
		return;
	int height = treeHeight(treeRoot);
	cout << "Tree Height : " << height << endl;
	drawNode(treeRoot, XSTART, YSTART);
	drawSubTree(treeRoot, height, 1);
}

void display()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawTree();
	glFlush();
}

void keyboardPress(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'i' : addElement(); break;
		case 'd' : deleteElement(); break;
		case 'q' : exit(0);
		default : cout << "Please enter a valid option" << endl;
					fflush(stdin);
	}
}
	
int main(int argc, char **argv)
{
	nil = new NODE;
	nil->nil = 1;
	nil->color = BLACK;
	treeRoot = nil;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(800, 500);
	glutCreateWindow("OpenGL LAB");
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 500);
	glutDisplayFunc(display);
	//glutMouseFunc(mouseClick);
	glutKeyboardFunc(keyboardPress);
	glutMainLoop();
	return 0;
}
