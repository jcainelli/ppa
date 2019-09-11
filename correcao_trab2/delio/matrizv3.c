#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toolsv3.h"
#include "matrizv3.h"
#define _DEBUG_ 0


int malocar (mymatriz *matriz){
  matriz->matriz = (int **) malloc(matriz->lin*sizeof(int*));
  for(int i=0; i< matriz->lin; i++){
    matriz->matriz[i] = (int *) malloc(matriz->col*sizeof(int));

  }
  #if _DEBUG_
  printf("Matriz alocada com sucesso\n");
  #endif



  return 0;
}

int mliberar (mymatriz *matriz){
  #if _DEBUG_
  printf("Entrou .na função mliberar...\n");
  #endif

  free(matriz->matriz);

  return 0;
}

int mcomparar (mymatriz *mat_a, mymatriz *mat_b){
  #if _DEBUG_
  //mostrar matrizes antes de comparar
  mimprimir(mat_a);
  mimprimir(mat_b);
  #endif

  int diff = 0;
  #if _DEBUG_
  printf("Entrou na função mcomparar\n");
  #endif
  if (mat_a->lin == mat_b->lin){
    #if _DEBUG_
    printf("Tamanho de linhas matriz A = B\n");
    #endif
      if (mat_a->col == mat_b->col){
        #if _DEBUG_
        printf("Tamanho de colunas matriz A = B\n");
        #endif
        for (int i = 0;i<mat_a->lin;i++){
          for (int j = 0; j<mat_a->col;j++){
            if (mat_a->matriz[i][j] != mat_b->matriz[i][j]) {
              diff = 1;
            }            
          }
        }
        if (diff == 0){
          printf("\nAs matrizes são iguais");
        }
        else{
          printf("\nAs matrizes são diferentes");
        }
      }else{
        printf("\nAs matrizes tem tamanho diferentes");
      }

  }
  else{
    printf("\nAs matrizes tem tamanho diferentes");
  }
  #if _DEBUG_
  printf("tamanho a linhas: %i colunas: %i\n",mat_a->lin, mat_a->col);
  printf("tamanho b linhas: %i colunas: %i\n",mat_b->lin, mat_b->col);
  #endif

  return 0;

}

int mzerar(mymatriz *matriz){
  for (int i = 0;i<matriz->lin;i++){
    for (int j = 0; j<matriz->col;j++){
      matriz->matriz[i][j] = 0;
    }
  }
  return 0;
}

int mimprimir(mymatriz *matriz){
  for (int i = 0;i<matriz->lin;i++){
    printf("\n%i-> ",i+1);
    for (int j = 0; j<matriz->col;j++){
      printf("| %i ",matriz->matriz[i][j]);
    }
  }
  printf("\n");
  return 0;
}


int mgerar(mymatriz *matriz, int valor){
  #if _DEBUG_
  printf("Entrou na função mgerar...\n");
  #endif
  for (int i = 0;i<matriz->lin;i++){
    for (int j = 0; j<matriz->col;j++){
      matriz->matriz[i][j] = valor;
    }
  }
  return 0;

  

}

