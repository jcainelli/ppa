#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef SOME_HEADER_FILE_H
#define SOME_HEADER_FILE_H
typedef struct {
  int **matriz;
  int lin;
  int col;
} mymatriz;

typedef struct {
  int lin_inicio;
  int lin_fim;
  int col_inicio;
  int col_fim;
} bloco_t;

typedef struct {
  mymatriz *matriz;
  bloco_t *bloco;
} matriz_bloco_t;
#endif

// gerência de matrizes
int malocar (mymatriz *matriz);
int mgerar(mymatriz *matriz, int valor);
int mgerar_aleatorio(mymatriz *matriz, int valor);
int mimprimir (mymatriz *matriz);
int mzerar (mymatriz *matriz);
int mliberar (mymatriz *matriz);
int mcomparar (mymatriz *mat_a, mymatriz *mat_b);

// aloca um vetor de matrizes blocos com o número de matrizes igual ao divisor.
matriz_bloco_t **csubmatrizv2 (int mat_lin, int mat_col, int divisor);
// recebe uma matriz de inteiros e a particiona em n (parâmetro divisor) pedaço de acordo com a orientação (horizontal ou vertical)
matriz_bloco_t **particionar_matriz (int **matriz, int mat_lin, int mat_col, int orientacao, int divisor);

//Onde:
//  divisor é o numero de cortes das matrizes;
//  orientacao é "1" para horizontal e "0" para vertical
