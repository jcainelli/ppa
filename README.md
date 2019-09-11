# PPA - Programação Paralela Avançada
Programa em C para comparação de matrizes.

## ARQUIVOS:
* README-res: resultado da execução de exemplo (./matrizv3 mat_a3x4.example mat_b4x3.example);
* gera_matrizv3.c: fontes do programa de geração de matrizes em arquivo
* gmat: Biblioteca compilada para geração de matrizes
* mainEx01.c: fontes do programa principal que executa as operações: mcomparar, mliberar, mimprimir, mzerar, mgerar
* mainEx02.c: fontes do programa principal que executa as operações: msomar, mmultiplicar
* Makefile: Arquivo para compilação dos fontes
* mat_a3x4.tst, mat_b3x4.tst: Arquivos de matrizes para teste
* matriz-operacoesv3.c: implementação das funções msomar, mmultiplicar
* matriz-operacoesv3.h: Struct das funções msomar, mmultiplicar
* matrizv3.c: fontes do programa que implementa as operações: mcomparar, mliberar, mimprimir, mzerar, mgerar
* matrizv3.h: Struct das funções: mcomparar, mliberar, mimprimir, mzerar, mgerar
* toolsv3.(c/h): biblioteca de leitura/escrita de matrizes em arquivos;  

# Compilar a biblioteca Gmat (gera_matrizv3.c) para gerar uma matriz
make gmat

# Gerar uma matriz 10x10
./gmat 10 10

# Compilar Exercicio 1 (Main):
 make mainEx

# Compilar Execicio 2 (Main):
make mainEx02

# Executar: 
 ./mainEx02 mat_a3x4.tst mat_b4x3.tst 