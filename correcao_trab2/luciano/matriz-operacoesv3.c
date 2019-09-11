#include "matriz-operacoesv3.h"

mymatriz *msomar (mymatriz *mat_a, mymatriz *mat_b, int tipo) {
    if(mat_a == NULL || mat_b == NULL){
		printf("LEON:ERRO (MSOMAR): Matriz(es) invalida(s)\n");
		return 0;
	}

    //validar se são do mesmo tamanho
    if (mat_a->lin != mat_b->lin || mat_a->col != mat_b->col) {
       printf("LEON:ERRO (MSOMAR): Linhas e/ou colunas das matrizes são diferentes e não poderão ser somadas.\n");
       return 0;
    }
 
    int i,j;
    int l,c;
    int linha,coluna;
    mymatriz *mat_result; //resultado da soma
  
    //Prepara a matriz resultado da soma 
    mat_result = (mymatriz*) malloc(sizeof(mymatriz));
    mat_result->matriz = NULL;
	mat_result->lin = mat_a->lin;
	mat_result->col = mat_a->col;
 
	if (malocar(mat_result)) {
		printf("LEON:ERRO (MSOMAR): Matriz invalida.\n");
	}
    mzerar(mat_result); 
	
    //Opções de parametros passados para 'tipo'
    //tipo = 0 --> a matriz será somada por linhas X colunas
    //tipo <> 0 --> a matriz será somada por colunas X linhas

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

mymatriz *mmultiplicar(mymatriz *mat_a, mymatriz *mat_b, int tipo) {

    mymatriz *mat_mult;

    if (mat_a == NULL || mat_b == NULL) {
       printf("LEON:ERRO (MMULTIPLICAR) - Matriz A e/ou Matriz B invalida(s) - nula.\n");
       return 0;
    }

    if (mat_a->lin != mat_b->col) {
       printf("LEON:ERRO (MMULTIPLICAR) - Para multiplicar o numero de linha da Matriz A e o numero de colunas da Matriz B devem ser iguais.\n");
       return 0;
    }

    mat_mult = (mymatriz*) malloc(sizeof(mymatriz));
    mat_mult->matriz = NULL;
    mat_mult->lin = mat_a->lin;
    mat_mult->col = mat_b->col;

    if (malocar(mat_mult)) {
		printf ("LEON:ERRO (MMULTIPLICAR) - Erro ao alocar matriz Soma\n");
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


