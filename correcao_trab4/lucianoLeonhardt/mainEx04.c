#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "toolsv3.h"
#include "matrizv3.h"
#include "matriz-operacoesv3.h"
#include "matriz-operacoes-threads.h"

void *execMultThread (void *arg) {
  param_t *p = (param_t *) arg;

  multiplicarIKJThread(p->mat_a, p->mat_b, p->mat_c, p->tid, p->ntasks);
  return NULL;
}
void *execMultiThreadBlocos (void *arg) {
  param_t *p = (param_t *) arg;

  multiplicaBlocoThread(p->mat_bloco_a, p->mat_bloco_b, p->mat_bloco_c);

  return NULL;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main(int argc, char *argv[]) {

	// DECLARAÇÃO de VARIÁVEIS
	mymatriz mat_a, mat_b;
	mymatriz **mmultbloco, **mmultblocoPara, **mmult, **mmultPara;
	char filename[100];
	FILE *fmat;
	int nr_line;
	int *vet_line = NULL;
	int N, M, La, Lb;
	double start_time, end_time;
	double seqTempos[10], paraTempos[10], seqBlocosTempos[10], paraBlocosTempos[10];
	double seqTot = 0;
	double seqBlocoTot = 0;
	double paraTot = 0;
	double paraBlocoTot = 0;
	double seqMed, seqBlocoMed, paraMed, paraBlocoMed = 0;

	matriz_bloco_t **Vsubmat_a = NULL;
	matriz_bloco_t **Vsubmat_b = NULL;
	matriz_bloco_t **Vsubmat_c = NULL;
	int nro_submatrizes=2;

	// Variaveis paralelo regular
	FILE *fmatpara_c;
	int nthreads = 2;

	pthread_t *threadsPara,*threadsBlocos  = NULL;

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    param_t *args;

	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%
	if (argc != 4){
		printf ("ERRO: Numero de parametros %s <matriz_a> <matriz_b> <nthreads>\n", argv[0]);
		exit (1);
	}

	//nthreads  = atoi(argv[3]);
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
	printf("I-Imprimindo Matriz A.\n");
	mimprimir(&mat_a);
	printf("F-Imprimindo Matriz A.\n");

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
	printf("I-Imprimindo Matriz B.\n");
	mimprimir(&mat_b);
	printf("F-Imprimindo Matriz B.\n");
	printf("\n%%%%%%%%%%%%%%%%\n");
	
	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//               Operações de Multiplicação
	mmult = (mymatriz **) malloc (sizeof(mymatriz *));
	printf("\n ##### Multiplicar - Sequencial #####\n");
	printf("\n ##### INI - SEQUENCIAL - multiplicar_t1 de Matrizes - MULTIPLICACAO #####\n");

	// Executa 10 vezes
	for(int i = 0; i < 10; i++) {
		start_time = wtime();
		mmult[0] = mmultiplicar(&mat_a, &mat_b, 1);
		end_time = wtime();
		//mimprimir(mmult[0]);
		seqTempos[i] = end_time - start_time;
		printf("\tRuntime Multiplicação Sequencial:[%d] %f\n", i, seqTempos[i]);
	}
	printf("\n ##### FIM - SEQUENCIAL - multiplicar_t1 de Matrizes - MULTIPLICACAO #####\n");
	sprintf(filename, "mult_t1.result");
	fmat = fopen(filename,"w");
	fileout_matriz(mmult[0], fmat);
	fclose(fmat);

	// Agrega as 10 execucoes
	for(int i = 0; i < 10; i++) {
		seqTot += seqTempos[i];
	}
	seqMed = seqTot/10;
	printf("\tRuntime medio - 10 execucoes - Multiplicar Sequencial: %f\n\n", seqMed);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	printf("\n ##### INI - PARALELO - multiplicar_t1 de Matrizes - MULTIPLICACAO #####\n");
/************************************************************************************************************************/
    mmultPara = (mymatriz **)malloc(sizeof(mymatriz *));
    mmultPara[0] = malloc(sizeof(mymatriz));
    mmultPara[0]->matriz = NULL;
    mmultPara[0]->lin = mat_a.lin;
    mmultPara[0]->col = mat_b.col;
	
    //realiza a alocação de memória para matriz resultado
	if (malocar(mmultPara[0])) {
		printf ("ERROR: Out of memory\n");
	}

	// Executa 10 vezes
	for(int i = 0; i < 10; i++) {

		start_time = wtime();
		mzerar(mmultPara[0]);
		// Iniciando threads para multiplicação
		threadsPara = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
		args = (param_t *) malloc(nthreads * sizeof(param_t));

        for (int i = 0; i < nthreads; i++)
        {
            args[i].tid = i;
            args[i].ntasks = nthreads;
			args[i].mat_a  = &mat_a;
			args[i].mat_b  = &mat_b;
			args[i].mat_c  = mmultPara[0];

            pthread_create(&threadsPara[i], NULL, execMultThread, (void *) (args+i));
        }
        for (int i = 0; i < nthreads; i++)
        {
            pthread_join(threadsPara[i], NULL);
        }
		end_time = wtime();
		paraTempos[i] = end_time - start_time;
		printf("\tRuntime Multiplicação Paralelo:[%d] %f\n", i, paraTempos[i]);
	}
	printf("\n ##### FIM - PARALELO - multiplicar_t1 de Matrizes - MULTIPLICACAO #####\n");
	fmatpara_c = fopen("outPara.map-result","w");
	fileout_matriz(mmultPara[0], fmatpara_c);
	//Zera demais para não mudar o resultado
	free(threadsPara);

/************************************************************************************************************************/

	// Agrega as 10 execuçoes
	for(int i = 0; i < 10; i++) {
		paraTot += paraTempos[i];
	}
	paraMed  = paraTot/10;
	printf("\tRuntime medio - 10 execucoes - Multiplicar Threads: %f\n\n", paraMed);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//               Operações de Multiplicação (em bloco)
	mmultbloco = (mymatriz **) malloc (sizeof(mymatriz *));
	printf("\n ##### INI - SEQUENCIAL - multiplicar_t1 de Matrizes - MULTIPLICACAO <por bloco> #####\n");
	// Executa 10 vezes
	for(int i = 0; i < 10; i++) {
		start_time = wtime();
		Vsubmat_a = particionar_matriz (mat_a.matriz, N, La, 1, 2);
		Vsubmat_b = particionar_matriz (mat_b.matriz, Lb, M, 0, 2);
		Vsubmat_c = csubmatrizv2 (N, M, nro_submatrizes);
		mmsubmatriz (Vsubmat_a[0], Vsubmat_b[0], Vsubmat_c[0]);
		mmsubmatriz (Vsubmat_a[1], Vsubmat_b[1], Vsubmat_c[1]); 
		mmultbloco[0] = msomar(Vsubmat_c[0]->matriz, Vsubmat_c[1]->matriz, 1);
		end_time = wtime();
		//mimprimir(mmultbloco[0]);
		seqBlocosTempos[i] = end_time - start_time;
		printf("\tRuntime Multiplicação Bloco Sequencial:[%d] %f\n", i, seqBlocosTempos[i]);
	}
	sprintf(filename, "multBlocoSeq.result");
	fmat = fopen(filename,"w");
	fileout_matriz(mmultbloco[0], fmat);
	fclose(fmat);

	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%
	// Agrega as 10 execucoes
	for(int i = 0; i < 10; i++) {
		seqBlocoTot += seqBlocosTempos[i];
	}
	seqBlocoMed = seqBlocoTot/10;
	printf("\tRuntime medio - 10 execucoes - Multiplicar Blocos Sequencial: %f\n", seqBlocoMed);
	printf(" ##### FIM - SEQUENCIAL - multiplicar_t1 de Matrizes - MULTIPLICACAO <por bloco> #####\n");
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	printf("\n ##### INI - PARALELA - multiplicar_t1 de Matrizes - MULTIPLICACAO <por bloco> #####\n");

	printf("\n ##### Multiplicação em Bloco Multi-Threads de Matrizes #####\n");
	mmultblocoPara = (mymatriz **) malloc (sizeof(mymatriz *));
	// Executa 10 vezes
	for(int iCont = 0; iCont < 10; iCont++) {

		start_time = wtime();
		Vsubmat_a = particionar_matriz (mat_a.matriz, N, La, 1, 2);
		Vsubmat_b = particionar_matriz (mat_b.matriz, Lb, M, 0, 2);
		Vsubmat_c = csubmatrizv2 (N, M, nro_submatrizes);
		
		threadsBlocos = (pthread_t *) malloc (sizeof(pthread_t) * nthreads);

        args = (param_t *)malloc(nthreads * sizeof(param_t));
        start_time = wtime();
        for (int i = 0; i < nthreads; i++)
        {
            args[i].tid = i;
            args[i].ntasks = nthreads;
            args[i].mat_bloco_a = Vsubmat_a[i];
            args[i].mat_bloco_b = Vsubmat_b[i];
            args[i].mat_bloco_c = Vsubmat_c[i];
            pthread_create(&threadsBlocos[i], NULL, execMultiThreadBlocos, (void *)(args + i));
        }
		for (int i=0; i<nthreads; i++){
			pthread_join(threadsBlocos[i], NULL);
		}
		mmultblocoPara[0] = msomar(Vsubmat_c[0]->matriz, Vsubmat_c[1]->matriz, 1);
		end_time = wtime();
		paraBlocosTempos[iCont] = end_time - start_time;
		printf("\tRuntime Multiplicação Bloco Paralelo:[%d] %f\n", iCont, paraBlocosTempos[iCont]);
	}
	
	//free(dbtBlocoPara);
	free(threadsBlocos);
	sprintf(filename, "multBlocoPara.result");
	fmat = fopen(filename,"w");
	fileout_matriz(mmultblocoPara[0], fmat);
	fclose(fmat);

	// Agrega as 10 execuçoes
	for(int i = 0; i < 10; i++) {
		paraBlocoTot += paraBlocosTempos[i];
	}
	paraBlocoMed  = paraBlocoTot/10;
	printf("\tRuntime medio - 10 execucoes - Multiplicação Bloco Paralelo: %f\n\n", paraBlocoMed);
	printf(" ##### FIM - PARALELA - multiplicar_t1 de Matrizes - MULTIPLICACAO <por bloco> #####\n");
	/****/
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	printf("\n\nI-Imprimindo Matriz [mmult] Sequencial.\n");
	mimprimir(mmult[0]);
	//printf("\nF-Imprimindo Matriz Sequencial.\n");

	printf("\n\nI-Imprimindo Matriz [mmultPara] Paralela.\n");
	mimprimir(mmultPara[0]);
	//printf("\nF-Imprimindo Matriz Paralela.\n");

	printf("\n\nI-Imprimindo Matriz Bloco [mmultbloco] Sequencial.\n");
	mimprimir(mmultbloco[0]);
	//printf("\nF-Imprimindo Matriz Bloco [mmultbloco] Sequencial.\n");

	printf("\n\nI-Imprimindo Matriz Bloco [mmultblocoPara] Paralela.\n");
	mimprimir(mmultblocoPara[0]);
	//printf("\nF-Imprimindo Matriz Bloco [mmultblocoPara] Paralela.\n");

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//              Comparação dos resultados
	printf("\n\n##### Comparação dos resultados da Multiplicação de Matrizes #####\n");

	printf("\n[Companrando -> Sequencial mult[0] vs Sequencial Bloco multbloco[0]]\n");
	mcomparar (mmult[0],mmultbloco[0]);
	printf("\n[Companrando -> Sequencial mult[0] vs Paralelo mmultPara[0]\n");
	mcomparar (mmult[0], mmultPara[0]);
	printf("\n[Companrando -> Sequencial mult[0] vs Paralelo Bloco multblocoPara[0]]\n");
	mcomparar (mmult[0],mmultblocoPara[0]);
	
	printf("\n##Tempo Medio 10 Multiplicações Sequencial..............[%f]\n", seqMed);
	printf("##Tempo Medio 10 Multiplicações Sequencial Bloco........[%f]\n", seqBlocoMed);
	printf("##Tempo Medio 10 Multiplicações Paralelo................[%f]\n", paraMed);
	printf("##Tempo Medio 10 Multiplicações Paralelo Bloco..........[%f]\n", paraBlocoMed);
	printf("##SpeedUp Multiplicacao Sequencial / Paralela ..........[%f]\n", seqMed / paraMed);
	printf("##SpeedUp Multiplicacao Bloco Sequencial / Paralela.....[%f]\n\n", seqBlocoMed / paraBlocoMed);

	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	
	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//                   Liberação de memória

	mliberar(mmult[0]);
	mliberar(mmultbloco[0]);
	mliberar(mmultPara[0]);
	mliberar(mmultblocoPara[0]);

	free(mmult[0]);
	free(mmultbloco[0]);
	free(mmultPara[0]);
	free(mmultblocoPara[0]);

	mliberar(&mat_a);
	mliberar(&mat_b);

	free(mmult);
	free(mmultbloco);
	free(mmultPara);
	free(mmultblocoPara);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	return 0;
}
