/*
Objetivo: Disparar uma thread para cada linha necessária
*/

#include <mpi.h>
#include <stdio.h>
#include "matrizv3.h"

// função para mostrar o tabuleiro
void mostrarTabuleiro(mymatriz tab, int N)
{
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			if(tab.matriz[i][j] == 1)
                printf("R\t");
			else
                printf("-\t");
		}
        printf("\n\n");
	}
	printf("\n\n");
}

// verifica se é seguro colocar a rainha numa determinada coluna
// isso poderia ser feito com menos código, mas assim ficou mais didático
int seguro(mymatriz tab, int N, int lin, int col)
{
	int i, j;

	// verifica se ocorre ataque na linha
	for(i = 0; i < N; i++){
		if(tab.matriz[lin][i] == 1)
			return 0;
	}

	//verifica se ocorre ataque na coluna
	for(i = 0; i < N; i++){
		if(tab.matriz[i][col] == 1)
			return 0;
	}

	// verifica se ocorre ataque na diagonal principal - acima e abaixo
	for(i = lin, j = col; i >= 0 && j >= 0; i--, j--){
		if(tab.matriz[i][j] == 1)
			return 0;
	}
	for(i = lin, j = col; i < N && j < N; i++, j++){
		if(tab.matriz[i][j] == 1)
			return 0;
	}

	// verifica se ocorre ataque na diagonal secundária - acima e abaixo
	for(i = lin, j = col; i >= 0 && j < N; i--, j++){
		if(tab.matriz[i][j] == 1)
			return 0;
	}
	for(i = lin, j = col; i < N && j >= 0; i++, j--){
		if(tab.matriz[i][j] == 1)
			return 0;
	}
	
	return 1; // está seguro
}


void executar(mymatriz tab, int N, int col)
{
	if(col == N){
		//cout << "Solucao " << sol + 1 << ":\n\n";
		mostrarTabuleiro(tab, N);
		//sol++;
		return;
	}

	for(int i = 0; i < N; i++){
		// verifica se é seguro colocar a rainha naquela coluna
		if(seguro(tab, N, i, col) == 1)
		{
			// insere a rainha (marca com 1)
			tab.matriz[i][col] = 1;

			// chamada recursiva
			executar(tab, N, col + 1);

			// remove a rainha (backtracking)
			tab.matriz[i][col] = 0;
		}
	}
}

int main(int argc, char **argv) {
	int tam_tabuleiro = 8;
    int rank,size;
    mymatriz mat_tabuleiro;


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	//printf("Processo %d de %d\n",rank,size);

	if(rank == 0) {
		printf("(%d) -> Primeiro a escrever!\n",rank);		

        mat_tabuleiro.matriz = NULL;
        mat_tabuleiro.lin = tam_tabuleiro;
        mat_tabuleiro.col = tam_tabuleiro;
        if (malocar(&mat_tabuleiro)) {
            printf ("ERROR: Out of memory\n");
        }
        mzerar(&mat_tabuleiro);
        mimprimir(&mat_tabuleiro);

        MPI_Barrier(MPI_COMM_WORLD);
	    
        executar(mat_tabuleiro, tam_tabuleiro, 0);

        // Send Mensage

	}else{
		MPI_Barrier(MPI_COMM_WORLD);
		printf("(%d) -> Agora posso escrever!\n",rank);

        // Receiver Message

	}

	MPI_Finalize();
	return 0;
}
