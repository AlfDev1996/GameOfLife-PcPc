#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "MyUtilsUp.h"

int main(int argc, char* argv[]) {
    int myRank, tasks;
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int iterazioni = atoi(argv[3]);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    MPI_Datatype rowType;
    MPI_Type_vector(1,m,m,MPI_INT,&rowType);
    MPI_Type_commit(&rowType);
    MPI_Status status;
    int (*matrix)[m];
    int (*a)[m];
    int dspl[tasks];
    dspl[0]=0;
    int app, row=0;
    int dimArray[tasks];
    int (*nextGen)[m];


    MPI_Barrier(MPI_COMM_WORLD);
    float startTime= MPI_Wtime();


    if(myRank==0){
        matrix = malloc(sizeof(int [n][m]));
        initializeMatrix(n,m,matrix);
        printMatrix(n,m,matrix);

        int resto = n%(tasks-1);
        dimArray[0]=0;
        int start=0,end=0;
        for(int i= 1; i<tasks;++i){
            dimArray[i]= n/(tasks-1);
            if(resto>0){
                dimArray[i]++;
                --resto;
            }
            start=end;
            dspl[i]=start;
            end+=dimArray[i];
        }

    }

    MPI_Scatter(&dimArray,1,MPI_INT,&row,1,MPI_INT,0,MPI_COMM_WORLD);
    if(myRank>0){

        app = row +2;
        a = malloc(sizeof(int [app][m]));
        nextGen = malloc(sizeof(int[row][m]));
    }
    for(int k=0; k<iterazioni ;++k) {

        runGame(n,m,myRank,tasks,dimArray,rowType,row,dspl,app,a,nextGen,matrix,status);


    }
    MPI_Type_free(&rowType);
    MPI_Barrier(MPI_COMM_WORLD);
    float endTime= MPI_Wtime();


    if(myRank==0){
        printMatrix(n,m,matrix);
        printf("Tempo di esecuzione  %f \n", (endTime-startTime));
    }
    MPI_Finalize();
    return 0;
}
