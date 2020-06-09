//
// Created by alfonso 
//

/*
    Any live cell with fewer than 2 live neighbors dies as if caused by underpopulation.
    Any live cell with 2 or 3 live neighbors lives on to the next generation.
    Any live cell with more than 3 live neighbors dies, as if by overpopulation.
    Any dead cell with exactly 3 live neighbors becomes a live cell, as if by reproduction.
*/


void printMatrix(int n,int m,int(*matrix)[m]){
    for(int i=0; i<n;++i){
        for (int j=0; j<m;++j){
            printf(" %d ",matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");

}


void initializeMatrix(int n, int m, int (*matrix)[m]){
    srand(12);
    for(int i=0; i<n;++i){
        for(int j=0; j<m;++j){
            matrix[i][j]= rand()%2;
        }
    }
}
/*Funzione di modulo che non esiste in C99*/
int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

int verifyCellula(int i,int j,int n,int m,int  elem[n][m], int count,int rank){
    /*
    Any live cell with fewer than 2 live neighbors dies as if caused by underpopulation.
    Any live cell with 2 or 3 live neighbors lives on to the next generation.
    Any live cell with more than 3 live neighbors dies, as if by overpopulation.
    Any dead cell with exactly 3 live neighbors becomes a live cell, as if by reproduction.
*/

    count += elem[mod(i-1,n)][j % m];
    count+= elem[(i+1)%n][j%m];
    count+= elem[i%n][mod(j-1,m)];
    count+= elem[i%n][(j+1)%m];
    count+= elem[mod(i-1,n)][mod(j-1,m)];
    count+= elem[(i+1)%n][(j+1)%m];
    count+= elem[mod(i-1,n)][(j+1)%m];
    count+= elem[(i+1)%n][mod(j-1,m)];

    if(elem[i][j]==1 && count < 2)
     return 0;  //Cellula morta per sottopopolazione

    if(elem[i][j]==1 && (count>=2 && count <=3))
        return 1; //Cellula sopravvive alla prossima Generazione
    if(elem[i][j]==1 && count>3)
        return 0;  //Cellula morta per sovrapopolazione
    if(elem[i][j]==0 && count==3)
        return 1; //Cellula Rinasce per la prossima generazione

}

void runGame(int n,int m,int myRank, int tasks, int dimArray[tasks], MPI_Datatype rowType, int row, int dspl[tasks], int app, int (*a)[m],int (*nextGen)[m],int (*matrix)[m],MPI_Status status ){
    MPI_Scatterv(matrix, dimArray, dspl, rowType, &a[1][0], row, rowType, 0, MPI_COMM_WORLD);
    if (myRank > 0) {

        int count = 0;

        //Invio al mio precedente la prima riga della mia zona
        MPI_Request request;
        int dest = myRank == 1 ? (tasks - 1) : myRank - 1;
        //TAG = 2  invio riga 1 al mio precedente
        MPI_Isend(&a[1][0], 1, rowType, dest, 2, MPI_COMM_WORLD, &request);
        dest = (myRank == tasks - 1) ? 1 : myRank + 1;
        //TAG = 3 invio la ultima riga al mio successivo
        MPI_Isend(&a[row][0], 1, rowType, dest, 3, MPI_COMM_WORLD, &request);

        MPI_Request requests[2];
        int source = myRank == 1 ? (tasks - 1) : myRank - 1;
        MPI_Irecv(&a[0][0], m, MPI_INT, source, 3, MPI_COMM_WORLD, &requests[0]);
        source = myRank == (tasks - 1) ? 1 : myRank + 1;
        MPI_Irecv(&a[app - 1][0], m, MPI_INT, source, 2, MPI_COMM_WORLD, &requests[1]);

        for (int i = 2; i < (app - 2); ++i) {
            for (int j = 0; j < m; ++j) {

                nextGen[i - 1][j] = verifyCellula(i, j, app, m, a, count, myRank);

                count = 0;
            }
        }
        int countRecv = 2;
        int idx;
        while (countRecv > 0) {
            MPI_Waitany(2, requests, &idx, &status);
            if (idx == 0)
                for (int j = 0; j < m; ++j) {

                    nextGen[0][j] = verifyCellula(1, j, app, m, a, count, myRank);

                    count = 0;
                }
            else {
                for (int j = 0; j < m; ++j) {

                    nextGen[row - 1][j] = verifyCellula(row, j, app, m, a, count, myRank);

                    count = 0;
                }
            }
            --countRecv;
        }
    }

    MPI_Gatherv(&nextGen[0][0], row, rowType, matrix, dimArray, dspl, rowType, 0, MPI_COMM_WORLD);
}


