#include "matriz-operacoesv3.h"
#include "matriz-operacoes-MPI.h"

int multiplicarMPI(int **mat_a, int **mat_b, int **mat_c, int N, int L, int M, int size){

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int i,j,k;
	for (i = rank; i < N; i += size) {
		for(k = 0; k < L; k++) {
			for(j = 0; j < M; j++) {
				mat_c[i][j] += mat_a[i][k] * mat_b[k][j];
			}
		}
	}

	return 0;
}


void multiplicarMPIblocos(matriz_bloco_t **mat_a, matriz_bloco_t **mat_b, matriz_bloco_t **mat_c, int **matFim, int divisor, int size)
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	for(int i = rank; i < divisor; i += size){
		mmsubmatriz(mat_a[i], mat_b[i], mat_c[i]);
	}

  for (int j=0;j<divisor;j++){
    msomar(mat_c[j]->matriz, matFim, matFim, mat_c[j]->bloco->lin_fim, mat_c[j]->bloco->lin_fim, mat_c[j]->bloco->col_fim);
  }
}

int *matrixToVector(int**mat, int N, int M) {
	int* buff = (int*)malloc(N * M * sizeof(int));

	int bufIndex = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			buff[bufIndex] = mat[i][j];
			bufIndex++;
		}
	}

	return buff;
}

void vectorToMatrix(int *vector, int **mat, int N, int M) {
	int bufIndex = 0;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			mat[i][j] = vector[bufIndex];
			bufIndex++;
		}
	}
}


void somaVetor(int* vecA, int *vecB, int *res, int size) {
	for (int i = 0; i < size; i++) {
		res[i] = vecA[i] + vecB[i];
	}
}
