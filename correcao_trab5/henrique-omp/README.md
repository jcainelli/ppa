###################################################
### Disciplina de Programação Paralela Avançada ###
###################################################
Revisão: ago/2019
Linguagem: C
Versão: v3 (tools, matriz, matriz-operacoes)
Executáveis: main, mainEx01 e gmat

main_omp: quinto exercício - multiplicação multithread utilizando OpenMP
gmat: gerador de matrizes NxM em arquivo

ARQUIVOS:
 * README-res: resultado da execução de exemplo (./main mat_a3x4.example mat_b4x3.example);
 * mat_a3x4.example e mat_b4x3.example: matrizes exemplos A (3x4) e B(4x3);
 * toolsv3.(c/h): biblioteca de leitura/escrita de matrizes em arquivos;
 * matriz-operacoesv3.h: headers das funções de operações de matrizes que devem ser implementadas;
 * matriz-operacoesv3.c: operações de multiplicação e adição implementadas;
 * matriz-operacoes-omp.h: headers das multiplicações com OpenMP
 * matriz-operacoes-omp.c: implementações das multiplicações com OpenMp
 * matrizv3.h: headers das funções de gerência de matrizes que devem ser implementadas;
 * matrizv3.c: operações básicas de manipulação de matrizes;
 * gera_matrizv3.c: fontes do programa de geração de matrizes em arquivo;
 * main_matrizv3.c: fontes do programa de teste das operações de matrizes;

OBSERVAÇÕES
 * padrão adotado para retorno das funções: 1 para sucesso, 0 para erro.
 * execuções com erro são abortadas com exit(1) (padrão do C).

OBSERVAÇÕES PARA O EXERCÍCIO 5 - OpenMP
 * É possível passar o número de threads desejadas para a multiplicação multithread,
 porém a multiplicação em bloco utilizará sempre 2 (até 3 threads) ou 4 blocos
 (a partir de 4 threads).

PARA COMPILAR PROJETO
 $ make

PARA EXECUTAR
 $ ./main_omp 1000x1000-mat.map 1000x1000-mat-a.map <NR_THREADS>
