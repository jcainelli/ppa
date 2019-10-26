#include "matriz-operacoesv3.h"

mymatriz *msomar (mymatriz *mat_a, mymatriz *mat_b, int tipo) {

  mymatriz *mat_result;

  mat_result = (mymatriz *) malloc(sizeof(mymatriz));

  mat_result->lin = mat_a->lin;
  mat_result->col = mat_a->col;

  if (mat_a->lin != mat_b->lin || mat_a->col != mat_b->col) {
    printf("ERRO: Matrizes inválidas para adição.\n");
    exit(1);
  }

  if (!malocar(mat_result)) {
		printf ("ERROR: Out of memory\n");
    exit(1);
	}

  mzerar(mat_result);

  if(tipo == 0) {
    for(int i = 0; i < mat_a->lin; i++) {
      for(int j = 0; j < mat_a->col; j++) {
        mat_result->matriz[i][j] = mat_a->matriz[i][j] + mat_b->matriz[i][j];
      }
    }
  } else {
    for(int j = 0; j < mat_a->col; j++) {
      for(int i = 0; i < mat_a->lin; i++) {
        mat_result->matriz[i][j] = mat_a->matriz[i][j] + mat_b->matriz[i][j];
      }
    }
  }

  return mat_result;
}

mymatriz *mmultiplicar (mymatriz *mat_a, mymatriz *mat_b, int tipo) {

  mymatriz *mat_result = (mymatriz *) malloc(sizeof(mymatriz));

  mat_result->lin = mat_a->lin; // N
  mat_result->col = mat_b->col; // M

  if (mat_a->col != mat_b->lin) {
    printf("ERRO: Matrizes inválidas para multiplicação.\n");
    exit(1);
  }

  if (!malocar(mat_result)) {
		printf ("ERROR: Out of memory\n");
    exit(1);
  }

  mzerar(mat_result);

  switch (tipo) {
    case 0:
      for(int i = 0; i < mat_a->lin; i++) { // N
        for(int j = 0; j < mat_b->col; j++) { // M
          for(int k = 0; k < mat_a->col; k++) { // L
            mat_result->matriz[i][j] += (mat_a->matriz[i][k] * mat_b->matriz[k][j]);
          }
        }
      }
      break;

    case 1:
      for(int j = 0; j < mat_b->col; j++) { // M
        for(int k = 0; k < mat_a->col; k++) { // L
          for(int i = 0; i < mat_a->lin; i++) { // N
            mat_result->matriz[i][j] += (mat_a->matriz[i][k] * mat_b->matriz[k][j]);
          }
        }
      }
      break;

    case 2:
      for(int k = 0; k < mat_a->col; k++) { // L
        for(int i = 0; i < mat_a->lin; i++) { // N
          for(int j = 0; j < mat_b->col; j++) { // M
            mat_result->matriz[i][j] += (mat_a->matriz[i][k] * mat_b->matriz[k][j]);
          }
        }
      }
      break;

    case 3:
      for(int i = 0; i < mat_a->lin; i++) { // N
        for(int k = 0; k < mat_a->col; k++) { // L
          for(int j = 0; j < mat_b->col; j++) { // M
            mat_result->matriz[i][j] += (mat_a->matriz[i][k] * mat_b->matriz[k][j]);
          }
        }
      }
      break;

    case 4:
      for(int j = 0; j < mat_b->col; j++) { // M
        for(int i = 0; i < mat_a->lin; i++) { // N
          for(int k = 0; k < mat_a->col; k++) { // L
            mat_result->matriz[i][j] += (mat_a->matriz[i][k] * mat_b->matriz[k][j]);
          }
        }
      }
      break;

    case 5:
      for(int k = 0; k < mat_a->col; k++) { // L
        for(int j = 0; j < mat_b->col; j++) { // M
          for(int i = 0; i < mat_a->lin; i++) { // N
            mat_result->matriz[i][j] += (mat_a->matriz[i][k] * mat_b->matriz[k][j]);
          }
        }
      }
      break;
  }

  return mat_result;
}

// multiplica a mat_suba pela mat_subb atribuindo o resultado a mat_subc;
int mmsubmatriz (matriz_bloco_t *mat_suba, matriz_bloco_t *mat_subb, matriz_bloco_t *mat_subc) {
  mat_subc->matriz = mmultiplicar(mat_suba->matriz, mat_subb->matriz, 3);

  return 1;
}
