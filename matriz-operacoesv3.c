#include "matrizv3.h"
#include "matriz-operacoesv3.h"

mymatriz *msomar (mymatriz *mat_a, mymatriz *mat_b, int tipo){
    
	printf("### @@ Entrou no somar \n");
	
	int lin_a = (mat_a)->lin;
	int col_a = (mat_a)->col;
    int lin_b = (mat_b)->lin;
	int col_b = (mat_b)->col;

	printf("### @@ Entrou no somar %d : \n", lin_a);

    mymatriz *ponteiro;
    ponteiro = (mymatriz*) malloc(sizeof(mymatriz));
    ponteiro->lin = lin_a;
    ponteiro->col = col_a;
    ponteiro->matriz = NULL;

    if ((lin_a != lin_b) || (col_a != col_b)){
        printf("# As Matrizes (A) e (B) não são compatíveis no número de Linhas x Colunas");        
    }else{

		printf("### @@ Entrou no somar 2\n");

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
	mymatriz *ponteiro;

	if (mat_a->col != mat_b->lin) {
		printf("ERRO (MMULTIPLICAR) - Para multiplicar o numero de linha da Matriz A e o numero de colunas da Matriz B devem ser iguais.\n");
		return 0;
	}

    ponteiro = (mymatriz*) malloc(sizeof(mymatriz));
    ponteiro->matriz = NULL;
    ponteiro->lin = mat_a->lin;
    ponteiro->col = mat_b->col;

    if (malocar(ponteiro)) {
		printf ("ERRO (MMULTIPLICAR) - Erro ao alocar matriz Soma\n");
	}else{
		mzerar(ponteiro);

		if (tipo == 0) {
			for(int i = 0; i < mat_a->lin; i++){
				for(int j = 0; j < mat_b->col; j++){
					for(int k = 0; k < mat_a->col; k++){
						ponteiro->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
					}
				}
			}
		}else if (tipo == 1) {
			for(int i = 0; i < mat_a->lin; i++){
				for(int k = 0; k < mat_a->col; k++){
					for(int j = 0; j < mat_b->col; j++){
						ponteiro->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
					}
				}
			}
		}else if (tipo == 2) {
			for(int k = 0; k < mat_a->col; k++){
				for(int i = 0; i < mat_a->lin; i++){
					for(int j = 0; j < mat_b->col; j++){
						 ponteiro->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
					}
				}
			}
		}else if (tipo == 3) {
			for(int k = 0; k < mat_a->col; k++){
				for(int j = 0; j < mat_b->col; j++){
					for(int i = 0; i < mat_a->lin; i++){
						ponteiro->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
					}
				}
			}
		}else if (tipo == 4) {
			for(int j = 0; j < mat_b->col; j++){
				for(int i = 0; i < mat_a->lin; i++){
					for(int k = 0; k < mat_a->col; k++){
						ponteiro->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
					}
				}
			}
		}else if (tipo == 5) {
			for(int j = 0; j < mat_b->col; j++){
				for(int k = 0; k < mat_a->col; k++){
					for(int i = 0; i < mat_a->lin; i++){
						ponteiro->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
					}
				}
			}
		}		
	}
	
	return ponteiro;
}


matriz_bloco_t **csubmatrizv2 (int mat_lin, int mat_col, int divisor){
	printf("### Entrou no csubmatrizv2\n\n");

}

int mmsubmatriz (matriz_bloco_t *mat_suba, matriz_bloco_t *mat_subb, matriz_bloco_t *mat_subc){
	printf("### Entrou no csubmatrizv2\n\n");

}

matriz_bloco_t **particionar_matriz (int **matriz, int mat_lin, int mat_col, int orientacao, int divisor){
	printf("### Entrou no csubmatrizv2\n\n");

}