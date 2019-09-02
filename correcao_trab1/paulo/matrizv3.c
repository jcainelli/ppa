#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toolsv3.h"
#include "matrizv3.h"


// gerência de matrizes


int malocar (mymatriz *matriz)
{
int *mat;
	mat->matriz = (int **) malloc(mat->lin*sizeof(int*));
	for(int i = 0; i < mat->lin; i++){
		mat->matriz[1] = (int *) malloc(mat->col*sizeof(int));
	}
	printf("AAAAA - 0");
	return 0;
}


int mliberar (mymatriz *matriz)
{
	printf("AAAAA - 1");
	return 0;
}


int mcomparar (mymatriz *mat_a, mymatriz *mat_b)
{
	printf("AAAAA-2");
	return 0;
}

