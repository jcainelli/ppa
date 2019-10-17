#include <pthread.h>
#include "toolsv3.h"
#include "matrizv3.h"
#include "matriz-operacoesv3.h"
#include "matriz-operacoes-threads.h"

int multiplicarIKJThread (mymatriz *mat_a, mymatriz *mat_b, mymatriz *mat_c, int nThread, int nMaxThread) {
  int i_max = mat_a->lin;
  int j_max = mat_b->col;
  int k_max = mat_a->col;

  for (int i = nThread; i < i_max; i += nMaxThread){
      for (int k = 0; k < k_max; k++){
          for (int j = 0; j < j_max; j++){
              mat_c->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
          }
      }
  }
  return 0;
}

int multiplicaBlocoThread(matriz_bloco_t *mat_suba, matriz_bloco_t *mat_subb, matriz_bloco_t *mat_subc)
{
    mmsubmatriz(mat_suba, mat_subb, mat_subc);
    return 0;
}

