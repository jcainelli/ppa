#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "toolsv3.h"
#include "matrizv3.h"
#include "matriz-operacoesv3.h"
#include "matriz-operacoes-MPI.h"

int main(int argc, char *argv[]) {
	MPI_Init(NULL, NULL);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	int **mat_a = NULL;
	int **mat_b = NULL;
	int **mat_c = NULL;
	int **mat_d =NULL;
	double mediaSeqNor = 0, mediaSeqBlock = 0, mediaMpiNormal = 0, mediaMPIBlock = 0;
	int **matFim =NULL;
	int **matFim2 =NULL;
	FILE *fmat_a=NULL, *fmat_b=NULL, *fmat_c=NULL;
	int nr_line;
	int *vet_line = NULL;
	int N, M, La, Lb;
	double start_time, end_time;

	matriz_bloco_t **Vsubmat_a = NULL;
	matriz_bloco_t **Vsubmat_b = NULL;
	matriz_bloco_t **Vsubmat_c = NULL;
	int divisor = 2;

	if (rank == 0) { 
		if (argc != 3){
			printf ("ERRO: Numero de parametros %s <matriz_a> <matriz_b>\n", argv[0]);
			exit (1);
		}

		fmat_a = fopen(argv[1],"r");
		fmat_b = fopen(argv[2],"r");

		if ((fmat_a == NULL) || (fmat_b == NULL)) {
			printf("Error: Na abertura dos arquivos.");
			exit(1);
		}

		extrai_parametros_matriz(fmat_a, &N, &La, &vet_line, &nr_line);
		mat_a = malocar (N, La);
		filein_matriz (mat_a, N, La, fmat_a, vet_line, nr_line);

		extrai_parametros_matriz(fmat_b, &Lb, &M, &vet_line, &nr_line);
		mat_b = malocar (Lb, M);
		filein_matriz (mat_b, Lb, M, fmat_b, vet_line, nr_line);

		mat_c = malocar (N, M);
		mzerar (mat_c, N, M);

		mat_d = malocar (N, M);
		mzerar (mat_d, N, M);

		matFim = malocar (N, M);
		mzerar (matFim, N, M);

		matFim2 = malocar (N, M);
		mzerar (matFim2, N, M);

		if ((mat_c == NULL) || (mat_b == NULL) || (mat_a == NULL)) {
			printf("Erro: Matriz(es) invalida(s)\n");
		}
	}

	// Mestre
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&La, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&Lb, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&divisor, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int *vector_a = malloc(sizeof(int) * N * La);
	int *vector_b = malloc(sizeof(int) * Lb * M);

	if (rank == 0) {
		vector_a = matrixToVector(mat_a, N, La);
		vector_b = matrixToVector(mat_b, Lb, M);
	}

	MPI_Bcast(vector_a, N * La, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(vector_b, Lb * M, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank != 0) {
		mat_a = malocar (N, La);
		vectorToMatrix(vector_a, mat_a, N, La);

		mat_b = malocar (Lb, M);
		vectorToMatrix(vector_b, mat_b, Lb, M);

		mat_d = malocar (N, M);
		matFim2 = malocar (N, M);
		mzerar (matFim2, N, M);
	}

	// Bloco
	for(int i=0; i<10; i++) {
		mzerar (matFim2, N, M);

		start_time = wtime();
		Vsubmat_a = particionar_matriz (mat_a, N, La, 1, divisor);
		Vsubmat_b = particionar_matriz (mat_b, Lb, M, 0, divisor);
		Vsubmat_c = constroi_submatrizv2 (N, M, divisor);

		multiplicarMPIblocos(Vsubmat_a, Vsubmat_b, Vsubmat_c, matFim2, divisor, size);
		end_time = wtime();

		if (rank != 0) { 
			int *vec = matrixToVector(matFim2, N, M);
			MPI_Send(vec, N * M, MPI_INT, 0, 0, MPI_COMM_WORLD);
		} else {
			// Bloco
			int *vector_temp = malloc(sizeof(int) * N * M);
			int *finalzera = matrixToVector(matFim2, N, M);

			MPI_Status status;

			for (int i = 1; i < size; i++) {
				MPI_Recv(vector_temp, N * M, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
				somaVetor(vector_temp, finalzera, finalzera, N * M);
			}

			vectorToMatrix(finalzera, matFim2, N, M);
			mediaMPIBlock += end_time - start_time;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}


// Normal
	for(int i=0; i<10; i++) {
		mzerar (mat_d, N, M);

		if (rank == 0) { // Mestre
			start_time = wtime();

			multiplicarMPI(mat_a, mat_b, mat_d, N, La, M, size);

			end_time = wtime();

			int *vector_temp = malloc(sizeof(int) * N * M);
			int *finalzera = matrixToVector(mat_d, N, M);

			MPI_Status status;

			for (int i = 1; i < size; i++) {
				MPI_Recv(vector_temp, N * M, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
				somaVetor(vector_temp, finalzera, finalzera, N * M);
			}

			vectorToMatrix(finalzera, mat_d, N, M);

			mediaMpiNormal += end_time - start_time;

		} else { 
			multiplicarMPI(mat_a, mat_b, mat_d, N, La, M, size);

			int *vec = matrixToVector(mat_d, N, M);
			MPI_Send(vec, N * M, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}

		MPI_Barrier(MPI_COMM_WORLD);
	}

	if (rank == 0) {
		mediaMpiNormal /= 10;
		mediaMPIBlock /= 10;

		fmat_c= fopen("Resultado_MPI_Normal.result","w");
		fileout_matriz(mat_d, N, M, fmat_c);

		fmat_c= fopen("Resultado_MPI_Bloco.result","w");
		fileout_matriz(matFim2, N, M, fmat_c);

		if(La != Lb){
			printf("Matriz coluna diferente de matriz Linha. Não é possível multiplicar\n");
			return 1;
		} else {
			// Multiplicação Normal sequencial
			for(int i=0; i<10; i++){
				mzerar (mat_c, N, M);
				start_time = wtime();
				mmultiplicar(mat_a,mat_b,mat_c, N, La, M);
				end_time = wtime();
				mediaSeqNor += end_time - start_time;
				fmat_c= fopen("Resultado_Sequencial.result","w");
				fileout_matriz(mat_c, N, M, fmat_c);
			}
			mediaSeqNor = mediaSeqNor/10;

			// Multiplicacao Bloco Sequencial
			for (int i =0; i<10; i++){
				mzerar (matFim, N, M);
				start_time = wtime();
				Vsubmat_a = particionar_matriz (mat_a, N, La, 1, divisor);
				Vsubmat_b = particionar_matriz (mat_b, Lb, M, 0, divisor);
				Vsubmat_c = constroi_submatrizv2 (N, M, divisor);
				for(int j=0; j<divisor; j++){
					mmsubmatriz(Vsubmat_a[j], Vsubmat_b[j], Vsubmat_c[j]);
				}
				for(int k = 0; k<divisor; k++){
					msomar(Vsubmat_c[k]->matriz, matFim, matFim, N, La, M);
				}
				end_time = wtime();
				mediaSeqBlock += end_time - start_time;
				fmat_c= fopen("Resultado_Bloco_Sequencial.result","w");
				fileout_matriz(matFim, N, M, fmat_c);
			}
			mediaSeqBlock = mediaSeqBlock/10;

			printf("\n\n****** Resultados ****** \n\n");

			printf("COMPARAR MATRIZ_SeqC c/ MATRIZ_SeqBlC\n");
			mcomparar  (mat_c, matFim, N, M);
			printf("COMPARAR MATRIZ_SeqC c/ MATRIZ_MPIC\n");
			mcomparar  (mat_c, mat_d, N, M);
			printf(" COMPARAR MATRIZ_SeqC c/ MATRIZ_MPIBlC\n");
			mcomparar  (mat_c, matFim2, N, M);
			printf("TEMPO_MEDIO MATRIZ_SeqC : %f\n", mediaSeqNor);
			printf("TEMPO_MEDIO MATRIZ_SeqBlC: %f\n", mediaSeqBlock);
			printf("TEMPO_MEDIO MATRIZ_MPIC: %f\n", mediaMpiNormal);
			printf("TEMPO_MEDIO MATRIZ_MPIBlC: %f\n\n", mediaMPIBlock);
			printf("SPEEDUP (MATRIZ_C): %f\n", mediaSeqNor/mediaMpiNormal);
			printf("SPEEDUP (MATRIZ_BLC): %f\n\n", mediaSeqBlock/mediaMPIBlock);
	
			// LIBERAR MEMÓRIA
			liberar_submatriz (Vsubmat_a,divisor);
			liberar_submatriz (Vsubmat_b,divisor);
			liberar_submatriz (Vsubmat_c,divisor);

			mliberar (mat_a,N,La);
			mliberar (mat_b,Lb,M);
			mliberar (mat_c,N,M);
			mliberar (mat_d,N,M);
			mliberar (matFim,N,M);
			mliberar (matFim2,N,M);

			fclose(fmat_a);
			fclose(fmat_b);
			fclose(fmat_c);

		}

	}

	MPI_Finalize();

	return 0;
}
