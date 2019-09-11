#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toolsv3.h"
#include "matrizv3.h"
#include "matriz-operacoesv3.h"
#define _DEBUG_ 1

mymatriz *msomar (mymatriz *mat_a, mymatriz *mat_b, int tipo){
	#if _DEBUG_
	printf("Entrou na funcao msomar\n");
	#endif
	//criar matriz resultante
	//mymatriz smat_result;
	mymatriz* mat_result;// = &smat_result;
	//mat_result = (mymatriz*) malloc(sizeof(mymatriz));
	mat_result = (mymatriz*) calloc(mat_a->lin*mat_a->col,sizeof(mymatriz));
	//printf("--> Valor dos campos: %i", mat_result->matriz[0][0]);
	mat_result->matriz = NULL;
	mat_result->lin = mat_a->lin;
	mat_result->col = mat_a->col;
	if (malocar(mat_result) == 0){
		#if _DEBUG_
		printf("Matriz Result Alocada com sucesso\n");
		#endif
	}
	for(int i=0;i<(mat_result->lin);i++){
		for(int j=0;j<(mat_result->col);j++){
			#if _DEBUG_
			printf("Matriz a: \ni: %i \nj: %i\nTotal Linhas: %i\nTotal Colunas: %i\nTipo: %i\n", i,j,mat_result->lin,mat_result->col,tipo);
			#endif
			if (tipo == 0){
				#if _DEBUG_
				printf("-->Matriz 0\n");
				#endif
				mat_result->matriz[i][j] = mat_a->matriz[i][j] + mat_b->matriz[i][j];
			}else{
				#if _DEBUG_
				printf("-->Matriz 1\n");
				#endif
				mat_result->matriz[i][j] = mat_b->matriz[i][j] + mat_a->matriz[i][j];
			}
		}
	}
	#if _DEBUG_
	mimprimir(mat_a);
	mimprimir(mat_b);
	mimprimir(mat_result);
	#endif
	return mat_result;
}

mymatriz *mmultiplicar (mymatriz *mat_a, mymatriz *mat_b, int tipo){
	#if _DEBUG_
	printf("Entrou na funcao mmultiplicar\n");
	#endif
	if (mat_a->col != mat_b->lin){
		#if _DEBUG_
		printf("Tamanho matriz A:\nLinhas: %i\nColunas: %i\n",mat_a->lin,mat_a->col);
		printf("Tamanho matriz b:\nLinhas: %i\nColunas: %i\n",mat_b->lin,mat_b->col);
		printf("Matrizes não são compatíveis\n");
		#endif
		printf("===== A T E N Ç Ã O =====\nMatrizes não são compatíveis\n");
	}

//	int col,lin;
	//criar matriz resultante
	//mymatriz smat_result;
	mymatriz* mat_result;// = &smat_result;
	mat_result = (mymatriz*) calloc(mat_a->lin*mat_b->col,sizeof(mymatriz));
	//printf("--> Valor dos campos: %i", mat_result->matriz[0][0]);

	mat_result->matriz = NULL;
	mat_result->lin = mat_a->lin;
	mat_result->col = mat_b->col;
	//mzerar(mat_result);
	if (malocar(mat_result) == 0){
		#if _DEBUG_
		printf("Matriz Result Alocada com sucesso\n");
		#endif
			
	}
	//ijk
	if (tipo == 0){
		for(int i=0;i<(mat_result->lin);i++){
			for(int j=0;j<(mat_result->col);j++){
				for (int k=0;k<(mat_a->col);k++){
					if (k == 0){
						mat_result->matriz[i][j] = 0;
					}
				mat_result->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
				}
			}
		}
	}//ikj
	else if (tipo ==1){
		for(int i=0;i<(mat_result->lin);i++){
			for (int k=0;k<(mat_a->col);k++){
			for(int j=0;j<(mat_result->col);j++){
				
					if (k == 0){
						mat_result->matriz[i][j] = 0;
					}
				mat_result->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
				}
			}
		}
	}//jik
	else if (tipo ==2){
		for(int j=0;j<(mat_result->col);j++){
			for(int i=0;i<(mat_result->lin);i++){
				for (int k=0;k<(mat_a->col);k++){
					if (k == 0){
						mat_result->matriz[i][j] = 0;
					}
				mat_result->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
				}
			}
		}
	}//jki
	else if (tipo == 3){	
		for(int j=0;j<(mat_result->col);j++){
			for (int k=0;k<(mat_a->col);k++){
				for(int i=0;i<(mat_result->lin);i++){
					if (k == 0){
						mat_result->matriz[i][j] = 0;
					}
				mat_result->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
				}
			}
		}
	}//kij
	else if (tipo == 4){
		for (int k=0;k<(mat_a->col);k++){
			for(int i=0;i<(mat_result->lin);i++){
				for(int j=0;j<(mat_result->col);j++){
				
					if (k == 0){
						mat_result->matriz[i][j] = 0;
					}
				mat_result->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
				}
			}
		}
	}//kji
	else if (tipo == 5){	
		for (int k=0;k<(mat_a->col);k++){
			for(int j=0;j<(mat_result->col);j++){
				for(int i=0;i<(mat_result->lin);i++){
					if (k == 0){
						mat_result->matriz[i][j] = 0;
					}
				mat_result->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
				}
			}
		}
	}	
	//mimprimir(mat_a);
	//mimprimir(mat_b);
	//mimprimir(mat_result);
	return mat_result;
}