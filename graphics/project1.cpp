/* -*- Mode: C++ -*-
 * project1.cpp
 * Bruce C. Miller
 * A 2-dimensional floorplan and furniture layout of the my apt.
 */

#include <GL\glut.h>
#include <stdio.h>
#include <math.h>

#define PI 3.1415926535898                  // the constant PI

/* init routines */
void init(void) {
    glClearColor (0.0, 0.0, 0.0, 0.0);      // black background
    glShadeModel (GL_FLAT);                 // a uniform color shade model
}

/* draw a circle from a bunch of short lines */
void drawCircle(GLfloat xpos, GLfloat ypos, GLfloat radius) {
    GLfloat vectorX, vectorY;
    GLfloat vectorY1 = ypos + radius;
    GLfloat vectorX1 = xpos;
    glBegin(GL_LINE_STRIP);                 // trace circumference
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
    glRasterPos2f(x, y);                    // set text position
    // traverse string
    for (int j = 0; s[j] !=  '\0'; j++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, s[j]);
    }
}

/* draw a quad given the lower left and upper right vertex */
void drawQuad(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
    glBegin(GL_QUADS);                              
    glVertex2f(x1, y1);                     // bottom left
    glVertex2f(x2, y1);                     // bottom right
    glVertex2f(x2, y2);                     // top right
    glVertex2f(x1, y2);                     // top left
    glEnd();
}

/* display callback function */
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);           // clear background                             

    glColor3f(.11, .11, .11);               // background color for floor
    glLineWidth(2.0);                       // 2px wall thinkness

    // solid coloring for floor
    glPolygonMode(GL_FRONT, GL_FILL);       // switch to fill mode
    drawQuad(25, 25, 575, 475);             // solid filled quad for floor
    // end floor

    // walls
    glPolygonMode(GL_FRONT, GL_LINE);       // switch to outline mode
    glColor3f(.2667, .2745, .5412);         // gray blue for walls
    drawQuad(25, 25, 575, 475);             // outline of foundation
    glBegin(GL_LINES);
    glVertex2f(25, 225);                    // computer/furnace divider
    glVertex2f(225, 225);           
    glVertex2f(275, 375);                   // computer/stairs divider
    glVertex2f(275, 225);
    glVertex2f(325, 375);                   // stairs/bedroom divider
    glVertex2f(325, 225);
    glVertex2f(375, 175);                   // bedroom/storage divider
    glVertex2f(575, 175);
    glEnd();
    glBegin(GL_LINE_STRIP);                 // hallway closet
    glVertex2f(320, 425);
    glVertex2f(325, 425);
    glVertex2f(325, 475);
    glVertex2f(275, 475);
    glVertex2f(275, 425);
    glVertex2f(280, 425);
    glEnd();
    glBegin(GL_LINE_STRIP);                 // bedroom closet
    glVertex2f(325, 220);
    glVertex2f(325, 225);
    glVertex2f(275, 225);
    glVertex2f(275, 175);
    glVertex2f(325, 175);
    glVertex2f(325, 180);
    glEnd();
    glBegin(GL_LINE_STRIP);                 // bathroom walls
    glVertex2f(275, 175);
    glVertex2f(275, 75);
    glVertex2f(425, 75);
    glVertex2f(425, 175);
    glEnd();
    glBegin(GL_LINE_STRIP);                 // computer closet
    glVertex2f(75, 370);
    glVertex2f(75, 375);
    glVertex2f(25, 375);
    glVertex2f(25, 300);
    glVertex2f(75, 300);
    glVertex2f(75, 305);
    glEnd();
    // end walls

    glLineWidth(1.0);                       // 1px line for furniture/stairs 

    // stairs
    glBegin(GL_LINES);
    for (float steps = 373.0; steps > 225; steps = steps - 15) {
        glVertex2f(275, steps);
        glVertex2f(325, steps);
    }
    glEnd();
    // end stairs
        
    // furniture
    glColor3f(.31, .31, .3599);             // slightly off grey for furniture
    drawQuad(415, 176, 505, 310);           // bed
    drawQuad(50, 150, 125, 200);            // furnace
    drawQuad(75, 26, 175, 75);              // oil tank
    drawQuad(125, 226, 220, 275);           // computer dresser
    drawQuad(27, 376, 75, 473);             // computer desk 1
    drawQuad(150, 425, 225, 473);           // computer desk 2
    drawQuad(540, 190, 574, 250);           // bedroom desk 
    drawQuad(400, 425, 475, 473);           // bedroom dresser
    drawQuad(327, 275, 375, 375);           // couch
    drawQuad(545, 275, 574, 390);           // entertainment center
    drawQuad(396, 76, 424, 95);             // toilet
    drawQuad(350, 76, 390, 110);            // sink
    glBegin(GL_LINE_LOOP);                  // shower
    glVertex2f(277, 76);
    glVertex2f(340, 76);
    glVertex2f(340, 120);
    glVertex2f(300, 173);
    glVertex2f(277, 173);
    glEnd();
    drawCircle(410.0, 110.0, 12.0);         // toilet seat
    // end furniture

    // text labels
    glColor3f(.2775, .2914, .3592);         // gray light blue for text
    drawString(150, 480,                    // main title
               "Bruce Miller Estate, 515 E. Orange St.");      
    drawString(448, 240, "bed");            // bed label
    drawString(330, 320, "couch");          // couch label
    drawString(551, 330, "tv");             // tv label
    drawString(541, 218, "desk");           // bedroom desk label
    drawString(410, 446, "dresser");        // bedroom dresser label
    drawString(354, 90, "sink");            // sink label
    drawString(285, 115, "shower");         // shower label
    drawString(35, 425, "desk");            // computer desk 1 label
    drawString(170, 446, "desk");           // computer desk 2 label
    drawString(144, 247, "dresser");        // computer dresser label
    drawString(55, 172, "furnance");        // furnace label
    drawString(92, 48, "oil tank");         // oil tank label
    drawString(285, 360, "exit");           // exit label
    drawString(472, 100, "STORAGE");        // storage room label
    drawString(425, 350, "BEDROOM");        // bedroom label
    drawString(110, 350, "COMPUTER ROOM");  // computer room label
    drawString(100, 115, "FURANCE ROOM");   // furnance room label
    // end text labels

    glFlush();                              // flush cmd buffer
}

/* main function */
int main(int argc, char** argv) {
    glutInit(&argc, argv);                  // init opengl
    glutInitDisplayMode(GLUT_SINGLE |
                         GLUT_RGB);         // single display, RBG palette
    glutInitWindowSize(600, 500);           // window frame size
    glutInitWindowPosition(0, 0);           // top-left origin
    glutCreateWindow("CPS547: Project 1");  // title bar text
    init();                                 // some initialization routines
    glutDisplayFunc(display);               // display callback
    glViewport(0, 0, (GLsizei) 600, 
                (GLsizei) 500);             // 600x500 viewport
    glMatrixMode(GL_PROJECTION);            // activate projection matrix
    glLoadIdentity();                       // clear current matrix
    gluOrtho2D(0.0, (GLdouble) 600, 0.0, 
                (GLdouble) 500);            // 600x500 orthographic projection
    printf("CPS547: Project 1\n-----------------\n");
    // message for those with dim/cheap monitors
    printf("If the display is too dark, adjust brightness and gamma.\n");
    glutMainLoop();                         // enter window display loop
    return(0);
}
