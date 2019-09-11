#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toolsv3.h"
#include "matrizv3.h"

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main(int argc, char *argv[]) {

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	// DECLARAÇÃO de VARIÁVEIS
	mymatriz mat_a, mat_b;
	FILE *fmat;
	int nr_line;
	int *vet_line = NULL;
	int N, M, La, Lb;
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	if (argc != 3){
		printf ("ERROR: Numero de parametros %s <matriz_a> <matriz_b>\n", argv[0]);
		exit (1);
	}

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//                Leitura da Matriz A (arquivo)
	printf("Importando MatrizA...\n");
	fmat = fopen(argv[1],"r");
	if (fmat == NULL) {
		printf("ERROR: Na abertura dos arquivos.");
		exit(1);
	}
	extrai_parametros_matriz(fmat, &N, &La, &vet_line, &nr_line);
	//return 1;
	mat_a.matriz = NULL;
	mat_a.lin = N;
	mat_a.col = La;
	if (malocar(&mat_a)) {
		printf ("Ponto-1:ERROR: Out of memory\n");
	}
	filein_matriz (mat_a.matriz, N, La, fmat, vet_line, nr_line);
	free (vet_line);
	fclose(fmat);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//               Leitura da Matriz B (arquivo)
	printf("Importando MatrizB...\n");
	fmat = fopen(argv[2],"r");
	if (fmat == NULL) {
		printf("ERROR: Na abertura dos arquivos.");
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

	/* teste para zerar mzerar
	printf ("Antes de Zerar\n");
	mzerar(&mat_b);
	printf ("Fim de Zerar\n");
	mimprimir(&mat_a);
	mimprimir(&mat_b);
	printf ("Fim de Imprimir\n");
	*/

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//              Comparação dos resultados
	printf("\n ##### Comparação dos resultados das matrizes #####\n");
	printf("[mat_a vs mat_b]\t");
	mcomparar (&mat_a, &mat_b);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	// %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
	//                   Libera��o de mem�ria
	mliberar(&mat_a);
	mliberar(&mat_b);
	// %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

	return 0;
}

