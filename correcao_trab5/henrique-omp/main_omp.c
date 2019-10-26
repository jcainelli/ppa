#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toolsv3.h"
#include "matrizv3.h"
#include "matriz-operacoesv3.h"
#include "matriz-operacoes-omp.h"

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main(int argc, char *argv[]) {

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	// DECLARAÇÃO de VARIÁVEIS
	mymatriz mat_a, mat_b;
	mymatriz **mmultbloco, **mmult, **mmultthread, **mmultblocothread;
	matriz_bloco_t **Vsubmat_a, **Vsubmat_b, **Vsubmat_c;
	char filename[100];
	FILE *fmat;
	int nr_line;
	int *vet_line = NULL;
	int N, M, La, Lb, EXECUTIONS=10;
	double start_time, end_time;
	double recorded_time[4] = {0, 0, 0, 0};

  int tasks=2,blocos=2;

	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	if (argc != 3 && argc != 4){
		printf ("ERRO: Numero de parametros %s <matriz_a> <matriz_b> [N threads]\n", argv[0]);
		exit (1);
	}

	if(argc == 4) {
		tasks = atoi(argv[3]);
		if(tasks < 1) {
			tasks = 2;
		}
	}

	if(tasks >= 4) {
		blocos = 4;
	}

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//                Leitura da Matriz A (arquivo)
	fmat = fopen(argv[1],"r");
	if (fmat == NULL) {
		printf("Error: Na abertura dos arquivos.");
		exit(1);
	}
	extrai_parametros_matriz(fmat, &N, &La, &vet_line, &nr_line);
	//return 1;
	mat_a.matriz = NULL;
	mat_a.lin = N;
	mat_a.col = La;
	if (!malocar(&mat_a)) {
		printf ("ERROR: Out of memory\n");
	}
	filein_matriz (mat_a.matriz, N, La, fmat, vet_line, nr_line);
	free (vet_line);
	fclose(fmat);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//               Leitura da Matriz B (arquivo)
	fmat = fopen(argv[2],"r");
	if (fmat == NULL) {
		printf("Error: Na abertura dos arquivos.");
		exit(1);
	}
	extrai_parametros_matriz(fmat, &Lb, &M, &vet_line, &nr_line);
	mat_b.matriz = NULL;
	mat_b.lin = Lb;
	mat_b.col = M;
	if (!malocar(&mat_b)) {
		printf ("ERROR: Out of memory\n");
	}
	filein_matriz (mat_b.matriz, Lb, M, fmat, vet_line, nr_line);
	free (vet_line);
	fclose(fmat);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	printf("\n%%%%%%%%%%%%%%%%\n");

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//            Operação de Multiplicação Sequencial
	mmult = (mymatriz **) malloc (sizeof(mymatriz *));
	printf("\n ##### Multiplicação Sequencial de Matrizes #####\n");

	for(int i = 0; i < EXECUTIONS; i++) {

		start_time = wtime();
		mmult[0] = mmultiplicar(&mat_a, &mat_b, 3);
		end_time = wtime();

		printf("\tRuntime: %f\n", end_time - start_time);
		recorded_time[0] += end_time - start_time;

	}

	recorded_time[0] = recorded_time[0] / EXECUTIONS;

	sprintf(filename, "mult_seq.result");
	fmat = fopen(filename,"w");
	fileout_matriz(mmult[0], fmat);
	fclose(fmat);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//           Operações de Multiplicação em Bloco
	printf("\n ##### Multiplicação em Bloco de Matrizes #####\n");

	mmultbloco = (mymatriz **) malloc (tasks * sizeof(mymatriz *));

	for(int i = 0; i < EXECUTIONS; i++) {

		start_time = wtime();

		Vsubmat_a = particionar_matriz (mat_a.matriz, N, La, 1, blocos);
		Vsubmat_b = particionar_matriz (mat_b.matriz, Lb, M, 0, blocos);
		Vsubmat_c = csubmatrizv2 (N, M, blocos);
		for(int sub = 0; sub < blocos; sub++) {
			mmsubmatriz (Vsubmat_a[sub], Vsubmat_b[sub], Vsubmat_c[sub]);
		}

		int multbloc=0;
		for(int nro_sub = 0; nro_sub < blocos; nro_sub+=2) {
			mmultbloco[multbloc++] = msomar(Vsubmat_c[nro_sub]->matriz, Vsubmat_c[nro_sub+1]->matriz, 1);
		}

		if(blocos == 4) {
			mmultbloco[0] = msomar(mmultbloco[0], mmultbloco[1], 1);
		} else if (blocos == 8) {
			mmultbloco[0] = msomar(mmultbloco[0], mmultbloco[1], 1);
			mmultbloco[1] = msomar(mmultbloco[2], mmultbloco[3], 1);
			mmultbloco[0] = msomar(mmultbloco[0], mmultbloco[1], 1);
		}

		end_time = wtime();
		printf("\tRuntime: %f\n", end_time - start_time);
		recorded_time[1] += end_time - start_time;

	}

	recorded_time[1] = recorded_time[1] / EXECUTIONS;

	sprintf(filename, "mult_em_bloco.result");
	fmat = fopen(filename,"w");
	fileout_matriz(mmultbloco[0], fmat);
	fclose(fmat);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//           Operação de Multiplicação Multi-thread
	mmultthread = (mymatriz **) malloc (sizeof(mymatriz *));
	printf("\n ##### Multiplicação Multi-thread (OpenMP) de Matrizes #####\n");
	printf("\tthreads: %d\n", tasks);

	for(int i = 0; i < EXECUTIONS; i++) {

		start_time = wtime();

		mmultthread[0] = multiplicarOMP(&mat_a, &mat_b, tasks);
		end_time = wtime();

		printf("\tRuntime: %f\n", end_time - start_time);
		recorded_time[2] += end_time - start_time;

	}

	recorded_time[2] = recorded_time[2] / EXECUTIONS;

	sprintf(filename, "mult_multi_thread.result");
	fmat = fopen(filename,"w");
	fileout_matriz(mmultthread[0], fmat);
	fclose(fmat);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//       Operações de Multiplicação em Bloco Multi-Threads
	mmultblocothread = (mymatriz **) malloc (sizeof(mymatriz *));
	printf("\n ##### Multiplicação em Bloco Multi-Threads (OpenMP) de Matrizes #####\n");
	printf("\tthreads: %d\n", tasks);

	for(int i = 0; i < EXECUTIONS; i++) {

		start_time = wtime();

		mmultblocothread[0] = multiplicarOMPblocos(&mat_a, &mat_b, tasks);

		end_time = wtime();
		printf("\tRuntime: %f\n", end_time - start_time);
		recorded_time[3] += end_time - start_time;

	}

	recorded_time[3] = recorded_time[3] / EXECUTIONS;

	sprintf(filename, "mult_em_bloco_thread.result");
	fmat = fopen(filename,"w");
	fileout_matriz(mmultbloco[0], fmat);
	fclose(fmat);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//              Comparação dos resultados
	printf("\n ##### Comparação dos Resultados da Multiplicação de Matrizes #####\n");
	printf("[Sequencial vs Em Bloco]\t");
	mcomparar (mmult[0],mmultbloco[0]);
	printf("[Sequencial vs Multi-Thread]\t");
	mcomparar (mmult[0],mmultthread[0]);
	printf("[Sequencial vs Em Bloco Multi-Thread]\t");
	mcomparar (mmult[0],mmultblocothread[0]);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//                   Liberação de memória
	mliberar(mmult[0]);
	free (mmult[0]);
	mliberar(mmultbloco[0]);
	free (mmultbloco[0]);
	mliberar(mmultthread[0]);
	free (mmultblocothread[0]);

	mliberar(&mat_a);
	mliberar(&mat_b);
	free(mmult);
	free(mmultbloco);
	free(mmultthread);
	free(mmultblocothread);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	printf("Tempo médio execuções (%dx):\n\t- Sequencial:\t%.6fs\n\t- Em Bloco:\t%.6fs\n\t- Multi-Thread(OpenMP):\t%.6fs\n\t- Em Bloco MultiThread(OpenMP):\t%.6fs\n",
		EXECUTIONS, recorded_time[0], recorded_time[1], recorded_time[2], recorded_time[3]);

	printf("Speedup (Sequencial x OpenMP): %.6f\n", recorded_time[0] / recorded_time[2]);
	printf("Speedup (Seq. Em Bloco x OpenMP Em Bloco): %.6f\n", recorded_time[1] / recorded_time[3]);

	return 0;
}
