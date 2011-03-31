// -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
// Bruce C. Miller
// 10.28.01
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>

int main (int argc, char **argv) {
    struct timeval begin_time,              // start time
        end_time;                           // end time
    struct timezone tme_zne;                // localization
    long t1, t2;                            // usecs
    gettimeofday(&begin_time, &tme_zne);    // current time
    execlp("/bin/emacs", "emacs&", NULL);   // exec emacs
    gettimeofday(&end_time, &tme_zne);      // current time
    t1 = ((begin_time.tv_sec * 1000000)
          + begin_time.tv_usec);            // assign t1 start usec
    t2 = ((end_time.tv_sec * 1000000)
          + end_time.tv_usec);              // assign t2 end usec
    printf("Microseconds to complete: %d\n",
           t2 - t1);                        // difference in usec
    return(0);
}
