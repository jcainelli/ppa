#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toolsv3.h"
#include "matrizv3.h"
#include "matriz-operacoesv3.h"
#include "matriz-operacoes-threads.h"

// Variavies Globais Multi Threads 
mymatriz mat_a, mat_b;
mymatriz mat_mult;

int num_thrd = 3;   // Sendo Mestre


float tempoMedioExecutacao(int nr_execucoes, float *tempos){
	float soma = 0;

	for(int i = 0; i < nr_execucoes; i++){
		soma = soma + tempos[i];
	}
	return soma / nr_execucoes;
}


int *multiplicarThMain(void* pNr_thread)
{
  	int nr_thread = (int)pNr_thread;   // retrive the slice info

	if (nr_thread > 0){
		multiplicarTh(&mat_a, &mat_b, &mat_mult, nr_thread);
	}

	return 0;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main(int argc, char *argv[]) {

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%

	// DECLARAÇÃO de VARIÁVEIS	
	mymatriz **mmultbloco, **mmult;
	char filename[100];
	FILE *fmat;
	int nr_line;
	int *vet_line = NULL;
	int N, M, La, Lb;
	double start_time, end_time;
	
	// Variaveis de tempo
	int numero_testes = 10;
	float tempos_sequencial [10];
	float tempo_thread [10];
	float tempos_bloco [10];

	// Agrupador de Threads
	pthread_t* thread;  

	// Matrizes
	matriz_bloco_t **Vsubmat_a = NULL;
	matriz_bloco_t **Vsubmat_b = NULL;
	matriz_bloco_t **Vsubmat_c = NULL;
	int nro_submatrizes=2;

	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	if (argc != 3){
		printf ("ERRO: Numero de parametros %s <matriz_a> <matriz_b>\n", argv[0]);
		exit (1);
	}

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//                Leitura da Matriz A (arquivo)
	fmat = fopen(argv[1],"r");
	if (fmat == NULL) {
		printf("Error: Na abertura dos arquivos.");
		exit(1);
	}
	extrai_parametros_matriz(fmat, &N, &La, &vet_line, &nr_line);

	mat_a.matriz = NULL;
	mat_a.lin = N;
	mat_a.col = La;
	if (malocar(&mat_a)) {
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
	if (malocar(&mat_b)) {
		printf ("ERROR: Out of memory\n");
	}
	filein_matriz (mat_b.matriz, Lb, M, fmat, vet_line, nr_line);
	free (vet_line);
	fclose(fmat);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%
	
	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//               Operações de Multiplicação
	mmult = (mymatriz **) malloc (sizeof(mymatriz *));
	for(int i = 0; i < numero_testes; i++){
		printf("\n ##### multiplicar_t%d de Matrizes #####\n", i);
		start_time = wtime();
		mmult[0] = mmultiplicar(&mat_a, &mat_b, 0);
		end_time = wtime();

		//printf("\tRuntime: %f\n", end_time - start_time);
		tempos_sequencial[i] = (end_time - start_time);

		sprintf(filename, "mult_t1.result");
		fmat = fopen(filename,"w");
		fileout_matriz(mmult[0], fmat);
		fclose(fmat);		
	}
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//               Operações de Multiplicação - Threads	
	thread = (pthread_t*) malloc(num_thrd*sizeof(pthread_t));
	mat_mult.matriz = NULL;
    mat_mult.lin = mat_a.lin;
    mat_mult.col = mat_b.col;

	if (malocar(&mat_mult)){
		printf ("ERRO (MSOMAR) - Erro ao alocar matriz Soma\n");
		return NULL;
	}

	for(int i = 0; i < numero_testes; i++){
		printf("\n ##### multiplicar_t%d de Matrizes - Threads #####\n", i);

		mzerar(&mat_mult);
		
		start_time = wtime();
		for (int i = 1; i < num_thrd; i++){
			if (pthread_create (&thread[i], NULL, multiplicarThMain, (void*)i) != 0 ){
				perror("Erro na criação da Thread");
				free(thread);
				exit(-1);
			}
		}

		// Junção das Threads
		for (int i = 1; i < num_thrd; i++){
			pthread_join (thread[i], NULL);
		}

		end_time = wtime();

		tempo_thread[i] = (end_time - start_time);
	}
		

	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//               Operações de Multiplicação (em bloco)
	mmultbloco = (mymatriz **) malloc (sizeof(mymatriz *));
	for(int i = 0; i < numero_testes; i++){
		printf("\n ##### multiplicar_t%d de Matrizes - MULTIPLICACAO <por bloco> #####\n", i);
		start_time = wtime();

		Vsubmat_a = particionar_matriz (mat_a.matriz, N, La, 1, 2);
		Vsubmat_b = particionar_matriz (mat_b.matriz, Lb, M, 0, 2);
		Vsubmat_c = csubmatrizv2 (N, M, nro_submatrizes);

		mmsubmatriz (Vsubmat_a[0], Vsubmat_b[0], Vsubmat_c[0]);
		mmsubmatriz (Vsubmat_a[1], Vsubmat_b[1], Vsubmat_c[1]); 
		mmultbloco[0] = msomar(Vsubmat_c[0]->matriz, Vsubmat_c[1]->matriz, 1);

		end_time = wtime();	

		tempos_bloco[i] = (end_time - start_time);

		sprintf(filename, "mult_t1.result");
		fmat = fopen(filename,"w");
		fileout_matriz(mmultbloco[0], fmat);
		fclose(fmat);
	}
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//              Comparação dos resultados
	printf("\n ##### Comparação dos resultados da Multiplicação de matrizes #####\n");
	printf("Comparação Matriz Sequencia X Matriz Bloco : ");
	mcomparar(mmult[0], mmultbloco[0]);
	printf("\n");

	printf("Comparação Matriz Sequencia X Thread : ");
	mcomparar(mmult[0], &mat_mult);
	printf("\n");

	printf("Tempo médio Sequencial: %f \n" , tempoMedioExecutacao(numero_testes, tempos_sequencial));
	printf("Tempo médio Threads   : %f \n" , tempoMedioExecutacao(numero_testes, tempo_thread));
	printf("Tempo médio Blocos    : %f \n" , tempoMedioExecutacao(numero_testes, tempos_bloco));

	printf("Speed-Up Thread       : %f \n" , (tempoMedioExecutacao(numero_testes, tempos_sequencial) / tempoMedioExecutacao(numero_testes, tempo_thread)));
/*
	(print) COMPARAR MATRIZ_SeqC c/ MATRIZ_SeqBlC
	(print) COMPARAR MATRIZ_SeqC c/ MATRIZ_ThreadC
	(print) COMPARAR MATRIZ_SeqC c/ MATRIZ_ThreadBlC
	
	(print) TEMPO_MEDIO MATRIZ_SeqC 
	(print) TEMPO_MEDIO MATRIZ_SeqBlC
	(print) TEMPO_MEDIO MATRIZ_ThreadC
	(print) TEMPO_MEDIO MATRIZ_ThreadBlC
	(print) SPEEDUP (MATRIZ_C)
	(print) SPEEDUP (MATRIZ_BLC)

*/

	//printf("[mult_t0 vs multbloco_t0]\t");
	//mcomparar (mmult[0],mmultbloco[0]);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//                   Liberação de memória
	mliberar(mmult[0]);
	free (mmult[0]);
	mliberar(mmultbloco[0]);
	free (mmultbloco[0]);

	mliberar(&mat_a);
	mliberar(&mat_b);
	mliberar(&mat_mult);
	free(mmult);
	free(mmultbloco);
	free(thread);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	return 0;
}

