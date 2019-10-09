// Multiplicação por Thread
 void *multiplicarTh (mymatriz *mat_a, mymatriz *mat_b, mymatriz *mat_c, int porcao_particao);

// Multiplicação por Thread com blocos
 void *multiplicarThblocos (matriz_bloco_t *mat_suba, matriz_bloco_t *mat_subb, matriz_bloco_t *mat_subc);