#include <omp.h>
#include "toolsv3.h"
#include "matrizv3.h"
#include "matriz-operacoesv3.h"
#include "matriz-operacoes-omp.h"

int multiplicarOMP(mymatriz *mat_a, mymatriz *mat_b, mymatriz *mat_c, int ntasks)
{
   int i_max = mat_a->lin;
   int j_max = mat_b->col;
   int k_max = mat_a->col;

    #pragma omp parallel for num_threads(ntasks) shared (i_max,j_max,k_max,mat_a,mat_b,mat_c)
    for (int i = 0; i < i_max; i++) {
        for (int k = 0; k < k_max; k++) {
            for (int j = 0; j < j_max; j++) {
                mat_c->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
            }
        }
    }

    return 0;
} multiplicarOMPblocos(matriz_bloco_t *mat_suba, matriz_bloco_t *mat_subb, matriz_bloco_t *mat_subc)
{
    mmsubmatriz(mat_suba, mat_subb, mat_subc);
  

int  return 0;
}