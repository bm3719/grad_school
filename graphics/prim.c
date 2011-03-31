
/********************************************************
 *                                                      *
 *                Simple Skeleton Program               *
 *                                                      *
 ********************************************************/

#include <GL\glut.h>

void init (void)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);
}

void display (void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

	// Draw figures here
	glColor3f(0.415, 0.423, 0.682);
	glPointSize(6.5);
	glBegin(GL_POINTS);
		glVertex2f(300.0, 100.0);
		glVertex2f(300.0, 200.0);
		glVertex2f(200.0, 100.0);
	glEnd();

	glLineWidth(3.0);

	glLineStipple(1, 0x3F);
	glEnable(GL_LINE_STIPPLE);

	glBegin(GL_LINES);
		glVertex2f(20, 100);
		glVertex2f(150, 100);
	glEnd();
/*	glBegin(GL_LINE_LOOP);
		glVertex2f(100, 50);
		glVertex2f(100, 200);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex2f(50, 200);
		glVertex2f(100, 250);
	glEnd();*/

	glDisable(GL_LINE_STIPPLE);
/*
	glBegin(GL_TRIANGLES);
		glVertex2f(10, 100);
		glVertex2f(10, 160);
		glVertex2f(40, 160);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(40, 160);
		glVertex2f(80, 100);
		glVertex2f(40, 40);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(80, 100);
		glVertex2f(40, 40);
		glVertex2f(10, 40);
	glEnd();
*/
	glBegin(GL_QUADS);
		glVertex2f(10, 100);
		glVertex2f(10, 160);
		glVertex2f(40, 160);
		glVertex2f(40, 100);
	glEnd();
	glBegin(GL_QUAD_STRIP);
		glVertex2f(10, 10);
		glVertex2f(10, 40);
		glVertex2f(40, 10);
		glVertex2f(40, 40);
	glEnd();

	glPolygonMode(GL_FRONT, GL_POINT);
	glBegin(GL_TRIANGLES);
		glVertex2f(250, 100);
		glVertex2f(300, 100);
		glVertex2f(300, 150);
	glEnd();

	glPolygonMode(GL_FRONT, GL_LINE);
	glBegin(GL_TRIANGLES);
		glVertex2f(300, 100);
		glVertex2f(350, 100);
		glVertex2f(350, 150);
	glEnd();

	glPolygonMode(GL_FRONT, GL_FILL);
	glBegin(GL_TRIANGLES);
		glVertex2f(350, 100);
		glVertex2f(400, 100);
		glVertex2f(400, 150);
	glEnd();

	glFlush ();
}

int main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize (450, 250);
	glutInitWindowPosition (0, 0);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc (display);
	glViewport (0, 0, (GLsizei) 450, (GLsizei) 250);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0.0, (GLdouble) 450, 0.0, (GLdouble) 250);
	glutMainLoop ();
	return 0;
}
