#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toolsv3.h"
#include "matrizv3.h"



int malocar(mymatriz *mat){

    mat->matriz = (int **) malloc(mat->lin * sizeof(int*));
    for(int i=0; i < mat->lin; i++){
        mat->matriz[i] = (int *) malloc(mat->col*sizeof(int));
    }

    if(mat->matriz){
       // printf("Alocou Matriz em malocar\n");
        return 1;

    }else{
        printf("Erro ao alocar matriz em malocar\n");
        return 0;
    }
   
}

int mcomparar(mymatriz *mat_a, mymatriz *mat_b){
    int la = mat_a->lin;
    int lb = mat_b->lin;

    int ca = mat_a->col;
    int cb = mat_b->col;
    int diff = 0;


    if(la != lb || ca != cb){
        printf("As matrizes NÃO TEM a mesma DIMENSÃO, la= %d, lb= %d, ca= %d, cb= %d!! \n", la, lb, ca, cb);
    }else{
         for(int i=0; i < la; i++){
            for(int j=0; j < ca; j++){

                if(mat_a->matriz[i][j] != mat_b->matriz[i][j]){
                   
                   diff=0;
                   printf("As matrizes NÃO são identicas, A posição %dx%d=%d de A é diferente posição %dx%d=%d de B\n", i,j,mat_a->matriz[i][j], i,j, mat_b->matriz[i][j]);
                   break;
                    
                }else{
                  
                    diff=1;    
                }
            }
        }
        if(diff!=0){
            printf("As matrizes são idênticas!!! \n");
        }
    }
    return 0;
}

int mliberar(mymatriz *mat){
      
       for(int i=0; i < mat->lin; i++){
            free(mat->matriz[i]);
        }
        free(mat->matriz);
   
    return 0;

}

int mgerar(mymatriz *matriz, int valor){
    
    for (int i=0; i < matriz->lin; i++) {
        for (int j=0; j < matriz->col; j++){
            matriz->matriz[i][j] = rand() % 10;//rand() % 10;
            }
    }
    return valor;
}


int mimprimir(mymatriz *matriz){
    for (int i=0; i < matriz->lin; i++) {
        for (int j=0; j < matriz->col; j++){
         printf("%d,", matriz->matriz[i][j]);
        }
        printf("\n");
    }
    return 0;

}

int mzerar(mymatriz *mat){
    mat->matriz = (int **) calloc(mat->lin, mat->lin * sizeof(int*));
    for(int i=0; i < mat->lin; i++){
        mat->matriz[i] = (int *) calloc(mat->col, mat->col*sizeof(int));
    }
  //  printf ("Matriz, zeradinhaaa, saindoooo!!! \n");
   return 1;
}

