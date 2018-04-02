// Copyright [2018] <Akshay Venugopal>
#include <GL/glut.h>
#include <unistd.h>
#include <iostream>

#define MAX(x, y) (x > y)?x:y
#define MIN(x, y) (x < y)?x:y

using std;

struct COLOR {
    float red, green, blue;
};

const COLOR WHITE = {1, 1, 1};
const COLOR BLACK = {0.0, 0.0, 0.0};
const COLOR RED   = {0.8, 0.0, 0.0};
const COLOR GREEN = {0.0, 0.7, 0.0};
const COLOR BLUE  = {0.0, 0.0, 0.8};
const COLOR GRAY  = {0.6, 0.6, 0.6};


struct EDGE {
    int x1, y1, x2, y2;
    float m, im;
    bool processed = false;
};

EDGE edgeList[50];
int edgeCount = 0;
int flag = 0, polyFlag = 0;
int startX, startY, endX, endY;

void render();

int operator!=(const EDGE &e1, const EDGE &e2) {
    if ( e1.x1 == e2.x1 && e1.y1 == e2.y1 && e1.x2 == e2.x2 && e1.y2 == e2.y2)
        return 0;
    return 1;
}

void drawPolygon() {
    glColor3f(BLACK.red, BLACK.green, BLACK.blue);
    for ( int i = 0; i < edgeCount; i++ ) {
        glBegin(GL_LINES);
            glVertex2i(edgeList[i].x1, edgeList[i].y1);
            glVertex2i(edgeList[i].x2, edgeList[i].y2);
        glEnd();
    }
}

void printEdgeList() {
    for ( int i = 0; i < edgeCount; i++ ) {
        cout << "(" << edgeList[i].x1 << "," << edgeList[i].y1 << ")";
        cout << "->(" << edgeList[i].x2 << "," << edgeList[i].y2 << ")" << endl;
    }
}

void highlight(EDGE edge, COLOR c) {
    glColor3f(c.red, c.green, c.blue);
    glBegin(GL_LINES);
        glVertex2i(edge.x1, edge.y1);
        glVertex2i(edge.x2, edge.y2);
    glEnd();
    glFlush();
}

void sortEdgeList() {
    EDGE tmp;
    int x1, x2;
    for ( int i = 0; i < edgeCount - 1; i++ ) {
        for ( int j = 0; j < edgeCount - i - 1 ; j++ ) {
            x1 = MIN(edgeList[j].x1, edgeList[j].x2);
            x2 = MIN(edgeList[j+1].x1, edgeList[j+1].x2);
            if (x1 > x2) {
                tmp = edgeList[j];
                edgeList[j] = edgeList[j+1];
                edgeList[j+1] = tmp;
            }
        }
    }
}

int findMaxY(EDGE e1, EDGE e2, int i) {
    int ymax = 500;
    int xmax = MIN(e2.x1, e2.x2);
    int xmin = MAX(e1.x1, e1.x2);
    EDGE e3;
    for ( int k = i+1; k < edgeCount; k++ ) {
        e3 = edgeList[k];
        if (e3.y1 > e1.y1 && e3.y1 < e1.y2) {
            if (e3.x1 > xmin && e3.x2 < xmax) {
                if (e3 != e2 && e3.y1 < ymax) {
                    ymax = e3.y1;
                    highlight(e3, RED);
                }
            }
        }
    }
    if (ymax == 500)
        ymax = MIN(e1.y2, e2.y2+1);
    return ymax;
}

int scanFill(int i, int j, int y, float x1) {
    EDGE *e1 = &edgeList[i];
    EDGE *e2 = &edgeList[j];
    float x2 = e2->x1 + static_cast<float>(y-e2->y1) * e2->im);
    int ymax = findMaxY(*e1, *e2, i);
    glColor3f(0, 0, 0);
    while (y < ymax) {
        glBegin(GL_LINES);
            glVertex2i(static_cast<int>x1, y);
            glVertex2i(static_cast<int>x2, y);
        glEnd();
        glFlush();
        usleep(1000);
        y++;
        x1 += e1->im;
        x2 += e2->im;
    }
    e2->processed = true;
    return y;
}

int findOppEdge(int i, int y) {
    for (int j = i+1; j < edgeCount; j++) {
        if (y >= edgeList[j].y1 && y < edgeList[j].y2) {
            highlight(edgeList[j], GREEN);
            return j;
        }
    }
    return -1;
}

void processEdge(EDGE *e, int i) {
    e->processed = true;
    highlight(*e, GREEN);
    if (e->m == 0)
        return;
    int x, y, j;
    x = e->x1;
    y = e->y1;
    while (y < e->y2) {
        j = findOppEdge(i, y);
        if (j != -1) {
            y = scanFill(i, j, y, x);
        } else {
            y++;
        }
        x = e->x1 + (static_cast<float>(y-e->y1) * e->im);
    }
}

void polyFill() {
    cout << "Filling Polygon using scan line" << endl;
    sortEdgeList();
    printEdgeList();
    for ( int i = 0; i < edgeCount; i++ ) {
        if (edgeList[i].processed == false)
            processEdge(&edgeList[i], i);
    }
}

void addEdge(int x1, int y1, int x2, int y2) {
    int tmp;
    int xDiff, yDiff;
    float m, im;
    if (y1 > y2) {
        tmp = y1; y1 = y2; y2 = tmp;
        tmp = x1; x1 = x2; x2 = tmp;
    }
    edgeList[edgeCount].x1 = x1; edgeList[edgeCount].y1 = y1;
    edgeList[edgeCount].x2 = x2; edgeList[edgeCount].y2 = y2;
    xDiff = x2-x1;
    yDiff = y2-y1;
    if (yDiff == 0) {
        m = im = 0;
    } else if (xDiff == 0) {
        m = 1;
        im = 0;
    } else {
        m = static_cast<float>yDiff/static_cast<float>xDiff;
        im = static_cast<float>xDiff/static_cast<float>yDiff;
    }
    edgeList[edgeCount].m = m;
    edgeList[edgeCount].im = im;
    edgeCount++;
}

void selectPoint(int x, int y) {
    static int x1, y1, x2, y2;
    if (flag == 0) {
        x1 = startX = x; y1 = startY = y;
        flag = 1;
        return;
    }
    x2 = endX = x;
    y2 = endY = y;
    addEdge(x1, y1, x2, y2);
    x1 = x2; y1 = y2;
}

void reset() {
    for ( int i =0; i < edgeCount; i++ ) {
        edgeList[i].processed = false;
    }
    render();
}

void mouseClick(int button, int state, int x, int y) {
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
        if (polyFlag == 0) {
            selectPoint(x, 500-y);
            render();
        }
    }
}

void keyboardPress(unsigned char key, int x, int y) {
    switch (key) {
        case 'd': addEdge(startX, startY, endX, endY);
                    polyFlag = 1;
                    render();
                    break;
        case 'p': printEdgeList(); break;
        case 'f': polyFill(); break;
        case 'r': reset(); break;
        default : cout << "\nInvalid Key\n";
    }
}

void render() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    drawPolygon();
    glFlush();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(800, 500);
    glutCreateWindow("OpenGL LAB");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 500);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glutDisplayFunc(render);
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboardPress);
    edgeCount = 0;
    glutMainLoop();
    return 0;
}
