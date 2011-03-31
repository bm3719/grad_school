/* -*- Mode: C++ -*-
 * project2.cpp
 * Bruce C. Miller
 * 11.09.02
 * Displays a 3d rendering of apartment in project1.
 */

#include <GL\glut.h>
#include <stdio.h>
#include <math.h>

#define PI 3.1415926535898                      // the constant PI

// globals
int width = 600;                                // starting width
int height = 500;                               // starting height

// arrays for the object properties here
GLfloat wallambientdiffuse[3] = {0.3, 0.4, 0.2};
GLfloat wallspecularreflectance[3] = {1.0, 1.0, 1.0};
GLfloat bedambientdiffuse[3] = {0.8, 0.85, 0.8};
GLfloat bedspecularreflectance[3] = {1.0, 1.0, 1.0};
GLfloat woodambientdiffuse[3] = {0.3, 0.2, 0.2};
GLfloat woodspecularreflectance[3] = {1.0, 1.0, 1.0};
GLfloat fabricambientdiffuse[3] = {0.4, 0.6, 0.7};
GLfloat fabricspecularreflectance[3] = {1.0, 1.0, 1.0};

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

/* init routines */
void init (void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    // light model array
    GLfloat lightmodel[3] = {0.5, 0.5, 0.5};

    // light model stuff
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightmodel);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    //enable lighting factors
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    //lamplight
    GLfloat ambient0[3] = {0.1, 0.1, 0.1};
    GLfloat diffuse0[3] = {0.8, 0.8, 0.8};
    GLfloat specular0[3] = {0.1, 0.1, 0.1};
    GLfloat position0[4] = {225, 28, 260, 1};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
    glLightfv(GL_LIGHT0, GL_POSITION, position0);
    glEnable(GL_LIGHT0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/* display callback function */
void display (void) {
    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
    glColor3f(0,0,0);
    // eye at front of room, midway in air, looking in
    gluLookAt(125.0, 50.0, 25.0, 125.0, 50.0, 300.0, 0.0, 1.0, 0.0);

    glPushMatrix();                             // room
    glScalef(250.0, 100.0, 300.0);
    glTranslatef(0.5, 0.5, 0.5);                // put edge at origin
    // wall material properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                 wallambientdiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                 wallspecularreflectance);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();                             // bed
    glTranslatef(57.5, 0.0, 150);
    glScalef(100.0, 25.0, 150.0);
    glTranslatef(0.5, 0.5, 0.5);
    // bed material properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                 bedambientdiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                 bedspecularreflectance);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();                             // pillows
    glTranslatef(60.0, 25.0, 275.0);
    glScalef(40.0, 5.0, 20.0);
    glTranslatef(0.5, 0.5, 0.5);
    // pillow material properties (same as bed)
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                 bedambientdiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                 bedspecularreflectance);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(110.0, 25.0, 275.0);
    glScalef(40.0, 5.0, 20.0);
    glTranslatef(0.5, 0.5, 0.5);
    // pillow material properties (same as bed)
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                 bedambientdiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                 bedspecularreflectance);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();                             // couch seat
    glTranslatef(0.0, 0.0, 100.0);
    glScalef(50.0, 15.0, 100.0);
    glTranslatef(0.5, 0.5, 0.5);
    // couch material properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                 fabricambientdiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                 fabricspecularreflectance);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();                             // couch back
    glTranslatef(0.0, 15.0, 100.0);
    glScalef(15.0, 20.0, 100.0);
    glTranslatef(0.5, 0.5, 0.5);
    // couch material properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                 fabricambientdiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                 fabricspecularreflectance);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();                             // desktop
    glTranslatef(200.0, 20.0, 200.0);
    glScalef(50.0, 3.0, 80.0);
    glTranslatef(0.5, 0.5, 0.5);
    // desktop material properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                 woodambientdiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                 woodspecularreflectance);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();                             // desk legs (front right)
    glTranslatef(200.0, 0.0, 200.0);
    glScalef(2.0, 20.0, 2.0);
    glTranslatef(0.5, 0.5, 0.5);
    // desk leg material properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                 woodambientdiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                 woodspecularreflectance);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();                             // desk legs (front left)
    glTranslatef(248.0, 0.0, 200.0);
    glScalef(2.0, 20.0, 2.0);
    glTranslatef(0.5, 0.5, 0.5);
    // desk leg material properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                 woodambientdiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                 woodspecularreflectance);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();                             // desk legs (back right)
    glTranslatef(200.0, 0.0, 278.0);
    glScalef(2.0, 20.0, 2.0);
    glTranslatef(0.5, 0.5, 0.5);
    // desk leg material properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                 woodambientdiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                 woodspecularreflectance);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();                             // desk legs (back left)
    glTranslatef(248.0, 0.0, 278.0);
    glScalef(2.0, 20.0, 2.0);
    glTranslatef(0.5, 0.5, 0.5);
    // desk leg material properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                 woodambientdiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                 woodspecularreflectance);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();                             // lamp
    glTranslatef(225.0, 25.0, 260.0);
    glScalef(7.0, 20.0, 7.0);
    glTranslatef(0.5, 0.5, 0.5);
    glRotatef(270, 1, 0, 0);
    // lamp material properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                 bedambientdiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                 bedspecularreflectance);
    glutSolidCone(1, 1, 10, 10);                // lamp base
    glPopMatrix();
    glPushMatrix();                             // lamp
    glTranslatef(225.0, 25.0, 260.0);
    glScalef(5.0, 10.0, 5.0);
    glTranslatef(0.5, 0.5, 0.5);
    glRotatef(90, 1, 0, 0);
    // lampbase material properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                 woodambientdiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                 woodspecularreflectance);
    glutSolidCone(1, 1, 10, 10);                // lamp shade
    glPopMatrix();

//  glutSwapBuffers();                          // swap the buffers
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

    glMatrixMode(GL_PROJECTION);
    glutPostRedisplay();

    glFrustum(-50.0, 50.0, -50.0, 50.0, 75.0, 400.0);
}

/* main function */
int main (int argc, char** argv) {
    glutInit(&argc, argv);                      // init opengl
    glutInitDisplayMode(GLUT_SINGLE|            // double buffer, RBG palette
                        GLUT_RGB|GLUT_DEPTH);   // and depth buffer
    glutInitWindowSize(width, height);          // window frame size
    glutInitWindowPosition(0, 0);               // top-left origin
    glutCreateWindow("CPS547: Project 4");      // title bar text
    init();                                     // some initialization routines
    glutDisplayFunc(display);                   // display callback
    glutReshapeFunc(reshape);                   // reshape callback

    // instructions printed to console
    printf("CPS547: Project 4\n-----------------\n");

    glutMainLoop();                             // enter window display loop
    return(0);
}
