#include "matrizv3.h"

int malocar(mymatriz *mat) {
    int i;
    mat->matriz = NULL;

    //mat->matriz = (int **) calloc(mat->lin, sizeof(int*));
    mat->matriz = (int **) malloc(mat->lin * sizeof(int*));
    for(i = 0; i < mat->lin; i++){
        //mat->matriz[i] = (int *) calloc(mat->col, sizeof(int));
        mat->matriz[i] = (int *) malloc(mat->col * sizeof(int));
    }

    if (mat == NULL) {
        printf("LEON:ERRO (MALOCAR) - Sem memoria\n");
        //O retorno esta invertido para não ter que mexer no programa chamador, pois o mesmo não
        // trata a negação da chamada.
        return 1;
    }
    return 0;
}

int mgerar(mymatriz *matriz, int valor){
    if (matriz == NULL) {
		printf("LEON:ERRO (MGERAR) - Matriz passada como parâmetro é nula, verifique!!");
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
    if (matriz == NULL) {
		printf("LEON:ERRO (MIMPRIMIR) - Matriz passada como parâmetro é nula, verifique!!");
        return 0;
    }
    int i, j;

	for (j = 0; j < matriz->col; j++)
		printf("\t(%d)", j);
	    printf("\n");
	    for (i = 0; i < matriz->lin; i++) {
		    printf("(%d)", i);
	        for (j = 0; j < matriz->col; j++){
			    printf("\t%d", matriz->matriz[i][j]);
		    }
		    printf("\n");
	}
	return 1;
}

int mzerar(mymatriz *matriz){
    return mgerar(matriz, 0);
}

int mliberar(mymatriz *matriz) {
   int i;

    for(i = 0; i < matriz->lin; i++){
        free(matriz->matriz[i]);
    }

    //free(matriz);

    return 1;
}

int mcomparar(mymatriz *matriza, mymatriz *matrizb){
    int i, j;

    if (!matriza || !matrizb) {
        printf("LEON:ERRO (MCOMPARAR): Matrizes nao inicializadas\n");
        return 0;
    }
    if  ((matriza->lin != matrizb->lin) && (matriza->col != matrizb->col)){
        printf("LEON:ERRO (MCOMPARAR): MatrizA com definição linha/coluna diferente da MatrizB.\n");
        return 0;
    }

	for (i=0; i < matriza->lin; i++) {
	  for (j=0; j < matriza->col; j++){
			if (matriza->matriz[i][j] != matrizb->matriz[i][j]) {
				printf("LEON:ERRO (MCOMPARAR): O elemento [%d,%d] eh diferente nas matrizes: %d != %d\n", i,j,matriza->matriz[i][j], matrizb->matriz[i][j]);
				return 0;
			}
		}
	}
	printf("(MCOMPARAR): MatrizA é idênticas MatrizB\n\n");
    return 1;
}

