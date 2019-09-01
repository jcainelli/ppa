# PPA - Programação Paralela Avançada
Programa em C para comparação de matrizes.

## ARQUIVOS:
* README-res: resultado da execução de exemplo (./matrizv3 mat_a3x4.example mat_b4x3.example);
mat_a3x4.example e mat_b4x3.example: matrizes exemplos A (3x4) e B(4x3);
* toolsv3.(c/h): biblioteca de leitura/escrita de matrizes em arquivos;  
* matrizv3.h: headers das funções de gerência de matrizes que devem ser implementadas;
* matriz-operacoesv3.h: headers das funções de operações de matrizes que devem ser implementadas;
* matriz-operacoesv3.c: Operaçes (deve ser implementado);
* matrizv3.c: Funçes de matriz;
* gera_matrizv3.c: fontes do programa de geração de matrizes em arquivo;
* main_matrizv3.c: fontes do programa de teste das operações de matrizes;

# Compilar:
 make mainEx01

# Executar: 
 ./mainEx01 mat_b3x4.tst mat_a3x4.tst

# Métodos implementados e não tratados na execução do programa mainEx01.c
Estes métodos estão implementados porém não são utilizados na implementação original do programa mainEx01.c
* mgerar
* mzerar
* mimprimir