#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "toolsv3.h"
#include "matrizv3.h"
#include "matriz-operacoesv3.h"
#include "matriz-operacoes-mpi.h"

#define MATSIZE 500
#define MASTER 0               /* taskid of first task */
#define FROM_MASTER 1          /* setting a message type */
#define FROM_WORKER 2          /* setting a message type */


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main(int argc, char *argv[]) {

	// DECLARAÇÃO de VARIÁVEIS
	mymatriz mat_a, mat_b;
	mymatriz **mmult, **mmultbloco /*, **mmultPara  **mmultblocoPara, */;
	mymatriz *mmultPara;
	char filename[100];
	FILE *fmat /**fmatpara_c*/;
	int *vet_line = NULL;
	int nr_line, N, M, La, Lb, nro_submatrizes = 2;
	double seqTempos[10], seqBlocosTempos[10] /*, paraTempos[10], paraBlocosTempos[10]*/;	
	double seqTot = 0, seqBlocoTot = 0, seqMed = 0, seqBlocoMed  = 0, start_time = 0, end_time = 0 /* paraTot = 0 paraBlocoTot = 0*/;
	matriz_bloco_t **Vsubmat_a = NULL, **Vsubmat_b = NULL, **Vsubmat_c = NULL;

	int	numtasks,              /* number of tasks in partition */
	//taskid,                /* a task identifier */
	numworkers,            /* number of worker tasks */
	source,                /* task id of message source */
	dest,                  /* task id of message destination */
	mtype,                 /* message type */
	rows,                  /* rows of matrix A sent to each worker */
	//cols,                  /* cols of matrix B sent to each worker */
	averow, extra, offset, /* used to determine rows sent to each worker */
	i, j, k, /*rc,*/           /* misc */
	NRA, NCA, NCB;


	int parameters [4];


	// PARAMETERS VALIDADE
	if (argc != 3){
		printf ("ERRO: Numero de parametros %s <matriz_a> <matriz_b> <nthreads>\n", argv[0]);
		exit (1);
	}
	
	// MPI VARIABLES
	int rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Status status;

	numworkers = numtasks-1;

	if(rank == 0) {

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
	
		// %%%%%%%%%%%%%%%%%%%%%%%% Operações de Multiplicação Sequencial (em bloco) %%%%%%%%%%%%%%%%%%%%%%%%
		mmultbloco = (mymatriz **) malloc (sizeof(mymatriz *));

		for(int i = 0; i < 10; i++) {
			start_time = wtime();
			Vsubmat_a = particionar_matriz (mat_a.matriz, N, La, 1, 2);
			Vsubmat_b = particionar_matriz (mat_b.matriz, Lb, M, 0, 2);
			Vsubmat_c = csubmatrizv2 (N, M, nro_submatrizes);
		}

		for(int i = 0; i < 10; i++) {
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

		// %%%%%%%%%%%%%%%%%%%%%%%% INI - PARALELO - multiplicar_t1 de Matrizes - MULTIPLICACAO %%%%%%%%%%%%%%%%%%%%%%%%		

		int a[mat_a.lin][mat_a.col];
		int b[mat_b.lin][mat_b.col];
		int c[mat_a.lin][mat_b.col]; 


		for(int l = 0; l < mat_a.lin; l++)
			for (int c = 0; c < mat_a.col; c++)
				a[l][c] = mat_a.matriz[l][c];

		for(int l = 0; l < mat_b.lin; l++)
			for (int c = 0; c < mat_b.col; c++)
				b[l][c] = mat_b.matriz[l][c];

		NRA = mat_a.lin;
		NCA = mat_a.col;
		NCB = mat_b.col;
		averow = NRA/numworkers;
		extra = NRA%numworkers;
		offset = 0;
		mtype = FROM_MASTER;

		printf("numworkers : %d\n" , numworkers);
		// Parametros
		parameters[1] = mat_a.lin;
		parameters[0] = mat_a.col;
		parameters[2] = mat_b.lin;
		parameters[3] = mat_b.col;
		// Aloca memoria para o resultado da multiplicação
		mmultPara = (mymatriz*) malloc(sizeof(mymatriz));
		mmultPara->matriz = NULL;
		mmultPara->lin = mat_a.lin;
		mmultPara->col = mat_b.col; 
		
		for (dest=1; dest<=numworkers; dest++)
		{
			rows = (dest <= extra) ? averow+1 : averow;   	

			MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			MPI_Send(&parameters, 4, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			MPI_Send(&a[offset][0], rows*NCA, MPI_INT, dest, mtype,MPI_COMM_WORLD);
			MPI_Send(&b, NCA*NCB, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			offset = offset + rows;

			//printf("Sending %d rows to task %d offset=%d\n",rows,dest,offset);
			//printf("# Send Params : %d %d %d %d\n", parameters[0], parameters[1], parameters[2], parameters[3]);

			/* MPI_Send(&cols, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);		
			*/
		}

		/* Receive results from worker tasks */
		mtype = FROM_WORKER;
		printf("antesdo resultado : %d\n", numworkers);
		

		
		for (i=1; i<=numworkers; i++){
			source = i;
			printf("Result : %d\n", source);
			
			MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
			MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
			//MPI_Recv(&c[offset][0], rows*NCB, MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);

			// SOMAR O RETORNO
/*			for (int l = 0; l < mat_a.lin; l++){
				for (int c = 0; c < mat_b.col; c++){
					mmultPara[0]->matriz[l][c] = mmultPara[0]->matriz[l][c] + c[l][c];
				}
			}*/
			
		}

		// %%%%%%%%%%%%%%%%%%%%%%%% Comparação dos resultados %%%%%%%%%%%%%%%%%%%%%%%%
		printf("\n\n##### Comparação dos resultados da Multiplicação de Matrizes #####\n");
	
		printf("[Companrando -> Sequencial mult[0] vs Sequencial Bloco multbloco[0]] : ");
		mcomparar (mmult[0],mmultbloco[0]);
		printf("[Companrando -> Sequencial mult[0] vs Paralelo mmultPara[0] : ");
		/*
		mcomparar (mmult[0], mmultPara[0]);
		printf("[Companrando -> Sequencial mult[0] vs Paralelo Bloco multblocoPara[0]] : ");
		mcomparar (mmult[0],mmultblocoPara[0]);
	*/
		printf("\n##Tempo Medio 10 Multiplicações Sequencial..............[%f]\n", seqMed);
		printf("##Tempo Medio 10 Multiplicações Sequencial Bloco........[%f]\n", seqBlocoMed);
	/*	printf("##Tempo Medio 10 Multiplicações Paralelo................[%f]\n", paraMed);
		printf("##Tempo Medio 10 Multiplicações Paralelo Bloco..........[%f]\n\n", paraBlocoMed);	
		printf("##SpeedUp Multiplicacao Sequencial / Paralela ..........[%f]\n", seqMed / paraMed);
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
		free(mmultblocoPara);*/
	} 
	else { // Rank <> 0
		printf("%d esta esperando\n", rank);
				
		mtype = FROM_MASTER;		
		
		MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
		
		MPI_Recv(&parameters, 4, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);

		NCA = parameters[1];
		NCB = parameters[3];
		
		int a[parameters[0]] [parameters[1]];
		int b[parameters[2]] [parameters[3]];
		int c[parameters[0]] [parameters[3]]; 

		MPI_Recv(&a, rows*NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv(&b, NCA*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
		
		for (k=0; k<NCB; k++)
			for (i=0; i<rows; i++){
				c[i][k] = 0.0;
				for (j=0; j<NCA; j++)
					c[i][k] = c[i][k] + a[i][j] * b[j][k];
			}
		

		mtype = FROM_WORKER;
		MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
		MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
		//MPI_Send(&c, rows*NCB, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);		
	}

	MPI_Finalize();
	return 0;
}
