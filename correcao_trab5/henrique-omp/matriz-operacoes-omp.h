#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include "matrizv3.h"
#include "matriz-operacoesv3.h"

mymatriz *multiplicarOMP(mymatriz *mat_a, mymatriz *mat_b, int ntasks);

mymatriz *multiplicarOMPblocos(mymatriz *mat_a, mymatriz *mat_b, int ntasks);
