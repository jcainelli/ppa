#include "matrizv3.h"
#include "matriz-operacoesv3.h"

int multiplicarTh(mymatriz *mat_a, mymatriz *mat_b, mymatriz *mat_c, int tid, int ntasks)
{
    for (int i = tid; i < mat_a->lin; i += ntasks) {
        for (int k = 0; k < mat_a->col; k++) {
            for (int j = 0; j < mat_b->col; j++) {
                mat_c->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
            }
        }
    }

    return 0;
}

int multiplicarThblocos(matriz_bloco_t *mat_suba, matriz_bloco_t *mat_subb, matriz_bloco_t *mat_subc)
{
    mmsubmatriz(mat_suba, mat_subb, mat_subc);
    return 0;
}