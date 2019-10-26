#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "matrizv3.h"

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

mymatriz *msomar (mymatriz *mat_a, mymatriz *mat_b, int tipo);
mymatriz *mmultiplicar (mymatriz *mat_a, mymatriz *mat_b, int tipo);

// multiplica a mat_suba pela mat_subb atribuindo o resultado a mat_subc;
int mmsubmatriz (matriz_bloco_t *mat_suba, matriz_bloco_t *mat_subb, matriz_bloco_t *mat_subc);
