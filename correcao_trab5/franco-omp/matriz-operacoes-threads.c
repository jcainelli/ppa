#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "toolsv3.h"
#include "matrizv3.h"
#include "matriz-operacoesv3.h"
#include "matriz-operacoes-threads.h"

void *mmultiplicarth(void *arg){
    
    mymatriz *mat_mult;
    param_t *p = (param_t *) arg;

    mat_mult = (mymatriz*) malloc(sizeof( mymatriz));
    
    mat_mult->lin = p->mata->lin;
    mat_mult->col = p->matb->col; 

	 if (p->mata->matriz == NULL || p->matb->matriz == NULL) {
       printf("ERRO (MMULTIPLICAR) - Matriz A e/ou Matriz B invalida(s) - nula.\n");
       return NULL;
    }
   

    if (!malocar(mat_mult)) {
		printf ("ERRO (MMULTIPLICAR) - Erro ao alocar matriz Multiplica\n");
        return NULL;
	}
   
  //int limita = p->limite/p->ntasks
  int inicio = p->tid*p->limite/p->ntasks;
  int fim =  inicio+p->limite/p->ntasks;
  
  

  for(int i = inicio; i < fim; i++){          
    for(int j = 0; j <  mat_mult->col; j++){
      for(int k = 0; k < mat_mult->col; k++){
          p->matresult->matriz[i][j] += p->mata->matriz[i][k] * p->matb->matriz[k][j];
       }
   }
  }

    return 0;
    
}

void *mmsubmatrizth(void *args) {

  matriz_bloco_t *mat_suba; 
  matriz_bloco_t *mat_subb; 
  //matriz_bloco_t *mat_subc; 

  param_t_bloco *pth = (param_t_bloco *) args;

  mat_suba = pth->matsuba;
  mat_subb = pth->matsubb;
 // mat_subc = pth->matresult;
  //mat_subc->matriz = p->matresult->matriz = NULL;
  mzerar(pth->matresult->matriz);

    if(pth->tid == 1){
        //printf ("[%d] %p\n", pth->tid, mat_suba);
  
        for (int i = mat_suba->bloco->lin_inicio; i < mat_suba->bloco->lin_fim; i++) {
          for (int k = mat_suba->bloco->col_inicio; k < mat_suba->bloco->col_fim; k++){
            for (int j = mat_subb->bloco->col_inicio; j < mat_subb->bloco->col_fim; j++) {
               //mat_subc->matriz->matriz[i][j] 
               pth->matresult->matriz->matriz[j][i] += mat_suba->matriz->matriz[j][k] * mat_subb->matriz->matriz[k][i];
               //mat_subc->matriz->matriz[i][j] += mat_suba->matriz->matriz[i][k] * mat_subb->matriz->matriz[k][j];
       
               }
          }

        }      
     // printf("id %d \n", pth->tid );
    }else if(pth->tid == 0){
   // printf("id %d \n", pth->tid );
      //  printf ("[%d] = %p\n", pth->tid, mat_suba);
        for (int i = mat_suba->bloco->lin_inicio; i < mat_suba->bloco->lin_fim; i++) {
          for (int k = mat_suba->bloco->col_inicio; k < mat_suba->bloco->col_fim; k++){
            for (int j = mat_subb->bloco->col_inicio; j < mat_subb->bloco->col_fim; j++) {
              pth->matresult->matriz->matriz[i][j] += mat_suba->matriz->matriz[i][k] * mat_subb->matriz->matriz[k][j];
              
             // printf("valor A[%d][%d] = %d \n", i,j,mat_suba->matriz->matriz[i][k]);
            }
          }
        }

    }


 // pth->matresult = mat_subc;  
  return 0;
}
