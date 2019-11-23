#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toolsv3.h"
#include "matrizv3.h"
#include "matriz-operacoesv3.h"
#include "matriz-operacoes-omp.h"

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
    int ntasks = 0;
    int count_for = 10;

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
	ntasks = atoi(argv[3]);
  	printf ("Argumento N Threads %s \n", argv[3]);

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //Leitura da Matriz A (arquivo)
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
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    //               Leitura da Matriz B (arquivo)
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
    sprintf(filename, "MultSequencial.result");
    fmat = fopen(filename, "w");
    fileout_matriz(mmult[0], fmat);
    fclose(fmat);
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
    sprintf(filename, "MultBloco.result");
    fmat = fopen(filename, "w");
    fileout_matriz(mmultbloco[0], fmat);
    fclose(fmat);
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
        start_time = wtime();
        mzerar(mmultThread[0]);
        multiplicarOMP(&mat_a,&mat_b,mmultThread[0],ntasks);
        end_time = wtime();
        Mult_Thread += end_time - start_time;
    }
    sprintf(filename, "OpenMPNormal.result");
    fmat = fopen(filename, "w");
    fileout_matriz(mmultThread[0], fmat);
    fclose(fmat);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    // Multiplicação MultiThreads em Bloco
    mmultblocoThread = (mymatriz **)malloc(sizeof(mymatriz *));
    for (int count = 0; count < count_for; count++)
    {
        start_time = wtime();
        Vsubmat_a = particionar_matriz(mat_a.matriz, N, La, 1, nro_submatrizes);
        Vsubmat_b = particionar_matriz(mat_b.matriz, Lb, M, 0, nro_submatrizes);
        Vsubmat_c = csubmatrizv2(N, M, nro_submatrizes);

        #pragma omp parallel for num_threads(ntasks)
            for (int i = 0; i < 2; i++){
                multiplicarOMPblocos (Vsubmat_a[i], Vsubmat_b[i], Vsubmat_c[i]);
            }
        mmultblocoThread[0] = msomar(Vsubmat_c[0]->matriz, Vsubmat_c[1]->matriz, 1);
        end_time = wtime();
        Mult_ThreadBloco += end_time - start_time;
    }
    sprintf(filename, "OpenMPBloco.result");
    fmat = fopen(filename, "w");
    fileout_matriz(mmultblocoThread[0], fmat);
    fclose(fmat);
    // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%
    printf("\r                                              ");
    fflush(stdout);
    // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
    // Impressao dos resultados de tempo
    printf("\n ##### Comparação dos resultados da Multiplicação de Matrizes #####\n");
    printf("[Sequencial vs Bloco]\n");
    mcomparar(mmult[0], mmultbloco[0]);
    printf("[Sequencial vs OpenMP]\n");
    mcomparar(mmult[0], mmultThread[0]);
    printf("[Sequencial vs BlocoOpenMP]\n");
    mcomparar(mmult[0], mmultblocoThread[0]);

    printf("[Tempo Medio 10 Multiplicações Sequencial ..........: [%f]]\n", tempo_Mult_Sequencial/count_for);
	printf("[Tempo Medio 10 Multiplicações Sequencial Bloco ....: [%f]]\n", tempo_Mult_SeqBloco/count_for);
	printf("[Tempo Medio 10 Multiplicações OpenMP ..............: [%f]]\n", Mult_Thread/count_for);
	printf("[Tempo Medio 10 Multiplicações OpenMP Bloco ........: [%f]]\n", Mult_ThreadBloco/count_for);

    speedup_seqC = (tempo_Mult_Sequencial / count_for) / (Mult_Thread / count_for);
    speedup_BlC = (tempo_Mult_SeqBloco / count_for) / (Mult_ThreadBloco / count_for);
	printf("\n[Speed-Up Normal OpenMP ...................................: [%f]]\n", speedup_seqC);
    printf("[Speed-Up Bloco OpenMP  ...................................: [%f]]\n", speedup_BlC);
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