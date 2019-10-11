#include "matrizv3.h"

int malocar(mymatriz *mat) {
    int i;
    mat->matriz = NULL;

    mat->matriz = (int **) malloc(mat->lin * sizeof(int*));
    for(i = 0; i < mat->lin; i++){
        mat->matriz[i] = (int *) malloc(mat->col * sizeof(int));
    }
    
    if (mat == NULL){
        printf("Erro <malocar>: Matriz invaida. Nao foi possivel alocar a matriz.\n");
        return 1;
    }
    return 0; //Matriz alocada. Porém o retorno invertido por conta do programa que chama.
}

int mgerar(mymatriz *matriz, int valor){
    
    if (matriz == NULL){
        printf("Erro <mgerar>: Matriz invalida.\n");
        return 0;
    }

    int i, j;

    for(i = 0; i < matriz->lin; i++){
        for(j = 0; j < matriz->col; j++){
            if (valor == -9999) {
                matriz->matriz[i][j] = rand() % 100;
            }
            else {
                matriz->matriz[i][j] = 0;
            }
        }
    }
    return 1;
}

int mimprimir(mymatriz *matriz){

    if (matriz == NULL){
        printf("Erro <mimprimir>: Matriz invalida.\n");
        return 0;
    }

    int i, j;

    printf("Impressao <mimprimir>: Matriz NxM(%d,%d)\n", matriz->lin, matriz->col);

	for (j = 0; j < matriz->col; j++)
		printf("\t[C%d]", j);
	printf("\n");
	for (i = 0; i < matriz->lin; i++) {
		printf("[L%d]", i);
	  for (j = 0; j < matriz->col; j++){
			printf("\t%d", matriz->matriz[i][j]);
		}
		printf("\n");
	}
	return 1;
}

int mzerar(mymatriz *matriz){
    mgerar(matriz,0);
    return 1;
}

int mliberar(mymatriz *matriz) {

    if (matriz == NULL){
        printf("Erro <mliberar>: Matriz invalida.\n");
        return 0;
    }

    int i;

    for(i = 0; i < matriz->lin; i++){
        free(matriz->matriz[i]);
    }

    return 1;
}

int mcomparar(mymatriz *matriza, mymatriz *matrizb){
    
    if (matriza == NULL || matrizb == NULL) {
        printf("Erro <mcomparar>: Matriz(es) invalida(s)\n");
        return 0;
    }

    if (matriza->lin != matrizb->lin || matriza->col != matrizb->col) {
       printf("Erro <mcomparar>: Linhas e/ou colunas das matrizes são diferentes e não poderão ser comparadas.\n");
       return 0;
    }

    int i, j;

	for (i=0; i < matriza->lin; i++) {
	  for (j=0; j < matriza->col; j++){
			if (matriza->matriz[i][j] != matrizb->matriz[i][j]) {
				printf("Aviso <mcomparar>: Existe diferenca [%d,%d] --> %d != %d\n", i,j,matriza->matriz[i][j], matrizb->matriz[i][j]);
				return 0;
			}
		}
	}
	printf("Aviso <mcomparar>: Nao existe nenhuma diferenca entre as matrizes\n\n");
    return 1;
}
