// pqsort.c
// Bruce C. Miller
// Parallel Quick Sort

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <sys/types.h>

int partition(int *A, int p, int r) {
    int x = A[p], j, temp;
    r -= 1;
    for(j = p + 1; j < r; j++) {
        if(A[j] >= x) {
            temp = A[j];
            A[j] = A[r];
            A[r] = temp;    
            r--;
            j--;
        }
    }
    if (A[p] > A[r]) {
        temp = A[r];
        A[r] = A[p];
        A[p] = temp;
        return r;
    } else {
        temp = A[r - 1];
        A[r - 1] = A[p];
        A[p] = temp;
        return r - 1;
    }
} 

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
    int myid,                           // proc id
        numprocs,                       // # of procs in cluster        
        numdigits,                      // nelem in data arr
        i, j, k,
        levels,                         // # of tree levels
        numelem,
        pivotindex,                     // index of pivot
        l2size;                         // size of incoming list to be merged
    int * arr,                          // data array
        * list1, * list2,               // subarrays    
        arrsize;                        // size of array to be sent
    size_t nelems;

    struct timeval begin_time,          // beginning
        end_time;                       // end
    struct timezone tme_zne;            // localization
    long t1, t2;                        // usecs             
        
    MPI_Status *status;                 // send/recv status

    gettimeofday(&begin_time, &tme_zne);//current time  
        
    // setup MPI stuff
    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    // error handling
    if (argc != 2) {
        printf("usage: pqsort num\n");
        exit(1);
    }
    if (numprocs != 2 && numprocs != 4 && 
        numprocs != 1 && numprocs != 8) {
        printf("need 1, 2, 4, or 8 processors for tree\n");
        exit(1);
    }

    numdigits = atoi(argv[1]);

    // more error handling
    if (numdigits < 1) {
        printf("argv[1] must be a number greater than 1\n");
        exit(1);        
    }

    // number of levels in the tree 
    levels = (int) floor((log(numprocs) / log(2)) + 1);

    // create array on root node
    if (myid == 0) {
        // alloc array size N
        arr = malloc(sizeof(int) * numdigits);
                
        for (i = 0; i < numdigits; i++) {
            arr[i] = rand();            // populate array
        }
    }
        
    // algorithm for partitioning data and sending to subnodes in tree
    // calls sequential qsort at leaves
    for(i = levels - 1 ; i >= 0; i--) { // iterate on num levels remaining
        if (i == 3) {                   // top level
            if (!(myid % 2) && !(myid % 4) && !(myid % 8)) { // node 0
                numelem = numdigits;
                pivotindex = partition(arr, 0, numelem);
                list1 = malloc(sizeof(int) * pivotindex + 1);
                list2 = malloc(sizeof(int) * numelem - (pivotindex + 1));
                k = 0;
                for(j = 0; j < numelem; j++) {
                    // list1 len = pivotindex + 1
                    // list1 len = numdigits - pivotindex - 1
                    if (j <= pivotindex) {
                        list1[j] = arr[j];
                    } else {
                        list2[k] = arr[j];
                        k++;
                    }
                }
                // store length of list1
                numdigits = pivotindex + 1;

                if (!(myid % 2) && !(myid % 4) && !(myid % 8)) {
                    // send list2 size to rchild
                    arrsize = numelem - (pivotindex + 1);
                    MPI_Send(&arrsize, 1, 
                             MPI_INT, myid + 4, 0, MPI_COMM_WORLD);
        
                    // send list2 to rchild
                    MPI_Send(list2, numelem - (pivotindex + 1), 
                             MPI_INT, myid + 4, 0, MPI_COMM_WORLD);
                    // free arr and set it to list1
                    free(arr);
                    arr = malloc(sizeof(int) * numdigits);
                    for(j = 0; j < numdigits; j++) {
                        arr[j] = list1[j];
                    }
                }
            }
        } else if (i == 2) {
            if (!(myid % 2) && !(myid % 4)) { // nodes mult of 4
                if (myid % 8) {         // node 4
                    // get sublist length
                    MPI_Recv(&arrsize, 1, MPI_INT, myid - 4,
                             0, MPI_COMM_WORLD, status);
                    arr = malloc(sizeof(int) * arrsize);
                    numelem = arrsize;
                    printf("arrsize = %d\n", arrsize);      
                    // get sublist
                    MPI_Recv(arr, numelem, MPI_INT, myid - 4,
                             0, MPI_COMM_WORLD, status);
                    numdigits = numelem;
                } else {
                    numelem = numdigits;
                }
                pivotindex = partition(arr, 0, numelem);
                list1 = malloc(sizeof(int) * pivotindex + 1);
                list2 = malloc(sizeof(int) * numelem - (pivotindex + 1));
                k = 0;
                for(j = 0; j < numelem; j++) {
                    if (j <= pivotindex) {
                        list1[j] = arr[j];
                    } else {
                        list2[k] = arr[j];      
                        k++;
                    }
                }
                // store length of list1
                numdigits = pivotindex + 1;

                if (!(myid % 2) && !(myid % 4)) {
                    // send list2 size to rchild
                    arrsize = numelem - (pivotindex + 1);
                    MPI_Send(&arrsize, 1, 
                             MPI_INT, myid + 2, 0, MPI_COMM_WORLD);
        
                    // send list2 to rchild
                    MPI_Send(list2, numelem - (pivotindex + 1), 
                             MPI_INT, myid + 2, 0, MPI_COMM_WORLD);
                    //free arr and set it to list1
                    free(arr);
                    arr = malloc(sizeof(int) * numdigits);
                    for(j = 0; j < numdigits; j++) {
                        arr[j] = list1[j];
                    }
                }
            }
        } else if (i == 1) {
            if (!(myid % 2)) {          // nodes of even id
                if (!(myid % 2) && (myid % 4)) { // nodes 2, 6
                    // get sublist length
                    MPI_Recv(&arrsize, 1, MPI_INT, myid - 2,
                             0, MPI_COMM_WORLD, status);
                    arr = malloc(sizeof(int) * arrsize);
                    numelem = arrsize;
                    printf("arrsize = %d\n", arrsize);      
                    // get sublist
                    MPI_Recv(arr, numelem, MPI_INT, myid - 2,
                             0, MPI_COMM_WORLD, status);
                    numdigits = numelem;
                } else {
                    numelem = numdigits;
                }
                pivotindex = partition(arr, 0, numelem);
                list1 = malloc(sizeof(int) * pivotindex + 1);
                list2 = malloc(sizeof(int) * numelem - (pivotindex + 1));
                k = 0;
                for(j = 0; j < numelem; j++) {
                    if (j <= pivotindex) {
                        list1[j] = arr[j];
                    } else {
                        list2[k] = arr[j];      
                        k++;
                    }
                }
                
                // store length of list1
                numdigits = pivotindex + 1;
                                
                if (!(myid % 2)) {
                    // send list2 size to rchild
                    arrsize = numelem - (pivotindex + 1);
                    MPI_Send(&arrsize, 1, 
                             MPI_INT, myid + 1, 0, MPI_COMM_WORLD);
        
                    // send list2 to rchild
                    MPI_Send(list2, numelem - (pivotindex + 1), 
                             MPI_INT, myid + 1, 0, MPI_COMM_WORLD);
                    //free arr and set it to list1
                    free(arr);
                    arr = malloc(sizeof(int) * numdigits);
                    for(j = 0; j < numdigits; j++) {
                        arr[j] = list1[j];
                    }
                }
            }
        } else {                        // i == 0
            if (myid % 2) {
                // get sublist length
                MPI_Recv(&arrsize, 1, MPI_INT, myid - 1,
                         0, MPI_COMM_WORLD, status);
                arr = malloc(sizeof(int) * arrsize);
                numelem = arrsize;
                // get sublist
                MPI_Recv(arr, numelem, MPI_INT, myid - 1,
                         0, MPI_COMM_WORLD, status);
                numdigits = numelem;
            } else {
                numelem = numdigits;
            }

            if (numelem > 1) {
                nelems = sizeof (int) * numelem;
                qsort( (void *)arr, numdigits, sizeof(int),
                       intcompare);
            }
            // testing
            for (k = 0; k < numdigits; k++) {
                printf("node %d: arr[%d]: %d\n", myid, k, arr[k]);
            }
        }
    }

    // remerge up the tree
    for(i = 0; i <= levels - 1; i++) {  // iterate on num levels above
        if (i == 0) {
            // odd nodes, send data
            if (myid % 2) {             // odd num nodes
                // send arr size to parent
                MPI_Send(&arrsize, 1, 
                         MPI_INT, myid - 1, 0, MPI_COMM_WORLD);

                // send arr to rchild
                MPI_Send(arr, arrsize, 
                         MPI_INT, myid - 1, 0, MPI_COMM_WORLD);
            }
        } else if (i == 1) {
            // if even, recv
            if (!(myid % 2)) {
                MPI_Recv(&l2size, 1, MPI_INT, myid + 1,
                         0, MPI_COMM_WORLD, status);
                list2 = malloc(sizeof(int) * l2size);
                // get sublist
                MPI_Recv(list2, l2size, MPI_INT, myid + 1,
                         0, MPI_COMM_WORLD, status);
                // calc numdigits in both arrays
                arrsize = numdigits;
                numdigits = arrsize + l2size;

                // copy arr back to list1
                free(list1);
                list1 = malloc(sizeof(int) * arrsize);
                for (j = 0; j < arrsize; j++) {
                    list1[j] = arr[j];
                }
                // free arr and resize it to numdigits
                free(arr);
                arr = malloc(sizeof(int) * numdigits);
                // combine the arrays
                k = 0;
                for (j = 0; j < numdigits; j++) {
                    if (j < arrsize) {
                        arr[j] = list1[j];
                    } else {
                        arr[j] = list2[k];
                        k++;
                    }
                }
                arrsize = numdigits;
            }
            // 2 & 6 send
            if (!(myid % 2) && (myid % 4)) {
                // send arr size to parent
                MPI_Send(&arrsize, 1,
                         MPI_INT, myid - 2, 0, MPI_COMM_WORLD);
 
                // send arr to rchild
                MPI_Send(arr, arrsize,
                         MPI_INT, myid - 2, 0, MPI_COMM_WORLD);
            }
        } else if (i == 2) {
            // 0 & 4 recv
            if (!(myid % 2) && !(myid % 4)) {
                MPI_Recv(&l2size, 1, MPI_INT, myid + 2,
                         0, MPI_COMM_WORLD, status);
                list2 = malloc(sizeof(int) * l2size);
                // get sublist
                MPI_Recv(list2, l2size, MPI_INT, myid + 2,
                         0, MPI_COMM_WORLD, status);
                // calc numdigits in both arrays
                arrsize = numdigits;
                numdigits = arrsize + l2size;
 
                // copy arr back to list1
                free(list1);
                list1 = malloc(sizeof(int) * arrsize);
                for (j = 0; j < arrsize; j++) {
                    list1[j] = arr[j];
                }
                // free arr and resize it to numdigits
                free(arr);
                arr = malloc(sizeof(int) * numdigits);
                // combine the arrays
                k = 0;
                for (j = 0; j < numdigits; j++) {
                    if (j < arrsize) {
                        arr[j] = list1[j];
                    } else {
                        arr[j] = list2[k];
                        k++;
                    }
                }
                arrsize = numdigits;

            }
            // 4 send
            if (!(myid % 2) && !(myid % 4) && (myid % 8)) {
                // send arr size to parent
                MPI_Send(&arrsize, 1,
                         MPI_INT, myid - 4, 0, MPI_COMM_WORLD);
 
                // send arr to rchild
                MPI_Send(arr, arrsize,
                         MPI_INT, myid - 4, 0, MPI_COMM_WORLD);

            }
        } else {                        // i == 3
            // 0 recvs 
            if (!(myid % 2) && !(myid % 4) && !(myid % 8)) {
                MPI_Recv(&l2size, 1, MPI_INT, myid + 4,
                         0, MPI_COMM_WORLD, status);
                list2 = malloc(sizeof(int) * l2size);
                // get sublist
                MPI_Recv(list2, l2size, MPI_INT, myid + 4,
                         0, MPI_COMM_WORLD, status);
                // calc numdigits in both arrays
                arrsize = numdigits;
                numdigits = arrsize + l2size;
 
                // copy arr back to list1
                free(list1);
                list1 = malloc(sizeof(int) * arrsize);
                for (j = 0; j < arrsize; j++) {
                    list1[j] = arr[j];
                }
                // free arr and resize it to numdigits
                free(arr);
                arr = malloc(sizeof(int) * numdigits);
                // combine the arrays
                k = 0;
                for (j = 0; j < numdigits; j++) {
                    if (j < arrsize) {
                        arr[j] = list1[j];
                    } else {   
                        arr[j] = list2[k];
                        k++;
                    }
                }
                arrsize = numdigits;
            }
        }
    }
    // node 0 outputs result
    if (myid == 0) {
        printf("final results:\n");
        for (j = 0; j < numdigits; j++) {
            printf("arr[%d]: %d\n", j, arr[j]);
        }
    }

    gettimeofday(&end_time, &tme_zne);  // current time
    t1 = ((begin_time.tv_sec * 1000000)
          + begin_time.tv_usec);        // assign t1 start usec
    t2 = ((end_time.tv_sec * 1000000)
          + end_time.tv_usec);          // assign t2 end usec
    printf("Microseconds to complete %d\n",
           t2 - t1);                    // difference in usec 

    MPI_Finalize();
    return (0);
}
