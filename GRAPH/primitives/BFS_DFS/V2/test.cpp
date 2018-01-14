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

//void render()
//{
//	cout << "started render" << endl;
//	glClearColor(1.0, 1.0, 1.0, 0.0);
//	glClear(GL_COLOR_BUFFER_BIT);
//	drawAllNodesEdges();
//	glFlush();
//	cout << "finished render" << endl;
//}

NODE *node1 = NULL, *node2 = NULL;
int edgeWeightFlag = 0;

void selectNode(int x, int y)
{
	if( node1 == NULL )
	{
		node1 = getNodeByXY(x, y);
		if(node1 != NULL )
			cout << "\nStart Node : " << node1->id << endl;
	}
	else
	{
		node2 = getNodeByXY(x, y);
		if( node2 != NULL )
		{
			if( node2 == node1 )
				return;
			cout << "End Node : " << node2->id << endl;			
			edgeWeightFlag = 1;
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
	if( edgeWeightFlag == 1 && key <= '9' && key >= '0' )
	{
		int weight = (int)(key-'0');
		cout << "Edge weight = " << weight << endl;
		createEdge(node1, node2, weight);
		node1 = node2 = NULL;
		edgeWeightFlag = 0;
		return;
	}
	switch(key)
	{
		case 'n' : printNodeList(); break;
		case 'a' : printAdjList(); break;
		case 'e' : printEdgeList(); break;
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
	glutIdleFunc(render);
	return 0;
}	
	

