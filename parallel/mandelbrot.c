/* Parallel Mandelbrot program */

#include "mpi.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define  X_RESN  800                    /* x resolution */
#define  Y_RESN  800                    /* y resolution */

typedef struct complextype {
    float real, imag;
} Compl;

int main (int argc, char *argv[]) {
    int myid, numprocs;
    MPI_Status mpistat;
    Window          win;                /* initialization for a window */
    unsigned int    width, height,      /* window size */
        x, y,                           /* window position */
        border_width,                   /*border width in pixels */
        display_width, display_height,  /* size of screen */
        screen;                         /* which screen */

    char    *window_name = "Mandelbrot Set", *display_name = NULL;
    GC              gc;
    unsigned long   valuemask = 0;
    XGCValues       values;
    Display         *display;
    XSizeHints      size_hints;
    Pixmap          bitmap;
    XPoint          points[800];
    FILE            *fp, *fopen ();
    char            str[100];
    double          start, finish;

    XSetWindowAttributes attr[1];

    /* Mandlebrot variables */
    int i, j, k;
    int xProcs, xTag;

    start = MPI_Wtime();

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    /* connect to Xserver */

    if (myid == 0) {
        if ((display = XOpenDisplay (display_name)) == NULL ) {
            fprintf (stderr, "drawon: cannot connect to X server %s\n", 
                     XDisplayName (display_name) );
            exit (-1);
        }

        /* get screen size */
        screen = DefaultScreen (display);
        display_width = DisplayWidth (display, screen);
        display_height = DisplayHeight (display, screen);

        /* set window size */
        width = X_RESN;
        height = Y_RESN;

        /* set window position */
        x = 0;
        y = 0;

        /* create opaque window */
        border_width = 4;
        win = XCreateSimpleWindow (display, RootWindow (display, screen),
                                   x, y, width, height, border_width, 
                                   BlackPixel (display, screen), WhitePixel (display, screen));

        size_hints.flags = USPosition|USSize;
        size_hints.x = x;
        size_hints.y = y;
        size_hints.width = width;
        size_hints.height = height;
        size_hints.min_width = 300;
        size_hints.min_height = 300;

        XSetNormalHints (display, win, &size_hints);
        XStoreName(display, win, window_name);

        /* create graphics context */
        gc = XCreateGC (display, win, valuemask, &values);

        XSetBackground (display, gc, WhitePixel (display, screen));
        XSetForeground (display, gc, BlackPixel (display, screen));
        XSetLineAttributes (display, gc, 1, LineSolid, CapRound, JoinRound);

        attr[0].backing_store = Always;
        attr[0].backing_planes = 1;
        attr[0].backing_pixel = BlackPixel(display, screen);

        XChangeWindowAttributes(display, win, CWBackingStore | CWBackingPlanes | CWBackingPixel, attr);

        XMapWindow (display, win);
        XSync(display, 0);

    }

    /* Calculate and draw points */
    if (myid == 0) {
        for (i=0; i < X_RESN; i++) {
            for (j=0; j < Y_RESN; j++) {
                xTag = (i * X_RESN) + j;
                xProcs = j % numprocs;
                k = mandel_it(i, j);
                MPI_Send(&k, 1, MPI_INT, xProcs, j, MPI_COMM_WORLD);
                if (myid == 0) {
                    MPI_Recv(&k, 1, MPI_INT, MPI_ANY_SOURCE, j, MPI_COMM_WORLD, &mpistat);
                    if (k == 100) XDrawPoint (display, win, gc, j, i);
                }
            }
        }
    } else {
    }

    if (myid == 0) {
        XFlush (display);
        //sleep (30);
    }

    MPI_Finalize();
    finish = MPI_Wtime();
    printf("Time elapsed (n%i): %5.2f\n", myid, finish - start);

    return 0;
}

int mandel_it(int row, int col) {
    Compl z, c;
    int k;
    float lengthsq, temp;

    z.real = z.imag = 0.0;
    c.real = ((float) col - 400.0)/200.0; /* scale factors for 800 x 800 window */
    c.imag = ((float) row - 400.0)/200.0;
    k = 0;

    do {                                /* iterate for pixel color */
        temp = z.real*z.real - z.imag*z.imag + c.real;
        z.imag = 2.0*z.real*z.imag + c.imag;
        z.real = temp;
        lengthsq = z.real*z.real+z.imag*z.imag;
        k++;
    } while (lengthsq < 4.0 && k < 100);

    return k;
}
