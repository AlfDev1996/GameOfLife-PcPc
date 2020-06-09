#include <stdio.h>
#include <stdlib.h>
#include "MyUtils.h"
#include <mpi.h>
int main(int argc, char* argv[]) {
    int myRank,tasks;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);

    if(tasks>1){
        printf("Versione sequenziale avviare con un solo processo\n");
        exit(1);
    }
    float start = MPI_Wtime();
    if(myRank ==0){

        //Dimensione N righe e M colonne della matrice
        int n = atoi(argv[1]);
        int m = atoi(argv[2]);
        int iterazioni = atoi(argv[3]);
        srand(12);

        int **matrix = (int **) malloc(n * sizeof(int *));
        int **nextGen = (int **) malloc(n * sizeof(int *));
        for (int i = 0; i < n; ++i) {
            matrix[i] = (int *) malloc(sizeof(int) * m);
            nextGen[i] = (int *) malloc(sizeof(int) * m);
        }

        initializeMatrix(n, m, matrix);
        printMatrix(n,m,matrix);

        int count = 0;

        for (int iter = 0; iter < iterazioni; ++iter) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {

                    nextGen[i][j] = verifyCellula(i, j, n, m, matrix, count);
                    count = 0;
                }
            }

            swapMatrix(n, m, matrix, nextGen);

        }
        float end = MPI_Wtime();
        printMatrix(n, m, matrix);
        printf("\nNumero di righe: %d\nNumero di colonne: %d\nTotale elementi; %d\nNumero di iterazioni: %d\nTempo di esecuzione: %f\n",n,m,n*m,iterazioni, (end-start));
    }
    MPI_Finalize();

    return 0;
}
