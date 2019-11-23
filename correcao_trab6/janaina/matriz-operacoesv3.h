#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#ifndef SOME_HEADER_FILE_H
#define SOME_HEADER_FILE_H
typedef struct {
  int lin_inicio;
  int lin_fim;
  int col_inicio;
  int col_fim;
} bloco_t;

typedef struct {
  int **matriz;
  bloco_t *bloco;
} matriz_bloco_t;
#endif


int msomar (int **mat_a, int **mat_b, int **mat_c, int N, int L, int M);
int mmultiplicar (int **mat_a, int **mat_b, int **mat_c, int N, int L, int M);
int mmsubmatriz (matriz_bloco_t *mat_suba, matriz_bloco_t *mat_subb, matriz_bloco_t *mat_subc);
