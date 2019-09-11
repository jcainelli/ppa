for(int i=0;i<(mat_result->lin);i++){
		for(int j=0;j<(mat_result->col);j++){
			for (int k=0;k<(mat_a->col);k++){
				if (k == 0){
					mat_result->matriz[i][j] = 0;
				}
				#if _DEBUG_
				printf("Tipo %i",tipo);
				printf("valor de K %i\n",k);
				printf("Multiplicando as matrizes i = %i j = %i\n", i, j);
				printf("valor inicial do campo: %i\n",mat_result->matriz[i][j]);
				printf("valor inicial do campo MA: %i\n",mat_a->matriz[i][j]);
				printf("valor inicial do campoMB: %i\n",mat_b->matriz[i][j]);
				printf("Posicao A %i, %i\nPosicao B %i, %i\n",i,k,k,j);
				printf("multiplicacao Valor %i * Valor %i\n",mat_a->matriz[i][k],mat_b->matriz[k][j]);
				#endif 

				mat_result->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
			}
		}
	}