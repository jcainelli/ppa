#include "matriz-operacoesv3.h"
#include "matriz-operacoes-threads.h"

int calculaParticaoCorte (int valor, int porcao_particao){
    return valor / 2;
}

void *multiplicarTh (mymatriz *mat_a, mymatriz *mat_b, mymatriz *mat_c, int porcao_particao){

    int lin_inic_a, lin_media_a, col_inic_a, col_fim_a, col_inic_b, col_fim_b = 0;

    lin_media_a = calculaParticaoCorte(mat_a->lin, porcao_particao);

    if (porcao_particao == 1){        
        for(int i = 0; i < lin_media_a; i++){          
            for(int j = 0; j < mat_b->col; j++){
                for(int k = 0; k < mat_a->col; k++){
                    mat_c->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
                }
            }
        }
    }else{
        for(int i = lin_media_a; i < mat_a->lin; i++){          
            for(int j = 0; j < mat_b->col; j++){
                for(int k = 0; k < mat_a->col; k++){
                    mat_c->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
                }
            }
        }
    }    
}