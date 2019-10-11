#include "matriz-operacoesv3.h"

// *************************
// ************************* SOMAR
// *************************
mymatriz *msomar (mymatriz *mat_a, mymatriz *mat_b, int tipo) {
  if(mat_a == NULL || mat_b == NULL){
	  printf("Erro <msomar>: Matriz(es) invalida(s)\n");
	  return 0;
	}

  //validar se são do mesmo tamanho
  if (mat_a->lin != mat_b->lin || mat_a->col != mat_b->col) {
    printf("Erro <msomar>: Linhas e/ou colunas das matrizes são diferentes e não poderão ser somadas.\n");
    return 0;
  }
 
  int i,j;
  int l,c;
  int linha,coluna;
    
  mymatriz *mat_result; 
  mat_result = (mymatriz*) malloc(sizeof(mymatriz));
  mat_result->matriz = NULL;
	mat_result->lin = mat_a->lin;
	mat_result->col = mat_a->col;
	if (malocar(mat_result)) {
		printf("Erro <msomar>: Matriz invalida.\n");
    return 0;
	}
  mzerar(mat_result); 
	
  //Opções de parametros passados para 'tipo'
  //tipo = 0 --> a matriz será somada por linhas X colunas
  //tipo != 0 --> a matriz será somada por colunas X linhas
    if(tipo != 0){
        linha=mat_a->col;
        coluna=mat_a->lin;
    } else{
        linha=mat_a->lin;
        coluna=mat_a->col;
    }

	for(i=0; i<linha; i++){
        for(j=0; j<coluna; j++){
            if(tipo != 0){
                l=j;
                c=i;
            } else{
                l=i;
                c=j;
            }
			mat_result->matriz[l][c] = mat_a->matriz[l][c] + mat_b->matriz[l][c];
		}
	}
  return mat_result;
}

// *************************
// ************************* MULTIPLICAR
// *************************
mymatriz *mmultiplicar(mymatriz *mat_a, mymatriz *mat_b, int tipo) {

    if (mat_a == NULL || mat_b == NULL) {
       printf("Erro <mmultiplicar>: Matriz(es) invalida(s)\n");
       return 0;
    }

    if (mat_a->col != mat_b->lin) {
       printf("Erro (mmultiplicar): Coluna matriz A e linha da matriz B são diferentes e não poderão ser multiplicadas.\n");
       return 0;
    }

    if (tipo < 0 || tipo > 5) {
       printf("Erro (mmultiplicar): Tipo deve ter valor entre 0 e 5. Ele determinara a combinação o IJK. \n");
       return 0;
    }

    mymatriz *mat_result; 
    mat_result = (mymatriz*) malloc(sizeof(mymatriz));
    mat_result->matriz = NULL;
    mat_result->lin = mat_a->lin;
    mat_result->col = mat_b->col;

    if (malocar(mat_result)) {
		printf("Erro <mmultiplicar>: Matriz invalida.\n");
	}
	mzerar(mat_result);

    int i,j,k;
    int ii = 0, jj = 0, kk = 0;
    int lin_a = 0, col_b = 0, k_elem = 0;

    if (tipo == 0) {    //IJK
        lin_a = mat_a->lin; //I
        col_b = mat_b->col; //J
        k_elem = mat_a->col; //K: tanto faz K=mat_a->col ou K=mat_b->lin, pois para multiplicar tem que ser iguais
    }
    else if (tipo == 1) {   //IKJ
        lin_a = mat_a->lin; //I
        col_b = mat_a->col; //K
        k_elem = mat_b->col; //J 
    }
    else if (tipo == 2) {   //KIJ
        lin_a = mat_a->col; //K
        col_b = mat_a->lin; //I
        k_elem = mat_b->col; //J
    }
    else if (tipo == 3) {   //KJI
        lin_a = mat_a->col; //K
        col_b = mat_b->col; //J
        k_elem = mat_a->lin; //I
    }
    else if (tipo == 4) {   //JIK
        lin_a = mat_b->col; //J
        col_b = mat_a->lin; //I
        k_elem = mat_a->col; //K
    }
    else if (tipo == 5) {   //JKI
        lin_a = mat_b->col; //J
        col_b = mat_a->col; //K
        k_elem = mat_a->lin; //I
    }

    for(i = 0; i < lin_a; i++){
        for(j = 0; j < col_b; j++){
            for(k = 0; k < k_elem; k++){
                if (tipo == 0) {    //IJK
                    ii = i; 
                    jj = j; 
                    kk = k; 
                }
                else if (tipo == 1) {   //IKJ
                    ii = i; 
                    kk = j;
                    jj = k; 
                }
                else if (tipo == 2) {   //KIJ
                    kk = i;
                    ii = j; 
                    jj = k; 
                }
                else if (tipo == 3) {   //KJI
                    kk = i;
                    jj = j;
                    ii = k;  
                }
                else if (tipo == 4) {   //JIK
                    jj = i;
                    ii = j; 
                    kk = k;
                }
                else if (tipo == 5) {   //JKI
                    jj = i;
                    kk = j;
                    ii = k;                    
                }
                mat_result->matriz[ii][jj] += mat_a->matriz[ii][kk] * mat_b->matriz[kk][jj];
            }
        }
    } 
    return mat_result;
}

// *************************
// ************************* MULTIPLICAR SUBMATRIZ
// *************************
int mmsubmatriz (matriz_bloco_t *mat_suba, matriz_bloco_t *mat_subb, matriz_bloco_t *mat_subc) {
    
  if(!mat_suba || !mat_subb || !mat_subc){
		printf("Erro <mmsubmatriz>: Matriz(es) invalida(s)\n");
		return 1;
	}

  //printf("Erro <mmsubmatriz>: %d %d\n", mat_suba->matriz->col, mat_subb->matriz->lin);
  if (mat_suba->matriz->col != mat_subb->matriz->lin) {
    printf("Erro (mmsubmatriz): Coluna matriz A e linha da matriz B são diferentes e não poderão ser multiplicadas.\n");
    return 1;
  }

  int i,j,k;
  /*printf("Erro <mmsubmatriz>: %d %d %d %d - %d %d %d %d\n", 
  mat_suba->bloco->lin_inicio,
  mat_suba->bloco->lin_fim,
  mat_suba->bloco->col_inicio,
  mat_suba->bloco->col_fim,
  mat_subb->bloco->lin_inicio,
  mat_subb->bloco->lin_fim,
  mat_subb->bloco->col_inicio,
  mat_subb->bloco->col_fim);*/ 

  for(i = mat_suba->bloco->lin_inicio; i < mat_suba->bloco->lin_fim; i++){
  	for(k = mat_suba->bloco->col_inicio; k < mat_suba->bloco->col_fim; k++){
  		for(j = mat_subb->bloco->col_inicio; j < mat_subb->bloco->col_fim; j++){
        mat_subc->matriz->matriz[i][j] += mat_suba->matriz->matriz[i][k] * mat_subb->matriz->matriz[k][j];
  		}
  	}
  }
	return 0;
}

// *************************
// ************************* PARTICIONAR MATRIZ
// *************************
// Orientacao é "1" para horizontal e "0" para vertical

matriz_bloco_t **particionar_matriz (int **matriz, int mat_lin, int mat_col, int orientacao, int divisor) {
  
  matriz_bloco_t **block = NULL;
  block = (matriz_bloco_t **) calloc(divisor, sizeof(matriz_bloco_t*));

  mymatriz *matriz2 = (mymatriz*)malloc(sizeof(mymatriz));
  matriz2->matriz = matriz;
  matriz2->lin = mat_lin;
  matriz2->col = mat_col;

  //printf("Imprimi lin col Matriz: %d %d  \n", matriz2->lin, matriz2->col );
  
  if (!block || !matriz2) {
    printf("Erro <particionar_matriz>: Matriz invalida\n");
    return NULL;
  }

  if (orientacao != 0 && orientacao != 1) {
    printf("Erro <particionar_matriz>: A orientacao deve ser valores de: 1 para horizontal e 0 para vertical \n");
    return NULL;
  }

  for(int i = 0; i< divisor; i++){
    block[i] = (matriz_bloco_t *) malloc(sizeof(matriz_bloco_t));
    block[i]->bloco = (bloco_t *) malloc(sizeof(bloco_t));
  }

  if(orientacao == 0){ //vertical
    int desloc = mat_lin/divisor;
    for(int i=0; i<divisor; i++){
      block[i]->matriz = matriz2;
      block[i]->matriz->lin = matriz2->lin;
      block[i]->matriz->col = matriz2->col;
      block[i]->bloco->lin_inicio = i * desloc;
      block[i]->bloco->lin_fim = (i+1) * desloc;
      block[i]->bloco->col_inicio = 0;
      block[i]->bloco->col_fim = mat_col;
    }
    block[divisor-1]->bloco->lin_fim = mat_lin;

  } else { //horizontal
    int desloc = mat_col/divisor;
    for(int i=0; i<divisor; i++){
      block[i]->matriz = matriz2;
      block[i]->matriz->lin = matriz2->lin;
      block[i]->matriz->col = matriz2->col;
      block[i]->bloco->lin_inicio = 0;
      block[i]->bloco->lin_fim = mat_lin;
      block[i]->bloco->col_inicio = i * desloc;
      block[i]->bloco->col_fim = (i+1) * desloc;    
    }
    block[divisor-1]->bloco->col_fim = mat_col;

  }    
  return block;
}

// *************************
// ************************* ALOCAR SUBMATRIZES
// *************************
// Aloca um vetor de matrizes blocos com o número de matrizes igual ao divisor.

matriz_bloco_t **csubmatrizv2 (int mat_lin, int mat_col, int divisor) {

  matriz_bloco_t **block = NULL;
  block = (matriz_bloco_t **) calloc(divisor, sizeof(matriz_bloco_t*));
  if (!block) {
    printf("Erro <csubmatrizv2>: Matriz invalida.\n");
    return NULL;
  }

  for(int i = 0; i< divisor; i++){
    block[i] = (matriz_bloco_t *) malloc(sizeof(matriz_bloco_t));
    block[i]->bloco = (bloco_t *) malloc(sizeof(bloco_t));
  }

  for(int i=0; i<divisor; i++){
    
    // ini
    mymatriz *mat_result; 
    mat_result = (mymatriz*) malloc(sizeof(mymatriz));
    mat_result->matriz = NULL;
 	  mat_result->lin = mat_lin;
	  mat_result->col = mat_col;
	  if (malocar(mat_result)) {
		printf("Erro <csubmatrizv2>: Matriz invalida.\n");
        return NULL;
	  }
    mzerar(mat_result); 
    // fim

    block[i]->matriz = mat_result;
    block[i]->bloco->lin_inicio = 0;
    block[i]->bloco->lin_fim = mat_lin;
    block[i]->bloco->col_inicio = 0;
    block[i]->bloco->col_fim = mat_col;
  }
	return block;
}