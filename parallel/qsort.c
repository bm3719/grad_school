// qsort.c
// Bruce C. Miller

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#define MAX_SIZE 250000

int intcompare(const void *p1, const void *p2) {
    int i = *((int *)p1);
    int j = *((int *)p2);
    if (i > j) {
        return(1);
    }
    if (j > i) {
        return(-1);
    }
    return(0);
}

int main (int argc, char * argv[]) {

    int i, j, numdigits;
    int *arr;
    size_t nelems;
    struct timeval begin_time,              // beginning
        end_time;                           // end
    struct timezone tme_zne;                // localization
    long t1, t2;                            // usecs
    gettimeofday(&begin_time, &tme_zne);    // current time
    if (argc != 2) {
        printf("usage qsort num\n");
        exit(0);
    }
    numdigits = atoi(argv[1]);
    arr = malloc(sizeof (int) * numdigits);
    nelems = sizeof(arr) / sizeof(int);
    if (numdigits > MAX_SIZE || numdigits < 1) {
        printf("argv[1] must be number between 1 and 250000\n");
        exit(1);
    }

    for (j = 0; j < numdigits; j++) {
        arr[j] = rand();
    }
    qsort((void *) arr, nelems, sizeof (int), intcompare);
    gettimeofday(&end_time, &tme_zne);      // current time
    t1 = ((begin_time.tv_sec * 1000000)
          + begin_time.tv_usec);            // assign t1 start usec
    t2 = ((end_time.tv_sec * 1000000)
          + end_time.tv_usec);              // assign t2 end usec
    printf("Child Complete in prcswpn\n");
    printf("Microseconds to complete %d\n",
           t2 - t1);                        // difference in usec
    return (0);
}


