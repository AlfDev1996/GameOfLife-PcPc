//
// Created by alfonso on 27/04/20.
//

/*
    Any live cell with fewer than 2 live neighbors dies as if caused by underpopulation.
    Any live cell with 2 or 3 live neighbors lives on to the next generation.
    Any live cell with more than 3 live neighbors dies, as if by overpopulation.
    Any dead cell with exactly 3 live neighbors becomes a live cell, as if by reproduction.
*/

void printMatrix(int n,int m,int**matrix){
    for(int i=0; i<n;++i){
        for (int j=0; j<m;++j){
            printf(" %d ",matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");

}

void initializeMatrix(int n, int m, int** matrix){
    srand(12);
    for(int i=0; i<n;++i){
        for(int j=0; j<m;++j){
            matrix[i][j]= rand()%2;
        }
    }
}

void swapMatrix(int n,int m, int **matrix, int **nextGen){
    for(int i=0; i<n;++i){
        for (int j=0; j<m;++j){
            matrix[i][j]= nextGen[i][j];
        }
    }
}
/*Funzione di modulo che non esiste in C99*/
int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

int verifyCellula(int i,int j,int n,int m,int** elem, int count){
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
      return 0; //Cellula morta per sottopopolazione
    if(elem[i][j]==1 && (count>=2 && count <=3))
      return 1; //Cellula sopravvive alla prossima Generazione
    if(elem[i][j]==1 && count>3)
       return 0; //Cellula morta per sovrapopolazione
    if(elem[i][j]==0 && count==3)
       return 1; //Cellula Rinasce per la prossima generazione

}

