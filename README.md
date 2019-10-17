# PPA - Programação Paralela Avançada
Programa em C para comparação de matrizes.

## ARQUIVOS:
* README-res: resultado da execução de exemplo (./matrizv3 mat_a3x4.example mat_b4x3.example);
* gera_matrizv3.c: fontes do programa de geração de matrizes em arquivo
* gmat: Biblioteca compilada para geração de matrizes
* Makefile: Arquivo para compilação dos fontes
* mat_a3x4.tst, mat_b3x4.tst: Arquivos de matrizes para teste
* matriz-operacoesv3.c: implementação das funções msomar, mmultiplicar
* matriz-operacoesv3.h: Struct das funções msomar, mmultiplicar
* matriz-operacoes-threads.h: Implementação das funções multiplicarTh e multiplicarThblocos
* matriz-operacoes-threads.h: Struct das funções multiplicarTh e multiplicarThblocos
* matrizv3.c: fontes do programa que implementa as operações: mcomparar, mliberar, mimprimir, mzerar, mgerar
* matrizv3.h: Struct das funções: mcomparar, mliberar, mimprimir, mzerar, mgerar
* toolsv3.(c/h): biblioteca de leitura/escrita de matrizes em arquivos;  

# Compilar todos
make clean; make

# Gerar Matrizes
./gmat 100 200
./gmat 200 100

# Valgrind
valgrind ./mainEx04 100x200-mat.map 200x100-mat.map

# Compilar a biblioteca Gmat (gera_matrizv3.c) para gerar uma matriz
make gmat

# Compilar Execicio 4 (Main):
make mainEx04

# Parâmetros
* Matriz (A)
* Matriz (B)
* Número de Threads

# Executar: 
 ./mainEx05 mat_a3x4.tst mat_b4x3.tst 4
