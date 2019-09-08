#include "matrizv3.h"
#include "matriz-operacoesv3.h"

mymatriz *msomar (mymatriz *mat_a, mymatriz *mat_b, int tipo){
    int lin_a = (mat_a)->lin;
	int col_a = (mat_a)->col;
    int lin_b = (mat_b)->lin;
	int col_b = (mat_b)->col;

    mymatriz *ponteiro;
    ponteiro = (mymatriz*) malloc(sizeof(mymatriz));
    ponteiro->lin = lin_a;
    ponteiro->col = col_a;
    ponteiro->matriz = NULL;

    if ((lin_a != lin_b) || (col_a != col_b)){
        printf("# As Matrizes (A) e (B) não são compatíveis no número de Linhas x Colunas");        
    }else{
        if (malocar(ponteiro) == 0) {            
            if(tipo == 0){ // Horizontal
                for(int i = 0; i < lin_a; i++){
                    for (int j = 0; j < col_a; j++){
                        ponteiro->matriz[i][j] = mat_a->matriz[i][j] + mat_b->matriz[i][j];
                    }
                }
            } else{ // Vertical
                for(int i = 0; i < col_a; i++){
                    for (int j = 0; j < lin_a; j++){
                        ponteiro->matriz[j][i] = mat_a->matriz[j][i] + mat_b->matriz[j][i];
                    }
                }
            }            
        }
    }

    return ponteiro;
}

mymatriz *mmultiplicar (mymatriz *mat_a, mymatriz *mat_b, int tipo){
mymatriz *mat_mult;

    if (mat_a == NULL || mat_b == NULL) {
       printf("ERRO (MMULTIPLICAR) - Matriz A e/ou Matriz B invalida(s) - nula.\n");
       return 0;
    }

    printf(" %d  --  %d\n", mat_a->lin , mat_b->col);

     if (mat_a->lin != mat_b->col) {
       printf("ERRO (MMULTIPLICAR) - Para multiplicar o numero de linha da Matriz A e o numero de colunas da Matriz B devem ser iguais.\n");
       return 0;
    }

    mat_mult = (mymatriz*) malloc(sizeof(mymatriz));
    mat_mult->matriz = NULL;
    mat_mult->lin = mat_a->lin;
    mat_mult->col = mat_b->col;

    if (malocar(mat_mult)) {
		printf ("ERRO (MMULTIPLICAR) - Erro ao alocar matriz Soma\n");
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