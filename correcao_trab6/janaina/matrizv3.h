#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int mgerar(int **matriz, int linha, int coluna, int valor);
int mzerar  (int **matriz, int linha, int coluna);
int mimprimir  (int **matriz, int linha, int coluna);
int mcomparar (int **matriza, int **matrizb, int linha, int coluna);
int **mliberar  (int **matriz, int linha, int coluna);
int **malocar  (int linha, int coluna);

matriz_bloco_t **particionar_matriz (int **matriz, int mat_lin, int mat_col, int orientacao, int divisor);
matriz_bloco_t **liberar_submatriz (matriz_bloco_t **submatriz, int divisor);
matriz_bloco_t **constroi_submatrizv2 (int mat_lin, int mat_col, int divisor);
