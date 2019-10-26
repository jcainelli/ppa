# GNU Makefile
# Revisão: ago/2019

CC = gcc
CCFLAGS = -Wall -O3
LDFLAGS = -fopenmp
TARGET = main_omp gmat main help

all: $(TARGET)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< $(LDFLAGS)

%: %.o
	$(CC) $(LDFLAGS) $^ -o $@

main: main.c matrizv3.o toolsv3.o
				$(CC) $(CCFLAGS) matrizv3.o toolsv3.o main.c -o $@ $(LDFLAGS)

main_omp: main_omp.c matrizv3.o toolsv3.o matriz-operacoesv3.o matriz-operacoes-omp.o
		$(CC) $(CCFLAGS) matriz-operacoesv3.o matriz-operacoes-omp.o matrizv3.o toolsv3.o main_omp.c -o $@ $(LDFLAGS)

gmat: matrizv3.o toolsv3.o gera_matrizv3.c
		$(CC) $(CCFLAGS) matrizv3.o toolsv3.o gera_matrizv3.c -o $@ $(LDFLAGS)

help:
	@echo
	@echo
	@echo "####### Exemplo de Execução #######"
	@echo "./main_omp mat_a3x4.example mat_b4x3.example <NR_THREADS>"

clean:
	rm -f *.o *~ $(TARGET) *.map *.result
