#include "primitives.h"

void tests()
{
	float pick_col[3];
	glReadPixels(199 , 199 , 1 , 1 , GL_RGB , GL_FLOAT , pick_col);
	cout << "Red:" << pick_col[0] << " Green:" << pick_col[1] << " Blue:" << pick_col[2] << endl; 
		
	for( int x = 100; x < 500; x++ )
	{
		glReadPixels(x , 100 , 1 , 1 , GL_RGB , GL_UNSIGNED_BYTE , pick_col);
		if( pick_col == BLACK )
		{
			cout << "Reached circle boundary at x = " << x << endl;
			break;
		}
	}
}

void render()
{
	cout << "started render" << endl;
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	drawAllEdges();
	drawAllNodes();
	
	glFlush();
	cout << "finished render" << endl;
}

NODE *begNode = NULL, *endNode = NULL;

void selectNode(int x, int y)
{
	if( begNode == NULL )
	{
		begNode = getNodeByXY(x, y);
		if(begNode != NULL )
			cout << "\nStart Node : " << begNode->id << endl;
	}
	else
	{
		endNode = getNodeByXY(x, y);
		if( endNode != NULL )//createEdge(begNode, endNode);
		{
			if( endNode == begNode )
				return;
			cout << "End Node : " << endNode->id << endl;
			createEdge(begNode, endNode);
			begNode = endNode = NULL;
		}	
	}
}

void mouseClick(int button, int state, int x, int y)
{
	if( button == GLUT_LEFT_BUTTON )
	{
		if( state == GLUT_DOWN )
		{
			addNode(x, 500-y);
			glFlush();
		}
	}
	if( button == GLUT_RIGHT_BUTTON )
	{
		if( state == GLUT_DOWN)
		{
			selectNode(x, 500-y);
			glFlush();
		}
	}
}

void reset()
{
	begNode = endNode = NULL;
	for( int i = 0; i < nodeCount; i++ )
	{
		if( nodeList[i].color != BLUE && nodeList[i].color != RED )
			nodeList[i].color = BLUE;
	}
	glutPostRedisplay();
}

void keyboardPress(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'n' : printNodeList(); break;
		case 'a' : printAdjList(); break;
		case 'e' : printEdgeList(); break;
		case 'r' : reset(); break;
		//case 'c' : clear(); break;
		case 'b' : bfs(); break;
		case 'd' : dfs(); break;
		default : cout << "unknown key" << endl;
	}
}

int main(int argc, char **argv)
{
	initializeOpenGL(argc, argv);
	glutDisplayFunc(render);
	glutMouseFunc(mouseClick);
	glutKeyboardFunc(keyboardPress);
	glutMainLoop();
	return 0;
}	
	

