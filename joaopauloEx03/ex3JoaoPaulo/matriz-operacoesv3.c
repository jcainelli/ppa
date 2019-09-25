
#include "matriz-operacoesv3.h"
#include "matrizv3.h"

mymatriz *msomar(mymatriz *mat_a, mymatriz *mat_b, int tipo) {
   mymatriz *mat_soma;

    if (mat_a == NULL || mat_b == NULL) {
       printf("ERRO (MSOMAR) - Matriz A e/ou Matriz B invalida(s) - nula.\n");
       return NULL;
    }
   if (mat_a->lin != mat_b->lin || mat_a->col != mat_b->col) {
       printf("ERRO (MSOMAR) - Para somar a Matriz A e  a Matriz B precisam ter o mesmo numero de linhas e colunas.\n");
       return NULL;
    }
   mat_soma = (mymatriz*) malloc(sizeof(mymatriz));
   mat_soma->matriz = NULL;
   mat_soma->lin = mat_a->lin;
   mat_soma->col = mat_a->col;

   if (malocar(mat_soma)) {
		printf ("ERRO (MSOMAR) - Erro ao alocar matriz Soma\n");
        return NULL;
	}
	mzerar(mat_soma);

    if (tipo == 0) {    //IJ
        for (int i=0; i < mat_a->lin; i++) {
            for (int j=0; j < mat_a->col; j++){
                mat_soma->matriz[i][j] = mat_a->matriz[i][j] + mat_b->matriz[i][j];
            }
        }
    }
    else if (tipo == 1){    //JI
        for (int j=0; j < mat_a->col; j++){
            for (int i=0; i < mat_a->lin; i++) {
                mat_soma->matriz[i][j] = mat_a->matriz[i][j] + mat_b->matriz[i][j];
            }
        }
    }
    return mat_soma;
}

mymatriz *mmultiplicar(mymatriz *mat_a, mymatriz *mat_b, int tipo) {
    
    mymatriz *mat_mult;
  
    if (mat_a == NULL || mat_b == NULL) {
       printf("ERRO (MMULTIPLICAR) - Matriz A e/ou Matriz B invalida(s) - nula.\n");
       return NULL;
    }
     if (mat_a->col != mat_b->lin) {
       printf("ERRO (MMULTIPLICAR) - Para multiplicar o numero de colunas da Matriz A e o numero de linhas da Matriz B devem ser iguais.\n");
       return NULL;
    }

    mat_mult = (mymatriz*) malloc(sizeof(mymatriz));
    mat_mult->matriz = NULL;
    mat_mult->lin = mat_a->lin;
    mat_mult->col = mat_b->col; 

    if (malocar(mat_mult)) {
		printf ("ERRO (MMULTIPLICAR) - Erro ao alocar matriz Multiplica\n");
        return NULL;
	}
	mzerar(mat_mult);

    if (tipo == 0) {    //IJK
        for(int i = 0; i < mat_a->lin; i++){          
            for(int j = 0; j < mat_b->col; j++){
                for(int k = 0; k < mat_a->col; k++){
                    mat_mult->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
                }
            }
        }
    }
    else if (tipo == 1) {   //IKJ
        for(int i = 0; i < mat_a->lin; i++){
		    for(int k = 0; k < mat_a->col; k++){
			    for(int j = 0; j < mat_b->col; j++){
				    mat_mult->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
			    }
		    }
	    }
    }
    else if (tipo == 2) {   //KIJ
        for(int k = 0; k < mat_a->col; k++){
            for(int i = 0; i < mat_a->lin; i++){
                for(int j = 0; j < mat_b->col; j++){
                     mat_mult->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
                }
            }
        }
    }
    else if (tipo == 3) {   //KJI
        for(int k = 0; k < mat_a->col; k++){
            for(int j = 0; j < mat_b->col; j++){
                for(int i = 0; i < mat_a->lin; i++){
                    mat_mult->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
                }
            }
        }
    }
    else if (tipo == 4) {   //JIK
        for(int j = 0; j < mat_b->col; j++){
		    for(int i = 0; i < mat_a->lin; i++){
			    for(int k = 0; k < mat_a->col; k++){
				    mat_mult->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
                }
            }
        }
    }
    else if (tipo == 5) {   //JKI
        for(int j = 0; j < mat_b->col; j++){
            for(int k = 0; k < mat_a->col; k++){
                for(int i = 0; i < mat_a->lin; i++){
                    mat_mult->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
                }
            }
        }
    }
    return mat_mult;
}


int mmsubmatriz (matriz_bloco_t *mat_suba, matriz_bloco_t *mat_subb, matriz_bloco_t *mat_subc) {
    
    if(!mat_suba || !mat_subb || !mat_subc){
        printf("\nOut Of Memory (mmsubamtriz)!");
        return 1;
    }

    for (int i = mat_suba->bloco->lin_inicio; i < mat_suba->bloco->lin_fim; i++){
        for (int j = mat_suba->bloco->col_inicio; j < mat_suba->bloco->col_fim; j++){
            for (int k = mat_subb->bloco->col_inicio; k < mat_subb->bloco->col_fim; k++){
                mat_subc->matriz->matriz[i][k] += mat_suba->matriz->matriz[i][j] * mat_subb->matriz->matriz[j][k];
            }   
        }
    }
     return 0;
}

matriz_bloco_t **particionar_matriz (int **matriz, int mat_lin, int mat_col, int orientacao, int divisor) {
    matriz_bloco_t **bloco = NULL;
    bloco = (matriz_bloco_t **) calloc(divisor, sizeof(matriz_bloco_t *));

    if(bloco == NULL || matriz == NULL){
        printf("\nOut Of Memory (particionar_matriz)!");
        return NULL;
    }

    for(int i = 0; i < divisor; i++){
        bloco[i] = (matriz_bloco_t *)malloc(sizeof(matriz_bloco_t));
        bloco[i]->bloco = (bloco_t *)malloc(sizeof(bloco_t));
    }

    mymatriz *mat = (mymatriz *)malloc(sizeof(mymatriz));
    mat->matriz = matriz;

    if(orientacao == 0){
        int desloc = mat_lin / divisor;
        for(int i = 0; i < divisor; i++){
            bloco[i]->matriz = mat;
            bloco[i]->bloco->lin_inicio = i* desloc;
            bloco[i]->bloco->lin_fim = (i + 1) * desloc;
            bloco[i]->bloco->col_inicio = 0;
            bloco[i]->bloco->col_fim = mat_col;
        }
        bloco[divisor - 1]->bloco->lin_fim = mat_lin;
    }else{
        int desloc = mat_col / divisor;
        for(int i = 0; i < divisor; i++){
            bloco[i]->matriz = mat;
            bloco[i]->bloco->lin_inicio = 0;
            bloco[i]->bloco->lin_fim = mat_lin;
            bloco[i]->bloco->col_inicio = i * desloc;   
            bloco[i]->bloco->col_fim = (i + 1) * desloc;
        }
        bloco[divisor - 1]->bloco->col_fim = mat_col;
    }
    return bloco;
}

matriz_bloco_t **csubmatrizv2 (int mat_lin, int mat_col, int divisor) {
    matriz_bloco_t **bloco = NULL;
    bloco = (matriz_bloco_t **) calloc(divisor, sizeof(matriz_bloco_t *));
    if (!bloco){
        printf("\nOut Of Memory (csubmatrizv2)!");
        return NULL;
    }

    for(int i = 0; i < divisor; i++){
        bloco[i] = (matriz_bloco_t *)malloc(sizeof(matriz_bloco_t));
        bloco[i]->bloco = (bloco_t *)malloc(sizeof(bloco_t));
    }

    for (int i = 0; i < divisor; i++)   {
        mymatriz *matriz = (mymatriz *)malloc(sizeof(mymatriz));
        matriz->lin = mat_lin;
        matriz->col = mat_col;
        malocar(matriz);
        mzerar(matriz);
        bloco[i]->matriz = matriz;
        bloco[i]->bloco->lin_inicio = 0;
        bloco[i]->bloco->lin_fim = mat_lin;
        bloco[i]->bloco->col_inicio = 0;
        bloco[i]->bloco->col_fim = mat_col;
    }
    return bloco;      
}
