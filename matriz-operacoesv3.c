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
     mymatriz *ponteiro;

    ponteiro = (mymatriz*) malloc(sizeof(mymatriz));
    ponteiro->lin = 4;
    ponteiro->col = 4;
    ponteiro->matriz = NULL;

    if (malocar(ponteiro) == 0) {	//
        printf("msomar - Alocado memoria para nova matriz\n");
    //    ponteiro->matriz = mat_cal;
        ponteiro->matriz[0][0] = 1;
    }
    printf("msomar - Antes do retorno\n");
    return ponteiro;

}