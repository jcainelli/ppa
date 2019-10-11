#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "toolsv3.h"
#include "matrizv3.h"
#include "matriz-operacoesv3.h"
#include "matriz-operacoes-threads.h"

typedef struct
{
    int tid;
    int ntasks;
    mymatriz *mat_a;
    mymatriz *mat_b;
    mymatriz *mat_c;
    matriz_bloco_t *mat_bloco_a;
    matriz_bloco_t *mat_bloco_b;
    matriz_bloco_t *mat_bloco_c;
} param_t;

void *exec_multi_thread(void *arg)
{
    param_t *p = (param_t *)arg;
    multiplicarTh(p->mat_a, p->mat_b, p->mat_c, p->tid, p->ntasks);
    return NULL;
}

void *exec_multi_thread_blocos(void *arg)
{
    param_t *p = (param_t *)arg;
    multiplicarThblocos(p->mat_bloco_a, p->mat_bloco_b, p->mat_bloco_c);
    return NULL;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main(int argc, char *argv[])
{
    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    // DECLARAÇÃO DE VARIÁVEIS
    mymatriz mat_a, mat_b;
    mymatriz **mmult;
    mymatriz **mmultbloco;
    mymatriz **mmultThread;
    mymatriz **mmultblocoThread;

    char filename[100];
    FILE *fmat;
    int nr_line;
    int *vet_line = NULL;
    int N, M, La, Lb;
    double start_time, end_time;

    matriz_bloco_t **Vsubmat_a = NULL;
    matriz_bloco_t **Vsubmat_b = NULL;
    matriz_bloco_t **Vsubmat_c = NULL;
    int nro_submatrizes = 2;
    int ntasks = 2;
    int count_for = 10;

    param_t *args;
    pthread_t *threads;

    double tempo_Mult_Sequencial = 0;
    double tempo_Mult_SeqBloco = 0;
    double Mult_Thread = 0;
    double Mult_ThreadBloco = 0;
    double speedup_seqC;
    double speedup_BlC;

    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%
    if (argc != 4){
		printf ("ERRO: Numero de parametros %s <matriz_a> <matriz_b> <nthreads>\n", argv[0]);
		exit (1);
	}
    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //Leitura da Matriz A (arquivo)

    printf("\n ***************************************** \n");
	printf(" Matriz A");
	printf("\n ***************************************** \n");

    fmat = fopen(argv[1], "r");
    if (fmat == NULL)
    {
        printf("Error: Na abertura dos arquivos.");
        exit(1);
    }
    extrai_parametros_matriz(fmat, &N, &La, &vet_line, &nr_line);
    mat_a.matriz = NULL;
    mat_a.lin = N;
    mat_a.col = La;
    if (malocar(&mat_a))
    {
        printf("ERROR: Out of memory\n");
        exit(1);
    }
    filein_matriz(mat_a.matriz, N, La, fmat, vet_line, nr_line);
    free(vet_line);
    fclose(fmat);
    mimprimir(&mat_a);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //               Leitura da Matriz B (arquivo)

    printf("\n ***************************************** \n");
	printf(" Matriz B");
	printf("\n ***************************************** \n");

    fmat = fopen(argv[2], "r");
    if (fmat == NULL)
    {
        printf("Error: Na abertura dos arquivos.");
        exit(1);
    }
    extrai_parametros_matriz(fmat, &Lb, &M, &vet_line, &nr_line);
    mat_b.matriz = NULL;
    mat_b.lin = Lb;
    mat_b.col = M;
    if (malocar(&mat_b))
    {
        printf("ERROR: Out of memory\n");
        exit(1);
    }
    filein_matriz(mat_b.matriz, Lb, M, fmat, vet_line, nr_line);
    free(vet_line);
    fclose(fmat);
    mimprimir(&mat_b);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    // Multiplicação Sequencial
   
    mmult = (mymatriz **)malloc(sizeof(mymatriz *));
    for (int count = 0; count < count_for; count++)
    {
        start_time = wtime();
        mmult[0] = mmultiplicar(&mat_a, &mat_b, 1);
        end_time = wtime();
        tempo_Mult_Sequencial += end_time - start_time;
    }
    sprintf(filename, "Mult_Sequencial.result");
    fmat = fopen(filename, "w");
    fileout_matriz(mmult[0], fmat);
    fclose(fmat);

    printf("\n *** Multiplicação Sequencial: ");
    printf("Média = %f\n", tempo_Mult_Sequencial/count_for);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    // Multiplicação Sequencial em Bloco
    mmultbloco = (mymatriz **)malloc(sizeof(mymatriz *));
    for (int count = 0; count < count_for; count++)
    {
        Vsubmat_a = particionar_matriz(mat_a.matriz, N, La, 1, nro_submatrizes);
        Vsubmat_b = particionar_matriz(mat_b.matriz, Lb, M, 0, nro_submatrizes);
        Vsubmat_c = csubmatrizv2(N, M, nro_submatrizes);
        start_time = wtime();
        //multiplicacao de blocos
        for (int i = 0; i < nro_submatrizes; i++){
            mmsubmatriz(Vsubmat_a[i], Vsubmat_b[i], Vsubmat_c[i]);
        }
        mmultbloco[0] = msomar(Vsubmat_c[0]->matriz, Vsubmat_c[1]->matriz, 1);

        for (int i = 2; i < nro_submatrizes; i++){
            mmultbloco[0] = msomar(mmultbloco[0], Vsubmat_c[i]->matriz, 1);
        }
        end_time = wtime();
        tempo_Mult_SeqBloco += end_time - start_time;
    }
    sprintf(filename, "Mult_SeqBloco.result");
    fmat = fopen(filename, "w");
    fileout_matriz(mmultbloco[0], fmat);
    fclose(fmat);

    printf("\n *** Multiplicação Sequencial em Bloco: ");
    printf("Média = %f\n", tempo_Mult_Sequencial/count_for);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    // Multiplicação MultiThread
    mmultThread = (mymatriz **)malloc(sizeof(mymatriz *));
    mmultThread[0] = malloc(sizeof(mymatriz));
    mmultThread[0]->matriz = NULL;
    mmultThread[0]->lin = mat_a.lin;
    mmultThread[0]->col = mat_b.col;

    //realiza a alocação de memória para matriz resultado
    if (malocar(mmultThread[0]))
    {
        printf("ERROR: Out of memory\n");
        exit(1);
    }
    else
    {
        mzerar(mmultThread[0]);
    }

    for (int count = 0; count < count_for; count++)
    {
        mzerar(mmultThread[0]);
        threads = (pthread_t *)malloc(ntasks * sizeof(pthread_t));
        args = (param_t *)malloc(ntasks * sizeof(param_t));
        start_time = wtime();
        for (int i = 0; i < ntasks; i++) {
            args[i].tid = i;
            args[i].ntasks = ntasks;
            args[i].mat_a = &mat_a;
            args[i].mat_b = &mat_b;
            args[i].mat_c = mmultThread[0];
            pthread_create(&threads[i], NULL, exec_multi_thread, (void *)(args + i));
        }

        for (int i = 0; i < ntasks; i++) {
            pthread_join(threads[i], NULL);
        }
        end_time = wtime();
        Mult_Thread += end_time - start_time;
        //printf("thread %d. tempo: %.20f\n",count, end_time - start_time);
    }
    sprintf(filename, "Mult_Thread.result");
    fmat = fopen(filename, "w");
    fileout_matriz(mmultThread[0], fmat);
    fclose(fmat);

    printf("\n *** Multiplicação MultiThread: ");
    printf("Média = %f\n", Mult_Thread/count_for);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    // Multiplicação MultiThreads em Bloco
    mmultblocoThread = (mymatriz **)malloc(sizeof(mymatriz *));
    for (int count = 0; count < count_for; count++)
    {
        Vsubmat_a = particionar_matriz(mat_a.matriz, N, La, 1, nro_submatrizes);
        Vsubmat_b = particionar_matriz(mat_b.matriz, Lb, M, 0, nro_submatrizes);
        Vsubmat_c = csubmatrizv2(N, M, nro_submatrizes);
        threads = (pthread_t *)malloc(ntasks * sizeof(pthread_t));
        args = (param_t *)malloc(ntasks * sizeof(param_t));
        start_time = wtime();
        for (int i = 0; i < ntasks; i++) {
            args[i].tid = i;
            args[i].ntasks = ntasks;
            args[i].mat_bloco_a = Vsubmat_a[i];
            args[i].mat_bloco_b = Vsubmat_b[i];
            args[i].mat_bloco_c = Vsubmat_c[i];
            pthread_create(&threads[i], NULL, exec_multi_thread_blocos, (void *)(args + i));
        }

        for (int i = 0; i < ntasks; i++) {
            pthread_join(threads[i], NULL);
        }

        mmultblocoThread[0] = msomar(Vsubmat_c[0]->matriz, Vsubmat_c[1]->matriz, 1);

        end_time = wtime();
        Mult_ThreadBloco += end_time - start_time;
        
    }
    sprintf(filename, "Mult_ThreadBloco.result");
    fmat = fopen(filename, "w");
    fileout_matriz(mmultblocoThread[0], fmat);
    fclose(fmat);

    printf("\n *** Multiplicação MultiThreads em Bloco: ");
    printf("Média = %f\n", Mult_ThreadBloco/count_for);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    printf("\r                                              ");
    fflush(stdout);
    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //              Comparação dos resultados
	
    printf("\n ***************************************** \n");
	printf(" Comparar resultados e tempos");
	printf("\n ***************************************** \n");
	
	printf("\n COMPARAR MATRIZ_SeqC c/ MATRIZ_SeqBlC \n");
	mcomparar(mmult[0],mmultbloco[0]);
	
	printf("\n COMPARAR MATRIZ_SeqC c/ MATRIZ_ThreadC \n");
	mcomparar(mmult[0], mmultThread[0]);

	printf("\n COMPARAR MATRIZ_SeqC c/ MATRIZ_ThreadBlC\n");
	mcomparar(mmult[0],mmultblocoThread[0]);
	
	printf("TEMPO_MEDIO MATRIZ_SeqC.....: %f\n", tempo_Mult_Sequencial/count_for);
	printf("TEMPO_MEDIO MATRIZ_SeqBlC...: %f\n", tempo_Mult_SeqBloco/count_for);
	printf("TEMPO_MEDIO MATRIZ_ThreadC..: %f\n", Mult_Thread/count_for);
	printf("TEMPO_MEDIO MATRIZ_ThreadBlC: %f\n", Mult_ThreadBloco/count_for);

    speedup_seqC = (tempo_Mult_Sequencial / count_for) / (Mult_Thread / count_for);
    speedup_BlC = (tempo_Mult_SeqBloco / count_for) / (Mult_ThreadBloco / count_for);

	printf("\nSPEEDUP (MATRIZ_C)..........: %f\n", speedup_seqC);
	printf("SPEEDUP (MATRIZ_BLC)........: %f\n\n", speedup_BlC);
    
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //Liberação de memória
    mliberar(mmult[0]);
    mliberar(mmultbloco[0]);
    mliberar(mmultThread[0]);
    mliberar(mmultblocoThread[0]);

    free(mmult[0]);
    free(mmultbloco[0]);
    free(mmultThread[0]);
    free(mmultblocoThread[0]);

    mliberar(&mat_a);
    mliberar(&mat_b);

    free(mmult);
    free(mmultbloco);
    free(mmultThread);
    free(mmultblocoThread);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%
    return 0;
}