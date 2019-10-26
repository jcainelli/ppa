#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "toolsv3.h"
#include "matrizv3.h"
#include "matriz-operacoesv3.h"
#include "matriz-operacoes-threads.h"
#include <omp.h>

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main(int argc, char *argv[]) {

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	// DECLARAÇÃO de VARIÁVEIS
	mymatriz mat_a, mat_b, mat_th_result;
	mymatriz **mmultbloco, **mmult, **mmulth, **mmultblocoth;
	char filename[100];
	FILE *fmat;
	int nr_line;
	int *vet_line = NULL;
	int N, M, La, Lb;
	double start_time, end_time;

    matriz_bloco_t **Vsubmat_a = NULL;
    matriz_bloco_t **Vsubmat_b = NULL;
	matriz_bloco_t **Vsubmat_c = NULL;
	//ntasks = ;
    int nro_submatrizes=atoi(argv[3]);

	//printf("nr %d", nro_submatrizes);
	

	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	if (argc != 4){
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
	//return 1;
	mat_a.matriz = NULL;
	mat_a.lin = N;
	mat_a.col = La;
	if (!malocar(&mat_a)) {
		printf ("ERROR: Out of memory -- depois de extrair parametros\n");
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
		printf ("ERROR: Out of memory\n -- malocar mat_b depois de extrair parametros");
	}
	filein_matriz (mat_b.matriz, Lb, M, fmat, vet_line, nr_line);
	free (vet_line);
	fclose(fmat);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	printf("\n%%%%%%%%%%%%%%%%\n");
	
	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//               Operações de Multiplicação
	mmult = (mymatriz **) malloc (sizeof(mymatriz *));
	printf("\n ##### multiplicar_t1 de Matrizes #####\n");
	//loop 10 vezes
	double mediamult = 0;
	for (int l=0; l<10; l++){
		start_time = wtime();
		mmult[0] = mmultiplicar(&mat_a, &mat_b, 1);
		end_time = wtime();
		//printf("\t Runtime[%d]:%f \n", l, end_time-start_time);
		mediamult += end_time - start_time;
	}

	mediamult = mediamult/10;
	printf("\t Média de Runtime: %f\n", mediamult);
	
	mimprimir(mmult[0]);
	//printf("\tRuntime: %f\n", end_time - start_time);
	sprintf(filename, "mult_t1.result");
	fmat = fopen(filename,"w");
	fileout_matriz(mmult[0], fmat);
	fclose(fmat);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//               Operações de Multiplicação (em bloco)
	mmultbloco = (mymatriz **) malloc (sizeof(mymatriz *));
	printf("\n ##### multiplicar_t1 de Matrizes em Bloco #####\n");
	
	
	double mediamultbloco = 0;
	for (int l=0; l<10; l++){
		start_time = wtime();
		Vsubmat_a = particionar_matriz (mat_a.matriz, N, La, 1, nro_submatrizes);
		Vsubmat_b = particionar_matriz (mat_b.matriz, Lb, M, 0, nro_submatrizes);
		Vsubmat_c = csubmatrizv2 (N, M, nro_submatrizes);

		for(int nsub = 0; nsub < nro_submatrizes; nsub++){
			mmsubmatriz(Vsubmat_a[nsub], Vsubmat_b[nsub], Vsubmat_c[nsub]);
		}

		for(int nsub = 0; nsub < nro_submatrizes/2; nsub++){
			mmultbloco[nsub] = msomar(Vsubmat_c[nsub+nsub]->matriz,Vsubmat_c[nsub+nsub+1]->matriz, 1);
		}

	    for(int nsub = 1 ; nsub< nro_submatrizes/2; nsub++){
			mmultbloco[0] = msomar(mmultbloco[nsub], mmultbloco[0],1);
		}
		end_time = wtime();

		mediamultbloco += end_time - start_time;
	}

	mediamultbloco = mediamultbloco/10;
	printf("\t Média de Runtime: %f\n", mediamultbloco);

	mimprimir(mmultbloco[0]);
	//printf("\tRuntime: %f\n", end_time - start_time);
	sprintf(filename, "mult_t1_bloco.result");
	fmat = fopen(filename,"w");
	fileout_matriz(mmultbloco[0], fmat);
	fclose(fmat);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%


	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//               Operações de Multiplicação com Threads
	
	printf("\n ##### multiplicar_t1 de Matrizes com Threads #####\n");
	
	mmulth = (mymatriz **) malloc (sizeof(mymatriz *));

	mat_th_result.matriz = NULL;
	mat_th_result.lin = N;
	mat_th_result.col = La;

	if (!malocar(&mat_th_result)) {
		printf ("Erro em malocar matresult\n");
	}

	mzerar(&mat_th_result);

	param_t *args;
	//pthread_t *threads;

	int nthreads = nro_submatrizes;
	int tid;
	printf("ntrhread = %d",nthreads);
	
	

//	threads = (pthread_t *) malloc(nro_submatrizes * sizeof(pthread_t));
    args = (param_t *) malloc(nro_submatrizes * sizeof(param_t));
	
	double mediamultth = 0;
	for(int lt = 0; lt < 10; lt ++){
		start_time = wtime();
		mzerar(&mat_th_result);
		
		#pragma omp parallel private(nthreads, tid) 
		{
			
			tid = omp_get_thread_num();
			nthreads = omp_get_num_threads();
			args[tid].tid = tid;
			args[tid].ntasks = nthreads;
			args[tid].limite = mat_a.col;
			args[tid].mata = &mat_a;
			args[tid].matb = &mat_b;
			args[tid].matresult = &mat_th_result;
			mmultiplicarth(args+tid);
			
		}
			
		
		//printf("\t Runtime[%d]:%f \n", lt, end_time-start_time);
	
		end_time = wtime();
		mediamultth += end_time - start_time;
	}
		mmulth[0] = args->matresult;
	mediamultth = mediamultth/10;
   	printf("\t Média de Runtime em Threads: %f\n", mediamultth);
	mimprimir(mmulth[0]);
	//printf("\tRuntime em thread: %f\n", end_time - start_time);
	sprintf(filename, "mult_t1_th.result");
	fmat = fopen(filename,"w");
	fileout_matriz(mmulth[0], fmat);
	fclose(fmat);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%


	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//               Operações de Multiplicação (em bloco) com THREADS
	mmultblocoth = (mymatriz **) malloc (sizeof(mymatriz *));
	matriz_bloco_t_th *argss;

	argss = NULL;
	
	printf("\n ##### multiplicar_t1 de Matrizes em Bloco com Threads #####\n");

	Vsubmat_a = particionar_matriz (mat_a.matriz, N, La, 1, 2);
	Vsubmat_b = particionar_matriz (mat_b.matriz, Lb, M, 0, 2);
	Vsubmat_c = csubmatrizv2 (N, M, nro_submatrizes);

    argss = (matriz_bloco_t_th *) malloc(nro_submatrizes * sizeof(matriz_bloco_t_th));
	
	double mediamultthbloco = 0;
for (int l=0; l<10; l++){
		start_time = wtime();

		nthreads = 2;

			#pragma omp parallel private(nthreads, tid) 
		{
			tid = omp_get_thread_num();
			nthreads = omp_get_num_threads();
			argss[tid].tid = tid;
			argss[tid].ntasks = 2;
			argss[tid].limite = 2;
			argss[tid].blocoa = Vsubmat_a[tid];
			argss[tid].blocob = Vsubmat_b[tid];
			argss[tid].blococ = Vsubmat_c[tid];
			mmsubmatrizth(argss+tid);

		}
			
	
		
		end_time = wtime();


		mmultblocoth[0] = msomar(Vsubmat_c[0]->matriz,Vsubmat_c[1]->matriz, 1);	
		mediamultthbloco += end_time - start_time;
	}
	
	mediamultthbloco = mediamultthbloco/10;
	printf("\t Média de Runtime: %f\n", mediamultthbloco);

	mimprimir(mmultblocoth[0]);
	//printf("\tRuntime: %f\n", end_time - start_time);
	sprintf(filename, "mult_t1_th_bloco.result");
	fmat = fopen(filename,"w");
	fileout_matriz(mmultblocoth[0], fmat);
	fclose(fmat);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%



	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//              Comparação dos resultados
	printf("\n ##### Comparação dos resultados da Multiplicação de matrizes #####\n");
	printf("[mult_t0 vs multbloco_t0]\t");
	mcomparar (mmult[0],mmultbloco[0]);

	printf("[mult_t0 vs multthread_t0]\t");
	mcomparar (mmult[0],mmulth[0]);

	printf("[mult_t0 vs multblocothread_t0]\t");
	mcomparar (mmult[0],mmultblocoth[0]);
	printf("---------------------------- \n");
	printf("Tempo médio Sequêncial: %f \n", mediamult);
	printf("Tempo médio Sequêncial em Bloco: %f \n", mediamultbloco);
	printf("Tempo médio em Threads: %f \n", mediamultth);
	printf("Tempo médio em  Threads em Blocos: %f \n", mediamultthbloco);
	printf("---------------------------- \n");
	double speedupth,speedupthbloco;
	speedupth = mediamult/mediamultth; 
	speedupthbloco = mediamultbloco/mediamultthbloco; 

	printf("SpeedUp em Threads: Sequecnia / Threads =  %f \n", speedupth);
	printf("SpeedUp de Blocos em Threads: Seq em Bloco / Seq em Bloco em Threads %f \n", speedupthbloco);

	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//                   Liberação de memória
	free(mmult[0]);
	free(mmultbloco[0]);
    mliberar(mmultblocoth[0]);
	mliberar(mmulth[0]);
	mliberar(&mat_a);
	mliberar(&mat_b);
	free(mmult);
	free(mmultbloco);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	return 0;
}
