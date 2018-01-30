#include "primitives.h"

NODE *node1 = NULL, *node2 = NULL;

void selectNode(int x, int y)
{
	if( node1 == NULL )
	{
		node1 = getNodeByXY(x, y);
		if(node1 != NULL )
		{
			cout << "\nStart Node : " << node1->id << endl;
		}
	}
	else
	{
		node2 = getNodeByXY(x, y);
		if( node2 != NULL )
		{
			if( node2 == node1 )
				return;
			cout << "End Node : " << node2->id << endl;
			int weight;
			cout << "Enter the edge weight : ";
			cin >> weight;
			createEdge(node1, node2, weight);
			node1 = node2 = NULL;
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

void keyboardPress(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'n' : printNodeList(); break;
		case 'a' : printAdjList(); break;
		case 'e' : printEdgeList(); break;
		case 'k' : kruskal(); break;
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
	glutIdleFunc(render);
	return 0;
}	
	

