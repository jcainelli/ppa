# PPA - Programação Paralela Avançada
Programa em C para comparação de matrizes - MPI.

## ARQUIVOS:
* README-res: resultado da execução de exemplo (./matrizv3 mat_a3x4.example mat_b4x3.example);
* gera_matrizv3.c: fontes do programa de geração de matrizes em arquivo
* gmat: Biblioteca compilada para geração de matrizes
* Makefile: Arquivo para compilação dos fontes
* mat_a3x4.tst, mat_b3x4.tst: Arquivos de matrizes para teste
* matriz-operacoesv3.c: implementação das funções msomar, mmultiplicar
* matriz-operacoesv3.h: Struct das funções msomar, mmultiplicar
* matriz-operacoes-mpi.h: Implementação das funções multiplicarMPI e multiplicarMPIblocos
* matriz-operacoes-mpi.h: Struct das funções multiplicarMPI e multiplicarMPIblocos
* matrizv3.c: fontes do programa que implementa as operações: mcomparar, mliberar, mimprimir, mzerar, mgerar
* matrizv3.h: Struct das funções: mcomparar, mliberar, mimprimir, mzerar, mgerar
* toolsv3.(c/h): biblioteca de leitura/escrita de matrizes em arquivos;  

# Compilar todos
make clean; make

# Gerar Matrizes
./gmat 100 200
./gmat 200 100

# Valgrind
valgrind ./mainEx06 100x200-mat.map 200x100-mat.map

# Compilar a biblioteca Gmat (gera_matrizv3.c) para gerar uma matriz
make gmat

# Compilar Execicio 6 (Main):
make mainEx06

# Compilar Execicio 6 (.c):
mpicc -Wall mainEx06.c

# Executar: 
mpirun --hostfile hosts mainEx06 100x100-mat.map 100x100-mat.map

 # Parâmetros
 1 - Arquivo de Hosts\
 2 - Programa principal\
 3 - Matriz (A)\
 4 - Matriz (B)\
