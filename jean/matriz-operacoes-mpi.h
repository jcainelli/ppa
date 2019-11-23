#ifndef MATRIX_OPE_SEQ_H
#define MATRIX_OPE_SEQ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mpi.h>
#include "toolsv3.h"
#include "matrizv3.h"
#include "matriz-operacoesv3.h"
#include "matriz-operacoes-mpi.h"

typedef struct {
	int tid;
    int ntasks;
    mymatriz *mat_a;
    mymatriz *mat_b;
    mymatriz *mat_c;
    matriz_bloco_t *mat_bloco_a;
    matriz_bloco_t *mat_bloco_b;
    matriz_bloco_t *mat_bloco_c;
}param_t;

int multiplicarMPIblocos(matriz_bloco_t *mat_suba, matriz_bloco_t *mat_subb, matriz_bloco_t *mat_subc);
int multiplicarMPI (mymatriz *mat_a, mymatriz *mat_b, mymatriz *mat_c, int nMaxThread);

#endif