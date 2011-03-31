/* -*- Mode: C++ -*-
 * project3.cpp
 * Bruce C. Miller
 * 11.09.02
 * Program displays a smiling bee travelling across a pastoral forest scene.
 */

#include <GL\glut.h>
#include <stdio.h>
#include <math.h>

#define PI 3.1415926535898                      // the constant PI

// globals 
int width = 600;                                // starting width
int height = 500;                               // starting height

float beex = width-100;                         // bee x coord
float beey = 270.0;                             // bee y coord
float beewing = 45.00;                          // bee wing rotation
bool down = true;                               // wing direction

/* draw a circle from a bunch of short lines */
void drawCircle(GLfloat xpos, GLfloat ypos, GLfloat radius) {
    GLfloat vectorX, vectorY;
    GLfloat vectorY1 = ypos + radius;
    GLfloat vectorX1 = xpos;
    glBegin(GL_POLYGON);                        // trace circumference
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

/* draw a quad given the lower left and upper right vertex */
void drawQuad(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
    glBegin(GL_QUADS);                              
    glVertex2f(x1, y1);                         // bottom left
    glVertex2f(x2, y1);                         // bottom right
    glVertex2f(x2, y2);                         // top right
    glVertex2f(x1, y2);                         // top left
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

/* draw a unit square at origin */
void unitSquare() {
    drawQuad(0, 0, 1, 1);
}

/* draw a unit circle at origin */
void unitCircle() {
    drawCircle(0, 0, 1);
}

/* draw a tree */
void drawTree(GLint x, GLint y) {
    glBegin(GL_POLYGON);
    glVertex2i(x, y+25);
    glVertex2i(x+100, y+25);
    glVertex2i(x+50, y+125);
    glEnd();
    drawQuad(x+40, y, x+60, y+25);
}

/* draw a bee */
void drawBee(int x, int y, int w) {
    glPushMatrix();                             // thorax
    glTranslatef(46.0+x, 27.0+y, 1.0);          // move thorax
    glScalef(11.0, 11.0, 1.0);                  // 22 diameter thorax
    glPolygonMode(GL_FRONT_AND_BACK, 
                  GL_FILL);                     // switch to fill mode
    glColor3f(1.0, 1.0, 0.0);                   // bee yellow 
    unitCircle();
    glPolygonMode(GL_FRONT_AND_BACK, 
                  GL_LINE);                     // switch to line mode
    glColor3f(0.0, 0.0, 0.0);                   // black
    unitCircle();
    glPopMatrix();                              // end thorax

    glPushMatrix();                             // head
    glTranslatef(25.0+x, 30.0+y, 1.0);          // move head
    glScalef(12.0, 12.0, 1.0);                  // 24 diameter head
    glPolygonMode(GL_FRONT_AND_BACK, 
                  GL_FILL);                     // switch to fill mode
    glColor3f(1.0, 1.0, 0.0);                   // bee yellow 
    unitCircle();
    glPolygonMode(GL_FRONT_AND_BACK, 
                  GL_LINE);                     // switch to line mode
    glColor3f(0.0, 0.0, 0.0);                   // black
    unitCircle();
    glPopMatrix();                              // end head

    glBegin(GL_LINE_STRIP);                     // smiley face :)
    glVertex2i(20+x, 28+y);
    glVertex2i(20+x, 24+y);
    glVertex2i(28+x, 24+y);
    glVertex2i(28+x, 28+y);
    glEnd();
    glPointSize(2);
    glBegin(GL_POINTS);                         // eyeballs
    glVertex2i(21+x, 34+y);
    glVertex2i(26+x, 34+y);
    glEnd();
    glBegin(GL_LINE_STRIP);                     // antennea
    glVertex2i(18+x, 41+y);
    glVertex2i(15+x, 49+y);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2i(24+x, 43+y);
    glVertex2i(22+x, 52+y);
    glEnd();
    glBegin(GL_POINTS);
    glVertex2i(15+x, 49+y);
    glVertex2i(22+x, 52+y);
    glEnd();
    glPointSize(1);

    glPushMatrix();                             // abdomen
    glTranslatef(76.0+x, 27.0+y, 1.0);          // move abdomen
    glScalef(20.0, 15.0, 1.0);                  // 24 diameter abdomen
    glPolygonMode(GL_FRONT_AND_BACK, 
                  GL_FILL);                     // switch to fill mode
    glColor3f(1.0, 1.0, 0.0);                   // bee yellow 
    unitCircle();
    glPolygonMode(GL_FRONT_AND_BACK, 
                  GL_LINE);                     // switch to line mode
    glColor3f(0.0, 0.0, 0.0);                   // black
    unitCircle();
    glPopMatrix();                              // end abdomen

    glBegin(GL_LINE_STRIP);                     // legs
    glVertex2i(40+x, 20+y);
    glVertex2i(36+x, 14+y);
    glVertex2i(39+x, 5+y);
    glEnd();
    glBegin(GL_LINE_STRIP);                     // legs
    glVertex2i(44+x, 18+y);
    glVertex2i(40+x, 12+y);
    glVertex2i(43+x, 3+y);
    glEnd();
    glBegin(GL_LINE_STRIP);                     // legs
    glVertex2i(48+x, 18+y);
    glVertex2i(44+x, 12+y);
    glVertex2i(47+x, 3+y);
    glEnd();

    glPushMatrix();                             // wings
    glTranslatef(45.0+x, 44.0+y, 1.0);          // move wings
    glRotatef(w, 0.0, 0.0, 1.0);                // rotate around z axis
    glTranslatef(11.0, 0.0, 0.0);
    glScalef(22.0, 12.0, 1.0);                  // 22 diameter wings
    glPolygonMode(GL_FRONT_AND_BACK, 
                  GL_FILL);                     // switch to fill mode
    glColor3f(0.75, 0.75, 0.75);                // wings grey
    unitCircle();
    glPolygonMode(GL_FRONT_AND_BACK, 
                  GL_LINE);                     // switch to line mode
    glColor3f(0.0, 0.0, 0.0);                   // black
    unitCircle();
    glPopMatrix();                              // end thorax
}

/* init routines */
void init (void) {
    glClearColor (0.0, 1.0, 1.0, 0.0);          // blue background
    glShadeModel(GL_FLAT);                      // a uniform color shade model
}

/* display callback function */
void display (void) {
    glClear(GL_COLOR_BUFFER_BIT);               // clear background                             
    glPolygonMode(GL_FRONT_AND_BACK, 
                  GL_FILL);                     // switch to fill mode
    // draw background
    glColor3f(0.0, 0.5, 0.0);                   // dark green
    glPushMatrix();
    glScalef(width, 350, 1.0);
    unitSquare();                               // ground
    glPopMatrix();
    glColor3f(0.0, 1.0, 0.0);                   // light green
    for (int i = 50; i < width; i += 200) {     // draw some trees
        drawTree(i, 350);
    }
    for (i = 50; i < width; i += 200) {         // draw some trees
        drawTree(i, 10);
    }
    drawBee(beex, beey, beewing);

    glutSwapBuffers();                          // swap the buffers
    glFlush();                                  // flush cmd buffer     
}

/* window reshape callback function */
void reshape(int x, int y) {
    // set width and height to current window dimensions
    width = x;
    height = y;
    glViewport(0, 0, (GLsizei) width, 
               (GLsizei) height);               // reset viewport
    glLoadIdentity();                           // clear current matrix
    gluOrtho2D(0.0, (GLdouble) width, 0.0, 
               (GLdouble) height);              // orthographic projection
    glutPostRedisplay();
}

/* idle callback function */
void idle() {
    // move bee
    if (beex >= 0) {                            // until left edge
        beex = beex - 0.1;                      // advance bee 0.1px/cycle
    } else {                                    // when at left
        beex = width - 100;                     // reset bee location to right
    }
    // move wings
    if (down) {                                 // move wings down
        if (beewing >= 15) {
            beewing = beewing - 0.1;            // move wings
        } else {
            down = false;                       // done with down move
        }
    } else {                                    // move wings up
        if (beewing <= 50) {
            beewing = beewing + 0.1;            // move wings
        } else {
            down = true;                        // done with up move
        }
    }

    glutPostRedisplay();
}

/* main function */
int main (int argc, char** argv) {
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
    glMatrixMode(GL_MODELVIEW);                 // activate modelview matrix
    glLoadIdentity();                           // clear current matrix
    gluOrtho2D(0.0, (GLdouble) width, 0.0,  
                (GLdouble) height);             // orthographic projection

    glutIdleFunc(idle);                         // idle callback
    glutReshapeFunc(reshape);                   // reshape callback

    // instructions printed to console
    printf("CPS547: Project 3\n-----------------\n");

    glutMainLoop();                             // enter window display loop
    return(0);
}
