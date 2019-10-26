#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "matrizv3.h"

typedef struct {
      int tid;
      int ntasks;
      int limite;
      mymatriz *mata;
      mymatriz *matb;
      mymatriz *matresulta;
      mymatriz *matresultb;
      mymatriz *matresult;
      
} param_t;

typedef struct {
      int tid;
      int ntasks;
      int limite;
      matriz_bloco_t *matsuba;
      matriz_bloco_t *matsubb;
      matriz_bloco_t *matresult;
      
} param_t_bloco;


typedef struct {
  int tid;
  int ntasks;
  int limite;
  matriz_bloco_t *blocoa;
  matriz_bloco_t *blocob;
  matriz_bloco_t *blococ;
} matriz_bloco_t_th;


void *mmultiplicarth(void * arg);


void *mmsubmatrizth(void * arg);