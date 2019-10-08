#include "matrizv3.h" 

int malocar(mymatriz *mat) {

    mat->matriz = NULL;

    mat->matriz = (int **) malloc(mat->lin*sizeof(int*));
    for(int i = 0; i < mat->lin; i++){
        mat->matriz[i] = (int *) malloc(mat->col*sizeof(int));
    }
    if (mat == NULL) {
        printf("ERRO (MALOCAR) - Sem memoria\n");
        return 1;
    }
    return 0;
}

int mgerar(mymatriz *matriz, int valor){

    if (valor != -9999 && valor != 0) {
        printf("ERRO (MGERAR) - Valor passado como entrada nao esta dentre os esperados para a funcao. Valores esperados: -9999 ou 0.\n");
        return 0;
    }

    if (matriz == NULL) {
       printf("ERRO (MGERAR) - Matriz invalida - nula.\n");
       return 0;
    }

    for(int i = 0; i < matriz->lin; i++){
        for(int j = 0; j < matriz->col; j++){
            if (valor == -9999) {
                matriz->matriz[i][j] = rand() % 10;
            }
            else {
                matriz->matriz[i][j] = 0;
            }
        }
    }
    return 1;
}

int mimprimir(mymatriz *matriz){

   if (matriz == NULL) {
       printf("ERRO (MGERAR) - Matriz invalida - nula.\n");
       return 0;
    }

	for (int j = 0; j < matriz->col; j++)
		printf("\t(%d)", j);
	printf("\n");
	for (int i = 0; i < matriz->lin; i++) {
		printf("(%d)", i);
        for (int j = 0; j < matriz->col; j++){
			printf("\t%d", matriz->matriz[i][j]);
		}
		printf("\n");
	}
	return 1;
}

int mzerar(mymatriz *matriz){
    return mgerar(matriz,0);
}

int mliberar(mymatriz *matriz) {

    if (matriz == NULL) {
       printf("ERRO (MLIBERAR) - Matriz invalida - nula.\n");
       return 0;
    }

    for(int i = 0; i < matriz->lin; i++){
        free(matriz->matriz[i]);
    }
    return 1;
}

int mcomparar(mymatriz *matriza, mymatriz *matrizb){

   if (matriza == NULL || matrizb == NULL) {
       printf("Matriz A e/ou Matriz B invalida(s) - nula.");
       return 0;
    }

   if (matriza->lin != matrizb->lin || matriza->col != matrizb->col) {
       printf("A Matriz A e Matriz B divergem no numero de linhas e colunas.");
       return 0;
    }

	for (int i=0; i < matriza->lin; i++) {
	  for (int j=0; j < matriza->col; j++){
			if (matriza->matriz[i][j] != matrizb->matriz[i][j]) {
				printf("# O elemento [%d,%d] eh diferente nas matrizes: %d != %d ", i,j,matriza->matriz[i][j], matrizb->matriz[i][j]);
				return 0;
			}
		}
	}
	printf("Matrizes identicas");

    return 1;
}
