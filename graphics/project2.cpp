/* -*- Mode: C++ -*-
 * project2.cpp
 * Bruce C. Miller
 * An interactive floorplan creator.  The user draws walls, then * places
 * objects in the view window.
 */

#include <GL\glut.h>
#include <stdio.h>
#include <math.h>

#define PI 3.1415926535898                      // the constant PI
#define OBJ_MAX 100                             // max number of entries in obj list

// globals 
int width = 600;                                // starting width
int height = 500;                               // starting height
int mode = 1;                                   // draw mode (1 = walls, 2 = objects)
int brush = 0;                                  // the object drawmode selected
// 0 = none, 1 = walls, 2 = chairs
// 3 = couches, 4 = lamps, 5 = tables
struct point {                                  // struct type to populate arrays
    int x;
    int y;
};
struct wall {                                   // struct type for wallList
    point start;
    point end;
};
wall wallList[OBJ_MAX];                         // array of walls
int numWalls = 0;                               // number of walls
point chairList[OBJ_MAX];                       // array of chairs
int numChairs = 0;                              // number of chairs
point couchList[OBJ_MAX];                       // array of couches
int numCouches = 0;                             // number of couches
point lampList[OBJ_MAX];                        // array of lamps
int numLamps = 0;                               // number of lamps
point tableList[OBJ_MAX];                       // array of tables
int numTables = 0;                              // number of tables


/* draw a circle from a bunch of short lines */
void drawCircle(GLfloat xpos, GLfloat ypos, GLfloat radius) {
    GLfloat vectorX, vectorY;
    GLfloat vectorY1 = ypos + radius;
    GLfloat vectorX1 = xpos;
    glBegin(GL_LINE_STRIP);                     // trace circumference
    for(GLfloat angle = 0.0; angle <= (2.0 * PI); angle += 0.01) { 
        // x and y values for edge at each (2.0 * PI)th (628th) place
        vectorX = xpos + (radius * (GLfloat)sin(angle));
        vectorY = ypos + (radius * (GLfloat)cos(angle)); 
        glVertex2d(vectorX1, vectorY1);
        vectorY1 = vectorY;
        vectorX1 = vectorX; 
    }
    glEnd();
}

/* draw a string at the given RasterPos */
void drawString(GLfloat x, GLfloat y, char * s) {
    glRasterPos2f(x, y);                        // set text position
    // traverse string
    for (int j = 0; s[j] !=  '\0'; j++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, s[j]);
    }
}

/* draw a quad given the lower left and upper right vertex */
void drawQuad(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
    glBegin(GL_QUADS);                              
    glVertex2f(x1, y1);                         // bottom left
    glVertex2f(x2, y1);                         // bottom right
    glVertex2f(x2, y2);                         // top right
    glVertex2f(x1, y2);                         // top left
    glEnd();
}

/* draw a chair given the lower left vertex */
void drawChair(GLint x, GLint y) {
    glPolygonMode(GL_FRONT_AND_BACK, 
                  GL_LINE);                     // switch to line mode
    glLineWidth(1.0);
    glColor3f(0, 1, 0);
    glBegin(GL_LINE_LOOP);                      // draw chair outline
    glVertex2i(x, y);
    glVertex2i(x+2, y);
    glVertex2i(x+2, y+13);
    glVertex2i(x+23, y+13);
    glVertex2i(x+23, y);
    glVertex2i(x+25, y);
    glVertex2i(x+25, y+17);
    glVertex2i(x+2, y+17);
    glVertex2i(x+2, y+35);
    glVertex2i(x, y+35);
    glEnd();
}

/* draw a couch given the lower left vertex */
void drawCouch(GLint x, GLint y) {
    glPolygonMode(GL_FRONT_AND_BACK, 
                  GL_LINE);                     // switch to line mode
    glLineWidth(1.0);
    glColor3f(0, 1, 1);
    drawQuad(x, y+35, x+60, y);                 // outline
    drawQuad(x+10, y+25, x+50, y);              // inside lines
}

/* draw a lamp given the lower left vertex */
void drawLamp(GLint x, GLint y) {
    glPolygonMode(GL_FRONT_AND_BACK, 
                  GL_LINE);                     // switch to line mode
    glLineWidth(1.0);
    glColor3f(1, 0, 1);
    glBegin(GL_LINE_LOOP);                      // shade
    glVertex2i(x, y+15);
    glVertex2i(x+25, y+15);
    glVertex2i(x+20, y+35);
    glVertex2i(x+5, y+35);
    glEnd();
    drawCircle(x+13, y+8, 7);                   // base
}

/* draw a table given the lower left vertex */
void drawTable(GLint x, GLint y) {
    glPolygonMode(GL_FRONT_AND_BACK, 
                  GL_LINE);                     // switch to line mode
    glLineWidth(1.0);
    glColor3f(0, 0, 1);
    drawQuad(x, y+35, x+60, y);                 // simple rectangle
}

/* left-click menu callback function */
void menu(int value) {
    if (value == 0) {                           // exit option selected
        exit(0);
    } else {                                    // assign the selected tool
        brush = value;
    }
    glutPostRedisplay();
}

/* init routines */
void init(void) {
    glClearColor (0.0, 0.0, 0.0, 0.0);          // black background
    glShadeModel(GL_FLAT);                      // a uniform color shade model
}

/* display callback function */
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);               // clear background                             
    glPolygonMode(GL_FRONT_AND_BACK, 
                  GL_LINE);                     // switch to line mode
    // display the drawing area
    glLineWidth(3.0);                           // 3px wall width
    glColor3f(1, 0, 0);
    for (int i = numWalls; i > 0; i--) {        // draw all walls
        glBegin(GL_LINES);              
        glVertex2i(wallList[i].start.x, wallList[i].start.y);
        glVertex2i(wallList[i].end.x, wallList[i].end.y);
        glEnd();
    }
    for (i = numChairs; i > 0; i--) {           // draw all chairs
        drawChair(chairList[i].x, chairList[i].y);
    }
    for (i = numCouches; i > 0; i--) {          // draw all couches
        drawCouch(couchList[i].x, couchList[i].y);
    }
    for (i = numLamps; i > 0; i--) {            // draw all lamps
        drawLamp(lampList[i].x, lampList[i].y);
    }
    for (i = numTables; i > 0; i--) {           // draw all tables
        drawTable(tableList[i].x, tableList[i].y);
    }

    //menus
    glutCreateMenu(menu);                       // menu callback
    if (mode == 1) {                            // wall mode menu
        glutAddMenuEntry("walls", 1);
    } else if (mode == 2) {                     // object mode menu
        glutAddMenuEntry("chairs", 2);
        glutAddMenuEntry("couches", 3);
        glutAddMenuEntry("lamps", 4);
        glutAddMenuEntry("tables", 5);
    }
    glutAddMenuEntry("exit", 0);                // exit menu option
    glutAttachMenu(GLUT_RIGHT_BUTTON);          // assoc menu with right mouse

    // toolbar
    glColor3f(1, 1, 1);                         // background color for toolbar
    glLineWidth(1.0);                           // 1px line thinkness
    glPolygonMode(GL_FRONT_AND_BACK, 
                  GL_FILL);                     // switch to fill mode
    drawQuad(0, height,                         // solid filled quad for toolbar
             width, height-40);
    glPolygonMode(GL_FRONT_AND_BACK, 
                  GL_LINE);                     // switch to line mode
    glColor3f(1, 0, 0);                         // red text
    drawString(width-40, height-25,             // exit button
               "Exit");        
    if (mode == 1) {                            // wall mode
        glLineWidth(3.0);                       // 3px walls
        glBegin(GL_LINES);                      // wall button
        glVertex2i(20, height-20);
        glVertex2i(80, height-20);
        glEnd();
        drawString(width-120,                   // continue button
                   height-25, "Continue");
    } else {                                    // object mode
        drawChair(5, height-38);                // chair button
        drawCouch(45, height-38);               // couch button
        drawLamp(110, height-38);               // lamp button
        drawTable(150, height-38);              // table button
    }
    glColor3f(0, 0, 0);                         // color for highlight
    glLineWidth(1.0);                           // 1px line thinkness
    switch (brush) {                             // highlight selected tool
    case 1:
        drawQuad(18, height-1, 82, height-39);
        break;
    case 2:
        drawQuad(3, height-1, 37, height-39);
        break;
    case 3:
        drawQuad(43, height-1, 107, height-39);
        break;
    case 4:
        drawQuad(108, height-1, 140, height-39);
        break;
    case 5:
        drawQuad(148, height-1, 212, height-39);
        break;
    default:
        break;
    }

    glutSwapBuffers();                          // swap the buffers
    glFlush();                                  // flush cmd buffer
}

/* mouse click callback function */
void onClick(int button, int state, int x, int y) {
    // convert y to display coords
    y = height - y;

    // wall mode toolbar handling
    if(button == GLUT_LEFT_BUTTON &&            // wall menu
       state == GLUT_UP && mode == 1) {
        if (x >= width-45 && x <= width &&      // exit button
            y >= height-40 && y <= height) {
            exit(0);
        } else if (x >= width-115 && x <= width-50 && // continue button
                   y >= height-40 && y <= height) {
            mode = 2;                           // switch modes
            brush = 0;                          // reset brush
        } else if (x >= 0 && x <= 90 &&         // wall button
                   y >= height-40 && y <= height) {
            brush = 1;                          // set brush to walls
        } else if (x >= 0 && x <= width &&      // wall draw end
                   y >= 0 && y <= height-40 &&
                   brush == 1) {
            wallList[numWalls].end.x = x;       // set the end point
            wallList[numWalls].end.y = y;
        }
    }
    // object mode toolbar handling
    if(button == GLUT_LEFT_BUTTON &&            // object menu
       state == GLUT_UP && mode == 2) {
        if (x >= width-45 && x <= width &&      // exit button
            y >= height-40 && y <= height) {
            exit(0);                            // sys exit
        } else if (x >= 0 && x <= 40 &&         // chair button
                   y >= height-40 && y <= height) {
            brush = 2;                          // set brush to chair
        } else if (x >= 0 && x <= 105 &&        // couch button
                   y >= height-40 && y <= height) {
            brush = 3;                          // set brush to couch
        } else if (x >= 0 && x <= 145 &&        // lamp button
                   y >= height-40 && y <= height) {
            brush = 4;                          // set brush to lamp
        } else if (x >= 0 && x <= 210 &&        // table button
                   y >= height-40 && y <= height) {
            brush = 5;                          // set brush to table
        }
    }
    // drawing
    if(button == GLUT_LEFT_BUTTON &&            // if button down
       state == GLUT_DOWN && 
       x >= 0 && x <= width &&                  //  and in draw area
       y >= 0 && y <= height-40) {
        if (brush == 1) {                       // walls
            numWalls++;                         // increment num of walls
            wallList[numWalls].start.x = x;     // set the start point
            wallList[numWalls].start.y = y;
            wallList[numWalls].end.x = x;       // init endpoint
            wallList[numWalls].end.y = y;
        } else if (brush == 2) {                // chairs
            numChairs++;                        // increment num of chairs
            chairList[numChairs].x = x;         // set chair location
            chairList[numChairs].y = y;
        } else if (brush == 3) {                // couches
            numCouches++;                       // increment num of couches
            couchList[numCouches].x = x;        // set couch location
            couchList[numCouches].y = y;
        } else if (brush == 4) {                // lamps
            numLamps++;                         // increment num of lamps
            lampList[numLamps].x = x;           // set lamp location
            lampList[numLamps].y = y;
        } else if (brush == 5) {                // tables
            numTables++;                        // increment num of tables
            tableList[numTables].x = x;         // set table location
            tableList[numTables].y = y;
        }
    }
    glutPostRedisplay();
}

/* mouse drag callback function */
void onDrag(int x, int y) {
    // convert y to display coords
    y = height - y;
    //update wall end to current coords
    if (mode == 1 && brush && 1) {
        wallList[numWalls].end.x = x;
        wallList[numWalls].end.y = y;
    }
    glutPostRedisplay();
}

/* window reshape callback function */
void reshape(int x, int y) {
    // set width and height to current window dimensions
    width = x;
    height = y;
    glViewport (0, 0, (GLsizei) width, 
                (GLsizei) height);              // reset viewport
    glLoadIdentity ();                          // clear current matrix
    gluOrtho2D (0.0, (GLdouble) width, 0.0, 
                (GLdouble) height);             // orthographic projection
    glutPostRedisplay();
}

/* main function */
int main(int argc, char** argv) {
    char ch[] = "";                             // junk string for gets()
    glutInit(&argc, argv);                      // init opengl
    glutInitDisplayMode(GLUT_DOUBLE |
                        GLUT_RGB);              // double buffer, RBG palette
    glutInitWindowSize(width, height);          // window frame size
    glutInitWindowPosition(0, 0);               // top-left origin
    glutCreateWindow("CPS547: Project 2");      // title bar text
    init();                                     // some initialization routines
    glutDisplayFunc(display);                   // display callback
    glViewport(0, 0, (GLsizei) width, 
               (GLsizei) height);               // set viewport
    glMatrixMode(GL_PROJECTION);                // activate projection matrix
    glLoadIdentity();                           // clear current matrix
    gluOrtho2D(0.0, (GLdouble) width, 0.0, 
                (GLdouble) height);             // orthographic projection

    // interaction
    glutMouseFunc(onClick);                     // mouse click callback
    glutMotionFunc(onDrag);                     // mouse drag callback
    glutReshapeFunc(reshape);                   // reshape callback

    // instructions printed to console
    printf("CPS547: Project 2\n-----------------\n");
    printf("\nFloorplan Creator Instructions.\n\n");
    printf("Select walls from the toolbar and place them by clicking and ");
    printf("dragging in\nthe view window below it. Click continue when ");
    printf("complete. To place objects,\nselect them from the preceding");
    printf(" toolbar or the popup menu and click once to\nplace. Click ");
    printf("Exit at any time to quit the program.\n\nPress return to proceed.");
    gets(ch);                                   // await carriage return

    glutMainLoop ();                            // enter window display loop
    return(0);
}
