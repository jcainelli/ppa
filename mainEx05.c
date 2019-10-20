#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "toolsv3.h"
#include "matrizv3.h"
#include "matriz-operacoesv3.h"
#include "matriz-operacoes-omp.h"


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

	if (argc != 4){
		printf ("ERRO: Numero de parametros %s <matriz_a> <matriz_b> <nthreads>\n", argv[0]);
		exit (1);
	}


	if (argv[3] != NULL){
		nthreads = atoi(argv[3]);
	}

	// %%%%%%%%%%%%%%%%%%%%%%%% Leitura da Matriz A (arquivo) %%%%%%%%%%%%%%%%%%%%%%%%
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
	

	// %%%%%%%%%%%%%%%%%%%%%%%% Leitura da Matriz B (arquivo) %%%%%%%%%%%%%%%%%%%%%%%%
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


	// %%%%%%%%%%%%%%%%%%%%%%%% INI - SEQUENCIAL - multiplicar_t1 de Matrizes - MULTIPLICACAO  %%%%%%%%%%%%%%%%%%%%%%%%
	mmult = (mymatriz **) malloc (sizeof(mymatriz *));

	for(int i = 0; i < 10; i++) {
		start_time = wtime();
		mmult[0] = mmultiplicar(&mat_a, &mat_b, 1);
		end_time = wtime();
		seqTempos[i] = end_time - start_time;
	}

	
	sprintf(filename, "mult_t1.result");
	fmat = fopen(filename,"w");
	fileout_matriz(mmult[0], fmat);
	fclose(fmat);

	for(int i = 0; i < 10; i++) {
		seqTot += seqTempos[i];
	}
	seqMed = seqTot/10;


	// %%%%%%%%%%%%%%%%%%%%%%%% INI - PARALELO - multiplicar_t1 de Matrizes - MULTIPLICACAO %%%%%%%%%%%%%%%%%%%%%%%%
    mmultPara = (mymatriz **)malloc(sizeof(mymatriz *));
    mmultPara[0] = malloc(sizeof(mymatriz));
    mmultPara[0]->matriz = NULL;
    mmultPara[0]->lin = mat_a.lin;
    mmultPara[0]->col = mat_b.col;
	
    //realiza a alocação de memória para matriz resultado
	if (malocar(mmultPara[0])) {
		printf ("ERROR: Out of memory\n");
	}

	for(int i = 0; i < 10; i++) {
		start_time = wtime();
		mzerar(mmultPara[0]);

		multiplicarOMP(&mat_a, &mat_b, mmultPara[0], nthreads);

		end_time = wtime();
		paraTempos[i] = end_time - start_time;
	}

	fmatpara_c = fopen("outPara.map-result","w");
	fileout_matriz(mmultPara[0], fmatpara_c);

	for(int i = 0; i < 10; i++) {
		paraTot += paraTempos[i];
	}
	paraMed  = paraTot/10;



	// %%%%%%%%%%%%%%%%%%%%%%%% Operações de Multiplicação (em bloco) %%%%%%%%%%%%%%%%%%%%%%%%
	mmultbloco = (mymatriz **) malloc (sizeof(mymatriz *));

	for(int i = 0; i < 10; i++) {
		start_time = wtime();
		Vsubmat_a = particionar_matriz (mat_a.matriz, N, La, 1, 2);
		Vsubmat_b = particionar_matriz (mat_b.matriz, Lb, M, 0, 2);
		Vsubmat_c = csubmatrizv2 (N, M, nro_submatrizes);

		mmsubmatriz (Vsubmat_a[0], Vsubmat_b[0], Vsubmat_c[0]);
		mmsubmatriz (Vsubmat_a[1], Vsubmat_b[1], Vsubmat_c[1]); 

		mmultbloco[0] = msomar(Vsubmat_c[0]->matriz, Vsubmat_c[1]->matriz, 1);
		end_time = wtime();

		seqBlocosTempos[i] = end_time - start_time;
	}
	sprintf(filename, "multBlocoSeq.result");
	fmat = fopen(filename,"w");
	fileout_matriz(mmultbloco[0], fmat);
	fclose(fmat);

	for(int i = 0; i < 10; i++) {
		seqBlocoTot += seqBlocosTempos[i];
	}
	seqBlocoMed = seqBlocoTot/10;

	printf("P1\n");

	// %%%%%%%%%%%%%%%%%%%%%%%% INI - PARALELA - multiplicar_t1 de Matrizes - MULTIPLICACAO <por bloco> %%%%%%%%%%%%%%%%%%%%%%%%
	mmultblocoPara = (mymatriz **) malloc (sizeof(mymatriz *));

	for(int iCont = 0; iCont < 10; iCont++) {

		start_time = wtime();
		Vsubmat_a = particionar_matriz (mat_a.matriz, N, La, 1, 2);
		Vsubmat_b = particionar_matriz (mat_b.matriz, Lb, M, 0, 2);
		Vsubmat_c = csubmatrizv2 (N, M, nro_submatrizes);
		
		start_time = wtime();
		// Aqui fixo por 2 porque so foi separado em 2 partes
		#pragma omp parallel for num_threads(2)		
		for (int i = 0; i < 2; i++){
			multiplicarOMPblocos (Vsubmat_a[i], Vsubmat_b[i], Vsubmat_c[i]);
		}

		mmultblocoPara[0] = msomar(Vsubmat_c[0]->matriz, Vsubmat_c[1]->matriz, 1);

		end_time = wtime();
		paraBlocosTempos[iCont] = end_time - start_time;
	}
	
	sprintf(filename, "multBlocoPara.result");
	fmat = fopen(filename,"w");
	fileout_matriz(mmultblocoPara[0], fmat);
	fclose(fmat);

	for(int i = 0; i < 10; i++) {
		paraBlocoTot += paraBlocosTempos[i];
	}
	paraBlocoMed  = paraBlocoTot/10;
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% Comparação dos resultados %%%%%%%%%%%%%%%%%%%%%%%%
	printf("\n\n##### Comparação dos resultados da Multiplicação de Matrizes #####\n");

	printf("[Companrando -> Sequencial mult[0] vs Sequencial Bloco multbloco[0]] : ");
	mcomparar (mmult[0],mmultbloco[0]);
	printf("[Companrando -> Sequencial mult[0] vs Paralelo mmultPara[0] : ");
	mcomparar (mmult[0], mmultPara[0]);
	printf("[Companrando -> Sequencial mult[0] vs Paralelo Bloco multblocoPara[0]] : ");
	mcomparar (mmult[0],mmultblocoPara[0]);

	printf("\n##Tempo Medio 10 Multiplicações Sequencial..............[%f]\n", seqMed);
	printf("##Tempo Medio 10 Multiplicações Sequencial Bloco........[%f]\n", seqBlocoMed);
	printf("##Tempo Medio 10 Multiplicações Paralelo................[%f]\n", paraMed);
	printf("##Tempo Medio 10 Multiplicações Paralelo Bloco..........[%f]\n", paraBlocoMed);	
	printf("\n##SpeedUp Multiplicacao Sequencial / Paralela ..........[%f]\n", seqMed / paraMed);
	printf("##SpeedUp Multiplicacao Bloco Sequencial / Paralela.....[%f]\n\n", seqBlocoMed / paraBlocoMed);

	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	
	// %%%%%%%%%%%%%%%%%%%%%%%% LIBERACAO MEMORIA %%%%%%%%%%%%%%%%%%%%%%%%
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

	return 0;
}
