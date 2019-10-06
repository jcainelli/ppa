
#include "matriz-operacoesv3.h"

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
		printf("ERRO (MMSUBMATRIZ) - Matriz(es) invalida(s)\n");
		return 1;
	}

  int i,j,k;
  for(i = mat_suba->bloco->lin_inicio; i < mat_suba->bloco->lin_fim; i++){
  	for(k = mat_suba->bloco->col_inicio; k < mat_suba->bloco->col_fim; k++){
  		for(j = mat_subb->bloco->col_inicio; j < mat_subb->bloco->col_fim; j++){
  			mat_subc->matriz[i][j] += mat_suba->matriz[i][k] * mat_subb->matriz[k][j];
  		}
  	}
  }
	return 0;
}

matriz_bloco_t **particionar_matriz (int **matriz, int mat_lin, int mat_col, int orientacao, int divisor) {
  
  matriz_bloco_t **block = NULL;
  block = (matriz_bloco_t **) calloc(divisor, sizeof(matriz_bloco_t*));
  
  if (!block || !matriz) {
    printf("ERRO (PARTICIONAR_MATRIZ) - Matriz invalida\n");
    return NULL;
  }

  if (orientacao != 0 && orientacao != 1) {
    printf("ERRO (PARTICIONAR_MATRIZ) - A orientacao deve ser valores de: 1 para horizontal e 0 para vertical \n");
    return NULL;
  }

  for(int i = 0; i< divisor; i++){
    block[i] = (matriz_bloco_t *) malloc(sizeof(matriz_bloco_t));
    block[i]->bloco = (bloco_t *) malloc(sizeof(bloco_t));
  }

  if(orientacao == 1){
    int desloc = mat_lin/divisor;
    for(int i=0; i<divisor; i++){
      block[i]->matriz = matriz;
      block[i]->bloco->lin_inicio = i * desloc;
      block[i]->bloco->lin_fim = (i+1) * desloc;
      block[i]->bloco->col_inicio = 0;
      block[i]->bloco->col_fim = mat_col;

    }
    block[divisor-1]->bloco->lin_fim = mat_lin;
  } else {
    int desloc = mat_col/divisor;
    for(int i=0; i<divisor; i++){
      block[i]->matriz = matriz;
      block[i]->bloco->lin_inicio = 0;
      block[i]->bloco->lin_fim = mat_lin;
      block[i]->bloco->col_inicio = i * desloc;
      block[i]->bloco->col_fim = (i+1) * desloc;
    }
    block[divisor-1]->bloco->col_fim = mat_col;
  }
  return block;
}

matriz_bloco_t **csubmatrizv2 (int mat_lin, int mat_col, int divisor) {

  matriz_bloco_t **block = NULL;
  block = (matriz_bloco_t **) calloc(divisor, sizeof(matriz_bloco_t*));
  if (!block) {
    printf("ERRO (CSUBMATRIZV2: Matriz invalida.\n");
    return NULL;
  }

  for(int i = 0; i< divisor; i++){
    block[i] = (matriz_bloco_t *) malloc(sizeof(matriz_bloco_t));
    block[i]->bloco = (bloco_t *) malloc(sizeof(bloco_t));
  }

  for(int i=0; i<divisor; i++){

    mymatriz *mat_result; 
    mat_result = (mymatriz*) malloc(sizeof(mymatriz));
    mat_result->matriz = NULL;
	mat_result->lin = mat_lin;
	mat_result->col = mat_col;
	if (malocar(mat_result)) {
		printf("ERRO (CSUBMATRIZV2: Matriz invalida.\n");
        return NULL;
	}
    mzerar(mat_result); 

    int **matBl = mat_result->matriz;
    block[i]->matriz = matBl;
    block[i]->bloco->lin_inicio = 0;
    block[i]->bloco->lin_fim = mat_lin;
    block[i]->bloco->col_inicio = 0;
    block[i]->bloco->col_fim = mat_col;
  }
	return block;
}

