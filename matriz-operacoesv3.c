#include "matrizv3.h"

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

// recebe uma matriz de inteiros e a particiona em n (parâmetro divisor) pedaço de acordo com a orientação (horizontal ou vertical
matriz_bloco_t **particionar_matriz (int **matriz, int mat_lin, int mat_col, int orientacao, int divisor){
	printf("### Entrou no csubmatrizv2:\n");
	printf("##  mat_lin : %d \n", mat_lin);
	printf("##  mat_col : %d \n", mat_col);
	printf("##  orient  : %d \n", orientacao);
	printf("##  divisor : %d \n", divisor);

	matriz_bloco_t *result;
	mymatriz *mymatriz;
	matriz_bloco_t matriz_bloco_t_1;
	matriz_bloco_t matriz_bloco_t_2;

	mymatriz->matriz = &matriz; // ddd - Verificar
	mymatriz->col = mat_col;
	mymatriz->lin = mat_lin;

	result = (matriz_bloco_t*) malloc(sizeof(matriz_bloco_t)*divisor);	

	// Print da matriz a ser dividida
	/*for(int i = 0; i < mat_lin; i++){
		for(int j = 0; j < mat_col; j++){
			printf("[%d], ", matriz[i][j]);
		}
		printf("\n");
	}*/

	bloco_t *bloco_t_a;
	bloco_t *bloco_t_b;

	bloco_t_a = (bloco_t*) malloc(sizeof(bloco_t_a));	
	bloco_t_b = (bloco_t*) malloc(sizeof(bloco_t_b));	

	if (orientacao == 0){ // vertical
		int divisor_a = mat_col / divisor;		
		
		bloco_t_a->lin_inicio = 0;
		bloco_t_a->lin_fim=mat_lin;
		bloco_t_a->col_inicio = 0;
		bloco_t_a->col_fim = divisor_a;
		
		bloco_t_b->lin_inicio = 0;
		bloco_t_b->lin_fim = mat_lin;
		bloco_t_b->col_inicio = bloco_t_a->col_inicio + 1;
		bloco_t_b->col_fim = mat_col;
	}else{ // horizontal
		int divisor_a = mat_lin / divisor;		
		
		bloco_t_a->lin_inicio = 0;
		bloco_t_a->lin_fim= divisor_a;
		bloco_t_a->col_inicio = 0;
		bloco_t_a->col_fim = mat_col;

		bloco_t_b->lin_inicio = bloco_t_a->lin_inicio + 1;
		bloco_t_b->lin_fim= mat_lin;
		bloco_t_b->col_inicio = 0;
		bloco_t_b->col_fim = mat_col;
	}

	matriz_bloco_t_1.bloco = bloco_t_a;
	matriz_bloco_t_1.matriz = mymatriz; // ddd Aqui se não funcionar tentar com um &
	matriz_bloco_t_1.divisor = divisor;	
	matriz_bloco_t_1.mat_lin = mat_lin;
	matriz_bloco_t_1.mat_col = mat_col;


	matriz_bloco_t_2.bloco = bloco_t_b;
	matriz_bloco_t_2.matriz = mymatriz;
	matriz_bloco_t_2.divisor = divisor;	
	matriz_bloco_t_2.mat_lin = mat_lin;
	matriz_bloco_t_2.mat_col = mat_col;

	// Resultado
	result[0] = matriz_bloco_t_1;
	result[1] = matriz_bloco_t_2;

	return result;
}

 // aloca um vetor de matrizes blocos com o número de matrizes igual ao divisor.
matriz_bloco_t **csubmatrizv2 (int mat_lin, int mat_col, int nro_submatrizes){
	printf("### Entrou no csubmatrizv2:\n");
	printf("##  mat_lin (a)     : %d \n", mat_lin);
	printf("##  mat_col (b)     : %d \n", mat_col);
	printf("##  nro_submatrizes : %d \n", nro_submatrizes);

	matriz_bloco_t *Vsubmat_c;
	Vsubmat_c = (matriz_bloco_t*) malloc(sizeof(matriz_bloco_t)*nro_submatrizes);

	matriz_bloco_t Vsubmat_c_1;	
	bloco_t bloco_t_value_1;
	bloco_t *bloco_t_mat_c_1;
	matriz_bloco_t Vsubmat_c_2;
	bloco_t bloco_t_value_2;
	bloco_t *bloco_t_mat_c_2;	

	// Vetor 1
	bloco_t_value_1.lin_inicio = 0;
	bloco_t_value_1.col_inicio = 0;
	bloco_t_value_1.lin_fim = mat_lin;
	bloco_t_value_1.col_fim = mat_col;
	
	bloco_t_mat_c_1 = &bloco_t_value_1;

	Vsubmat_c_1.mat_col = mat_col;
	Vsubmat_c_1.mat_lin = mat_lin;
	Vsubmat_c_1.bloco = bloco_t_mat_c_1;
	Vsubmat_c_1.divisor = 1;

	// Vetor 2
	bloco_t_value_2.lin_inicio = 0;
	bloco_t_value_2.col_inicio = 0;
	bloco_t_value_2.lin_fim = mat_lin;
	bloco_t_value_2.col_fim = mat_col;
	
	bloco_t_mat_c_2 = &bloco_t_value_2;

	Vsubmat_c_2.mat_col = mat_col;
	Vsubmat_c_2.mat_lin = mat_lin;
	Vsubmat_c_2.bloco = bloco_t_mat_c_2;
	Vsubmat_c_2.divisor = 1;


	Vsubmat_c[0] = Vsubmat_c_1;
	Vsubmat_c[1] = Vsubmat_c_2;

	return Vsubmat_c;
}

 // multiplica a mat_suba pela mat_subb atribuindo o resultado a mat_subc;
int mmsubmatriz (matriz_bloco_t *mat_suba, matriz_bloco_t *mat_subb, matriz_bloco_t *mat_subc){
	printf("### Entrou no mmsubmatriz:\n");
	printf("### mat_suba->mat_lin : %d\n", mat_suba->mat_lin);
	printf("### mat_suba->mat_col : %d\n", mat_suba->mat_col);
	printf("### mat_suba->divisor : %d\n", mat_suba->divisor);
		
	printf("###   mat_suba->bloco->lin_inicio : %d \n", mat_suba->bloco->lin_inicio);
	printf("###   mat_suba->bloco->lin_fim : %d \n", mat_suba->bloco->lin_fim);
	printf("###   mat_suba->bloco->col_inicio : %d \n", mat_suba->bloco->col_inicio);
	printf("###   mat_suba->bloco->col_fim    : %d \n", mat_suba->bloco->col_fim);
	
	printf("---------------\n");

	printf("### mat_subb->mat_lin : %d\n", mat_subb->mat_lin);
	printf("### mat_subb->mat_col : %d\n", mat_subb->mat_col);
	printf("### mat_subb->divisor : %d\n", mat_subb->divisor);

	printf("###   mat_subb->bloco->lin_inicio : %d \n", mat_subb->bloco->lin_inicio);
	printf("###   mat_subb->bloco->lin_fim : %d \n", mat_subb->bloco->lin_fim);
	printf("###   mat_subb->bloco->col_inicio : %d \n", mat_subb->bloco->col_inicio);
	printf("###   mat_subb->bloco->col_fim    : %d \n", mat_subb->bloco->col_fim);


	mymatriz matriz_aux = **mat_subb->matriz; // ddd - Ponto de atenção

	for(int i = 0; i < mat_suba->mat_lin; i++){				
		for(int j = 0; j < mat_suba->mat_col; j++){
			printf("%d, ", matriz_aux->matriz[i][j]);
			/*for(int k = 0; k < mat_suba->mat_col; k++){
				mat_subc.matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
			}*/
		}
		printf("\n");
	}

}
