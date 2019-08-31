#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toolsv3.h"
#include "matrizv3.h"


int malocar (mymatriz *pMatriz){

 pMatriz->matriz = malloc(pMatriz->lin*sizeof(int*));

  for(int i = 0; i < pMatriz->lin; i++){
    pMatriz->matriz[i] = malloc(pMatriz->col*sizeof(int));
	for (int j =0; j < pMatriz->col; j++){
		pMatriz->matriz[i][j] = 0;
	}
  }

  return 0;
}


int mcomparar (mymatriz *mat_a, mymatriz *mat_b){
	int lin_a = (mat_a)->lin;
	int col_a = (mat_a)->col;	
	int lin_b = (mat_b)->lin;
	int col_b = (mat_b)->col;
	
	if ((lin_a != lin_b) || (col_a != col_b)){
		printf("\n### Resultado: mat_a é DIFERENTE(!=) da mat_b\n");
		return 1;
	}

	for (int i=0; i < lin_a; i++){		
		for (int j=0; j < col_a; j++){
			int value_a = mat_a->matriz[i][j];
			int value_b = mat_b->matriz[i][j];
			if (value_a != value_b){
				printf("\n### Resultado: mat_a é DIFERENTE(!=)  da mat_b\n");
				return 1;
			}			
		}
	}
	
	printf("\n### Resultado: mat_a é IGUAL(==) da mat_b\n");

	return 0;
}


int mliberar (mymatriz *matriz){
 	free((matriz)->matriz); 
	return 0;
}

int mimprimir (mymatriz *matriz){
	int lin = (matriz)->lin;
	int col = (matriz)->col;		
	
	printf("# Listando Matriz:\n");
	for (int i=0; i < lin; i++){		
		for (int j=0; j < col; j++){
			int value = matriz->matriz[i][j];			
			printf("[%d],", value);
		}
		printf("\n");
	}
	return 0;
}

int mzerar (mymatriz *matriz){
	int lin = (matriz)->lin;
	int col = (matriz)->col;		
	
	printf("# Listando Matriz:\n");
	for (int i=0; i < lin; i++){		
		for (int j=0; j < col; j++){
			matriz->matriz[i][j] = 0;
		}
	}

	return 0;
}

int mgerar(mymatriz *matriz, int valor){	
	return 0;
}
