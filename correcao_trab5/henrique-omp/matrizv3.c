#include "matrizv3.h"

// gerência de matrizes
int malocar (mymatriz *matriz) {

  if(matriz->lin <= 0) {
    printf("ERRO: número de linhas indefinido.\n");
    return 0;
  }

  if(matriz->col <= 0) {
    printf("ERRO: número de colunas indefinido.\n");
    return 0;
  }

  int **linhas = (int **) malloc(matriz->lin * sizeof(int *));
  matriz->matriz = linhas;

  for(int i = 0; i < matriz->lin; i++) {
    matriz->matriz[i] = (int *) malloc(matriz->col * sizeof(int));
  }

  return 1;
}

int mgerar(mymatriz *matriz, int valor) {

  if(matriz->matriz == NULL) {
      printf("ERRO: Matriz inexistente.\n");
      return 0;
  }

  for(int i = 0; i < matriz->lin; i++) {
    for(int j = 0; j < matriz->col; j++) {
      matriz->matriz[i][j] = valor;
    }
  }

  return 1;
}

int mgerar_aleatorio(mymatriz *matriz, int valor_maximo) {

  if(matriz->matriz == NULL) {
      printf("ERRO: Matriz inexistente.\n");
      return 0;
  }

  time_t t;
  srand((unsigned) time(&t));

  for(int i = 0; i < matriz->lin; i++) {
    for(int j = 0; j < matriz->col; j++) {
      matriz->matriz[i][j] = (rand() % valor_maximo) + 1;
    }
  }

  return 1;
}

int mimprimir (mymatriz *matriz) {

  printf("Imprimindo matriz %i x %i ...\n", matriz->lin, matriz->col);

  if(matriz->matriz == NULL) {
      printf("ERRO: Matriz inexistente.\n");
      return 0;
  }

  for(int i = 0; i < matriz->lin; i++) {
    printf("\n");
    for(int j = 0; j < matriz->col; j++) {
      printf("%i\t", matriz->matriz[i][j]);
    }
  }

  printf("\n\nFim impressão matriz.\n\n");

  return 1;
}

int mzerar (mymatriz *matriz) {

  if(matriz->matriz == NULL) {
      printf("ERRO: Matriz inexistente.\n");
      return 0;
  }

  for(int i = 0; i < matriz->lin; i++) {
    for(int j = 0; j < matriz->col; j++) {
      matriz->matriz[i][j] = 0;
    }
  }

  return 1;
}

int mliberar (mymatriz *matriz) {

  if(matriz->matriz == NULL) {
      printf("ERRO: Matriz inexistente.\n");
      return 0;
  }

  for(int i = 0; i < matriz->lin; i++) {
    free(matriz->matriz[i]);
  }

  free(matriz->matriz);

  return 1;
}

int mcomparar (mymatriz *mat_a, mymatriz *mat_b) {
  //printf("Comparando matrizes... ");

  if(mat_a == NULL || mat_a->matriz == NULL) {
    printf ("ERRO: Matriz A inexistente\n\n");
    return 0;
  }

  if(mat_b == NULL || mat_b->matriz == NULL) {
    printf ("ERRO: Matriz B inexistente\n\n");
    return 0;
  }

  if(mat_a->lin != mat_b->lin) {
    printf ("ERRO: Matrizes com número de linhas diferentes.\n");
    printf ("\t- matriz A %i linhas.\n", mat_a->lin);
    printf ("\t- matriz B %i linhas.\n\n", mat_b->lin);
    return 0;
  }

  if(mat_a->col != mat_b->col) {
    printf ("ERRO: Matrizes com número de colunas diferentes.\n");
    printf ("\t- matriz A %i colunas.\n", mat_a->col);
    printf ("\t- matriz B %i colunas.\n\n", mat_b->col);
    return 0;
  }

  for(int i = 0; i < mat_a->lin; i++) {
    for(int j = 0; j < mat_a->col; j++) {
      if(mat_a->matriz[i][j] != mat_b->matriz[i][j]) {
        printf("ERRO: Matriz diferente.\n");
        printf("\t- elemento: linha %i, coluna %i\n", i+1, j+1);
        printf("\t- valor na matriz A %i\n", mat_a->matriz[i][j]);
        printf("\t- valor na matriz B %i\n\n", mat_b->matriz[i][j]);
        return 0;
      }
    }
  }

  printf("RESULTADO: matrizes idênticas.\n\n");

  return 1;
}

// submatriz
int gerar_submatriz (int **mat_origem, matriz_bloco_t *submatriz, bloco_t *bloco) {
  printf ("ERRO: Método não implementado.\n");
  exit (1);
}

int imprimir_submatriz (matriz_bloco_t *submatriz) {
  printf ("ERRO: Método não implementado.\n");
  exit (1);
}

matriz_bloco_t **liberar_submatriz (matriz_bloco_t **submatriz) {
  printf ("ERRO: Método não implementado.\n");
  exit (1);
}

int imprimir_bloco (matriz_bloco_t *submatriz) {
  printf ("ERRO: Método não implementado.\n");
  exit (1);
}

matriz_bloco_t **constroi_submatriz (int **matriz, int mat_lin, int mat_col, int divisor) {
  printf ("ERRO: Método não implementado.\n");
  exit (1);
}

matriz_bloco_t **constroi_submatrizv2 (int mat_lin, int mat_col, int divisor) {
  printf ("ERRO: Método não implementado.\n");
  exit (1);
}

// aloca um vetor de matrizes blocos com o número de matrizes igual ao divisor.
matriz_bloco_t **csubmatrizv2 (int mat_lin, int mat_col, int divisor) {

  matriz_bloco_t **matriz_bloco_t = malloc(divisor * sizeof(*matriz_bloco_t));

  for(int i = 0; i < divisor; i++) {
    matriz_bloco_t[i] = malloc(sizeof(matriz_bloco_t));

    matriz_bloco_t[i]->matriz = (mymatriz *) malloc(sizeof(mymatriz));
    matriz_bloco_t[i]->matriz->lin = mat_lin;

    matriz_bloco_t[i]->matriz->col = mat_col;

    if (!malocar(matriz_bloco_t[i]->matriz)) {
      printf ("ERROR: Out of memory\n");
    }

    mzerar(matriz_bloco_t[i]->matriz);
  }

  return matriz_bloco_t;
}

// recebe uma matriz de inteiros e a particiona em n (parâmetro divisor) pedaço de acordo com a orientação (horizontal ou vertical)
// orientacao: 0 vertical, 1 horizontal
matriz_bloco_t **particionar_matriz (int **matriz, int mat_lin, int mat_col, int orientacao, int divisor) {

  matriz_bloco_t **matriz_bloco_t = malloc(divisor * sizeof(*matriz_bloco_t));

  for(int parte = 0; parte < divisor; parte++) {
    matriz_bloco_t[parte] = malloc(sizeof(matriz_bloco_t));

    matriz_bloco_t[parte]->matriz = (mymatriz *) malloc(sizeof(mymatriz));

    if(orientacao == 0) {
      matriz_bloco_t[parte]->matriz->lin = mat_lin / divisor;

      if(parte == (divisor - 1)) {
        int resto_lin = mat_lin % divisor;
        if(resto_lin > 0) {
          matriz_bloco_t[parte]->matriz->lin += resto_lin;
        }
      }

      matriz_bloco_t[parte]->matriz->col = mat_col;
    } else {
      matriz_bloco_t[parte]->matriz->col = mat_col / divisor;

      if(parte == (divisor - 1)) {
        int resto_col = mat_col % divisor;
        if(resto_col > 0) {
          matriz_bloco_t[parte]->matriz->col += resto_col;
        }
      }

      matriz_bloco_t[parte]->matriz->lin = mat_lin;
    }

    if (!malocar(matriz_bloco_t[parte]->matriz)) {
      printf ("ERROR: Out of memory\n");
      break;
    }

    mzerar(matriz_bloco_t[parte]->matriz);

  }

  int bloco_hor = 0, bloco_vert = 0, lin_repos = 0, col_repos = 0;

  for(int i = 0; i < mat_lin; i++) {

    if(orientacao == 0) {
      bloco_hor = (i + matriz_bloco_t[0]->matriz->lin) /
        (matriz_bloco_t[0]->matriz->lin);
      bloco_hor--;

      if(bloco_hor == divisor) {
        bloco_hor--;
      }

      lin_repos = i % matriz_bloco_t[0]->matriz->lin;
    }

    for(int j = 0; j < mat_col; j++) {

      if(orientacao == 1) {
        bloco_vert = (j + matriz_bloco_t[0]->matriz->col) /
          (matriz_bloco_t[0]->matriz->col);
        bloco_vert--;

        if(bloco_vert == divisor) {
          bloco_vert--;
        }

        col_repos = j % matriz_bloco_t[0]->matriz->col;

        matriz_bloco_t[bloco_vert]->matriz->matriz[i][col_repos] = matriz[i][j];
      } else {

        matriz_bloco_t[bloco_hor]->matriz->matriz[lin_repos][j] = matriz[i][j];
      }
    }

  }

  return matriz_bloco_t;
}
