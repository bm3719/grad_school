/*Merge Sort*/
/*Note:  The main processor takes the last section and performs the sort itself.*/

#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>

#define FALSE 0;
#define TRUE !FALSE; 

int my_id;      /* process id */
int p;          /*number of processes */
int A[15500];
int B[15500];
int ProblemSize;

/****Random Number Generator****/
/* from Numerical Recipes */
#define M 714025
#define IA 1366
#define IC 150889

float ran(long *idum)  {
    static long iy,ir[98];
    static int iff=0;
    int j;
    if (*idum < 0 || iff == 0) {
             
        iff=1;
             
        if ((*idum=(IC-(*idum)) % M) < 0) *idum = -(*idum);
             
        for (j=1;j<=97;j++) {
                     
            *idum=(IA*(*idum)+IC) % M;
                     
            ir[j]=(*idum);
             
        }
             
        *idum=(IA*(*idum)+IC) % M;
             
        iy=(*idum);
    }
    j=1 + 97.0*iy/M;
    if (j > 97 || j < 1) exit(0);
    iy=ir[j];
    *idum=(IA*(*idum)+IC) % M;
    ir[j]=(*idum);
    return (float) iy/M;
}
void PrintArray (int *Array, int Size) {
    int Counter;
    for (Counter=0;Counter<Size;Counter++) {
        printf("%i ",A[Counter]);
    }
    printf("\n\n");
}


/****Merge Sort Synchronous****/
/*Taken from example on web page.*/
void merge(int Left, int Right, int Size, int MaxSide) {
    int i,rlim,llim;
    llim = Right;
    if( (Right+Size)<MaxSide) rlim = Right + Size;
    else               
        rlim = MaxSide;
    for (i=Left;i<rlim;i++) {
        if (Right>=rlim) B[i] = A[Left++];
        else if (Left >= llim) B[i]=A[Right++];
        else if (A[Left]<A[Right]) B[i] = A[Left++];
        else B[i]=A[Right++];
    }
}

void MergeSort(int ArraySize, int Size) {
    int i;
//if (my_id==0){
//  printf("in:\n"); 
//  PrintArray(&A[0],ArraySize);}
    while(Size<=ArraySize) {
        for(i=0;i<ArraySize;i+=2*Size)
            merge(i,i+Size,Size, ArraySize);
        for(i=0;i<ArraySize;i++) A[i]=B[i];
        Size=Size*2;
    }
}

int TestArray(int *ArrayToTest, int Size) {
    int Counter;
    int ReturnValue = TRUE;
    for (Counter=0;Counter<(Size-1);Counter++) {
        if (*(ArrayToTest+Counter)>*(ArrayToTest+Counter+1)){ReturnValue = FALSE;}
    }
    return ReturnValue;
}

/*Copy part of a dynamic array into another array*/
void CopyArrayPart(int *Source, int *Destination, int Size, int StartPoint) {
    int Counter1;
    for (Counter1==0;Counter1<Size;Counter1++) {
        *(Destination+Counter1) = *(Source+Counter1+StartPoint);}
}

/****Main Function****/
main(int argc, char **argv) {
    long RandomSeed;
    int Iterations;
    int Counter1;
    int *TempPointer;
    int SubSize, MainSubSize;
    int Source, Counter2, Counter3;
    MPI_Status Status;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &my_id );
    MPI_Comm_size( MPI_COMM_WORLD, &p );

    /*Read in arguments*/
    if (argc < 3) {
        if(my_id==0) {
            printf("Illegal number of arguments.\n");
            printf("Needs problem size and RandomSeed.\n");
        }
        goto EXIT;
    }
    else {
        sscanf(argv[1],"%i",&ProblemSize);
        sscanf(argv[2],"%l",&RandomSeed);
    }
    MPI_Barrier(MPI_COMM_WORLD); 
    /*Master generates work and sends it to slaves*/
    /*Generate Work*/
    if (my_id==0) {
        Counter1=0;
        TempPointer = &A[0];
        for (Counter1=0;Counter1<ProblemSize;Counter1++) {
            (*TempPointer) = (int)10000*ran(&RandomSeed);
            printf("");  /*Note this is here to ensure the randomness of the numbers. */
            /* I'm not sure why it works but it does.*/
            TempPointer++;
        }
    }
   
    /*Create a sub array for each slave.*/
    SubSize = ProblemSize/p;
    MainSubSize = (ProblemSize-SubSize*(p-1));
    if (p==1){MergeSort(ProblemSize,1); goto EXIT;}
    /*Send raw data to slaves*/
    if (my_id==0) {
        for (Counter1=0;Counter1<(p-1);Counter1++) {
            MPI_Send(&(A[SubSize*Counter1]), SubSize, MPI_INT, Counter1+1, 0, MPI_COMM_WORLD);
        }
        for (Counter2==0;Counter2<MainSubSize;Counter2++) {
            A[Counter2] = A[(SubSize*Counter1)+Counter2];
        }
    } else {
        /*Each slave receives it's part*/
        MPI_Recv(&(A[0]), SubSize, MPI_INT, 0, 0, MPI_COMM_WORLD, &Status);
    }
    /*Machines sort their parts*/
    if (my_id==0) {
        MergeSort(MainSubSize, 1);
        /*Master receives and sorts data*/
        Counter2=0;
        for (Counter1=1;Counter1<p;Counter1++) {
            //Copy all of A into B
            for
                (Counter3=0;Counter3<ProblemSize;Counter3++)
            {B[Counter3] = A[Counter3];}
            //Read data into A[]
            MPI_Recv(&(A[0]), SubSize, MPI_INT, Counter1, 1, MPI_COMM_WORLD, &Status);
            //Append B part into A
            for (Counter3=0;Counter3<(Counter2*SubSize+MainSubSize);Counter3++) {
                A[SubSize+Counter3] = B[Counter3];}
            Counter2++;
            //Sort
            MergeSort(Counter2*SubSize+MainSubSize, SubSize);
        }
    } else {
        MergeSort(SubSize, 1);
        /*Slaves send data to master*/
        MPI_Send(&(A[0]), SubSize, MPI_INT, 0, 1, MPI_COMM_WORLD);
    } 
EXIT:
    MPI_Finalize();
}
