// Bruce C. Miller
// msort.c - perform parallel merge sort on 3, 7, or 15 node tree

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <sys/types.h>
#define MAX_SIZE 250000
#define TRUE 1
#define FALSE 0

void mergeSort(int[], int[], int);
void m_sort(int[], int[], int, int);
void merge(int[], int[], int, int, int);

int *arr, *tmp1, *tmp2, *mtmp;          // data and temp array

int main(int argc, char * argv[]) {
    int myid,                           // proc id
        numprocs,                       // # of procs in cluster
        level,                          // tree level node is at
        numdigits,                      // # arr elements
        leafdigits,                     // # elements in leaf set
        i, j,parent,leftchild,rightchild,
        divisor;                        // # leafnodes
    int leafnode;                       // is myid = leafnode
    MPI_Status  * status;               // send/recv status

    struct timeval begin_time,          // beginning
        end_time;                       // end
    struct timezone tme_zne;            //localization
    long t1, t2;                        //usecs
    gettimeofday(&begin_time, &tme_zne);//current time

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    // error handling
    if (argc != 2) {
        printf("usage: msort num\n");
        exit(0);
    }
    if (numprocs != 3 && numprocs != 7 && numprocs != 15) {
        printf("need 3, 7, or 15 processors for tree\n");
        exit(0);
    }

    numdigits = atoi(argv[1]);
    // create array on root node
    if (myid == 0) {
        if (numdigits > MAX_SIZE || numdigits < 1) {
            printf("argv[1] must be number between 1 and 250000\n");
            exit(1);
        }
        // alloc array of size N
        arr = malloc(sizeof (int) * numdigits);
        tmp1 = malloc(sizeof (int) * numdigits);

        for (j = 0; j < numdigits; j++) {
            arr[j] = rand();            // populate array
        }

        divisor = ( numprocs + 1 ) / 2;
        leafdigits = numdigits/divisor;

        free(tmp1);
        tmp1 = malloc(sizeof (int) * leafdigits);

        // send left hand leaf node sets of data ****
        for (j = 0; j < divisor / 2; j++) {
            for (i = 0; i < leafdigits; i++) { // 0 - (n/2)-1
                tmp1[i] = arr[i + (j*leafdigits)];
            }
            MPI_Send(&tmp1, leafdigits, MPI_INT, 1, 0, MPI_COMM_WORLD);
        }

        // send right hand leaf node sets of data
        for (j = 0; j < divisor / 2; j++) {
            for (i = 0; i < leafdigits; i++) {
                tmp1[i] = arr[ i
                               + (j*leafdigits)
                               + (numdigits / 2) ];
            }
            MPI_Send(&tmp1, leafdigits, MPI_INT, 2, 0, MPI_COMM_WORLD);
        }

        free(tmp1);
        free(tmp2);
        tmp1 = malloc(sizeof(int) * numdigits/2);
        tmp2 = malloc(sizeof(int) * numdigits/2);
        // now tmp1 and tmp2 are both size N/2
        MPI_Recv(&tmp1, leafdigits, MPI_INT, 1, 0, MPI_COMM_WORLD, status);
        MPI_Recv(&tmp2, leafdigits, MPI_INT, 2, 0, MPI_COMM_WORLD, status);
        // *** need to merge the arrays still
        for (j = 0; j < numdigits/2; j++)
        {
            arr[j] = tmp1[j];
        }
        for (j = 0; j < numdigits/2; j++)
        {
            arr[j + (numdigits/2)]= tmp2[j];
        }
        mtmp = malloc(sizeof(int) * numdigits);
    } else {                            // myid != 0
        divisor = ( numprocs + 1 ) / 2;
        for (j = 0; j < numprocs - divisor; j++) {
            if (myid == j) {
                leafnode = FALSE;
            }
        }

        if (!leafnode)  {               // if not a leaf node
            level= (int)floor(log(myid+1)/log(2));
            parent = ((int)ceil(myid/2))-1;
            leafdigits = numdigits/divisor;
            for(i = 0; i < divisor / (2 * level); i++) {
                MPI_Recv(&tmp1, leafdigits, MPI_INT, parent, 0, MPI_COMM_WORLD, status);
                if(i<(divisor/(4*level)))
                    MPI_Send(&tmp1, leafdigits, MPI_INT, leftchild, 0, MPI_COMM_WORLD);
                else
                    MPI_Send(&tmp1, leafdigits, MPI_INT, rightchild, 0, MPI_COMM_WORLD);
            }
            free(tmp1);
            free(tmp2);
            tmp1 = malloc(sizeof(int) * (leafdigits * 4/level));
            tmp2 = malloc(sizeof(int) * (leafdigits * 4/level));
            MPI_Recv(&tmp1, leafdigits, MPI_INT, leftchild, 0, MPI_COMM_WORLD, status);
            MPI_Recv(&tmp2, leafdigits, MPI_INT, rightchild, 0, MPI_COMM_WORLD, status);

            free(arr);
            arr = malloc(sizeof(int) * (leafdigits * 4/level) * 2);
            for(j = 0; j < (leafdigits * 4/level); j++) {
                arr[j] = tmp1[j];
            }
            for(j = 0; j < (leafdigits * 4/level); j++) {
                arr[(leafdigits * 4/level) + j] = tmp2[j];
            }
            mtmp = malloc(sizeof(int) * (leafdigits * 4/level) * 2);
            //mergeSort(arr, mtmp, (leafdigits * 4/level) * 2 );
            MPI_Send(&arr, leafdigits * 4/level * 2, MPI_INT, parent, 0, MPI_COMM_WORLD);
        } else {                        // if leaf node
            leafdigits = numdigits/divisor;
            level= (int)floor(log(myid+1)/log(2));
            parent = ((int)ceil((myid+1)/2))-1;
            free(tmp1);
            tmp1 = malloc(sizeof(int) * leafdigits);
            MPI_Recv(&tmp1,leafdigits,MPI_INT,parent, 0, MPI_COMM_WORLD, status);
            mtmp= malloc(sizeof(int) * leafdigits);
            printf("hi\n");
            //mergeSort(tmp1, mtmp, leafdigits);
            printf("myid = %d, parents = %d\n", myid, parent);
            MPI_Send(&tmp1, leafdigits, MPI_INT, parent, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();

    gettimeofday(&end_time, &tme_zne);  // current time
    t1 = ((begin_time.tv_sec * 1000000)
          + begin_time.tv_usec);        // assign t1 start usec
    t2 = ((end_time.tv_sec * 1000000)
          + end_time.tv_usec);          //assign t2 end usec
    printf("Child Complete in prcswpn\n");
    printf("Microseconds to complete %d\n",
           t2 - t1);                    // difference in usec
    return (0);
}

// sequential merge sort code from
// http://linux.wku.edu/~lamonml/algor/sort/merge.html
void mergeSort(int numbers[], int temp[], int array_size) {
    m_sort(numbers, temp, 0, array_size - 1);
}

void m_sort(int numbers[], int temp[], int left, int right) {
    int mid;

    if (right > left) {
        mid = (right + left) / 2;
        m_sort(numbers, temp, left, mid);
        m_sort(numbers, temp, mid+1, right);
        merge(numbers, temp, left, mid+1, right);
    }
}

void merge(int numbers[], int temp[], int left, int mid, int right) {
    int i, left_end, num_elements, tmp_pos;

    left_end = mid - 1;
    tmp_pos = left;
    num_elements = right - left + 1;

    while ((left <= left_end) && (mid <= right)) {
        if (numbers[left] <= numbers[mid]) {
            temp[tmp_pos] = numbers[left];
            tmp_pos = tmp_pos + 1;
            left = left +1;
        } else {
            temp[tmp_pos] = numbers[mid];
            tmp_pos = tmp_pos + 1;
            mid = mid + 1;
        }
    }

    while (left <= left_end) {
        temp[tmp_pos] = numbers[left];
        left = left + 1;
        tmp_pos = tmp_pos + 1;
    } while (mid <= right) {
        temp[tmp_pos] = numbers[mid];
        mid = mid + 1;
        tmp_pos = tmp_pos + 1;
    }

    for (i=0; i <= num_elements; i++) {
        numbers[right] = temp[right];
        right = right - 1;
    }
}
