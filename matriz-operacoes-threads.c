#include "matriz-operacoesv3.h"
#include "matriz-operacoes-threads.h"

int calculaParticaoCorte (int valor, int porcao_particao){
    printf("##### calculaParticaoCorte\n");
    if (valor % 2){
        return valor / 2;
    }else{
        if (porcao_particao == 1){
            return valor / 2 + 1;
        }else{
            return valor / 2;
        }
    }
}

void *multiplicarTh (mymatriz *mat_a, mymatriz *mat_b, mymatriz *mat_c, int porcao_particao){
    printf("### DENTRO DA multiplicarTh\n\n");
    printf("### Mat (a) linha : %d \n", mat_a->lin);
    printf("### Quarto : %d \n", porcao_particao);

    int lin_inic_a, lin_fim_a, col_inic_a, col_fim_a, col_inic_b, col_fim_b = 0;

    if (porcao_particao == 0){        
        lin_inic_a = 0;
        lin_fim_a = calculaParticaoCorte(mat_a->lin, porcao_particao);
        col_inic_a = 0;
        col_fim_a = calculaParticaoCorte(mat_a->col, porcao_particao);
        
        col_inic_b = 0;
        col_fim_b = calculaParticaoCorte(mat_b->col, porcao_particao);
    }else{
        lin_inic_a = calculaParticaoCorte(mat_a->lin, porcao_particao);
        lin_fim_a = mat_a->lin;
        col_inic_a = calculaParticaoCorte(mat_a->col, porcao_particao);
        col_fim_a = mat_a->col;

        col_inic_b = calculaParticaoCorte(mat_b->col, porcao_particao);
        col_fim_b = mat_b->col;
    }

    printf("lin_inic_a : %d\n", lin_inic_a);
    printf("lin_fim_a  : %d\n", lin_fim_a);
    printf("col_inic_a : %d\n", col_inic_a);
    printf("col_fim_a  : %d\n", col_fim_a);
    printf("col_inic_b : %d\n", col_inic_b);
    printf("col_fim_b : %d\n", col_fim_b);
    printf("mat_c->lin : %d\n", mat_c->lin);
    printf("mat_c->cols : %d\n", mat_c->col);

    printf("multiplicarTh - 1");
    for(int i = lin_inic_a; i < lin_fim_a; i++){                  
        for(int j = col_inic_b; j < col_fim_b; j++){
            for(int k = col_inic_a; k < col_fim_a; k++){
                //printf("x: %d .y: %d .z:%d", i, j, k);
                printf(", (%d * %d)",mat_a->matriz[i][k], mat_b->matriz[k][j] );
                //mat_c->matriz[i][j] += mat_a->matriz[i][k] * mat_b->matriz[k][j];
                mat_c->matriz[i][j] = 0;
            }
        }
    }
    printf("multiplicarTh - 2");

}