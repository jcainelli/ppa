#include "matriz-operacoes-omp.h"

mymatriz *multiplicarOMP(mymatriz *mat_a, mymatriz *mat_b, int ntasks) {

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

  int N=mat_a->lin, L=mat_a->col, M=mat_b->col;

  #pragma omp parallel for num_threads(ntasks) shared(N, L, M)
  for(int i = 0; i < N; i++) { // N
    for(int k = 0; k < L; k++) { // L
      for(int j = 0; j < M; j++) { // M
        mat_result->matriz[i][j] += (mat_a->matriz[i][k] * mat_b->matriz[k][j]);
      }
    }
  }

  return mat_result;
}

mymatriz *multiplicarOMPblocos(mymatriz *mat_a, mymatriz *mat_b, int ntasks) {

  matriz_bloco_t **v_submat_a,**v_submat_b,**v_submat_c;
  mymatriz *result, *tempResult;

  int blocos=2;

  if(ntasks >= 4) {
		blocos = 4;
	}

  #pragma omp parallel
  {
    #pragma omp sections
    {
      #pragma omp section
      {
        v_submat_a = particionar_matriz (mat_a->matriz, mat_a->lin, mat_a->col, 1, blocos);
      }

      #pragma omp section
      {
    	   v_submat_b = particionar_matriz (mat_b->matriz, mat_b->lin, mat_b->col, 0, blocos);
      }
    }
  }

	v_submat_c = csubmatrizv2 (mat_a->lin, mat_b->col, blocos);

  #pragma omp parallel for num_threads (ntasks) shared(blocos)
  for(int t = 0; t < blocos; t++) {
    mymatriz *mat_a, *mat_b, *mat_result;

    mat_a = v_submat_a[t]->matriz;
    mat_b = v_submat_b[t]->matriz;
    mat_result = (mymatriz *) malloc(sizeof(mymatriz));

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

    int N=mat_a->lin, L=mat_a->col, M=mat_b->col;

    #pragma omp parallel for num_threads(ntasks) shared(N, L, M)
    for(int i = 0; i < N; i++) { // N
      for(int k = 0; k < L; k++) { // L
        for(int j = 0; j < M; j++) { // M
          mat_result->matriz[i][j] += (mat_a->matriz[i][k] * mat_b->matriz[k][j]);
        }
      }
    }

    v_submat_c[t]->matriz = mat_result;
  }

  for(int taskResults = 0; taskResults < blocos; taskResults+=2) {
    if(taskResults == 0) {
      result = msomar(v_submat_c[taskResults]->matriz, v_submat_c[taskResults+1]->matriz, 1);
    } else {
      tempResult = msomar(v_submat_c[taskResults]->matriz, v_submat_c[taskResults+1]->matriz, 1);
      result = msomar(result, tempResult, 1);
    }
  }

  return result;
}
