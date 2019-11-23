# GNU Makefile
# Revisão: outubro/2019

CC = gcc
CCFLAGS = -Wall #-O3 #-g -llikwid #-funroll-all-loops
LDFLAGS = -fopenmp
TARGET = mainEx05 gmat help

all: $(TARGET)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< $(LDFLAGS)

%: %.o
	$(CC) $(LDFLAGS) $^ -o $@

mainEx05: mainEx05.o matrizv3.o toolsv3.o matriz-operacoesv3.o matriz-operacoes-omp.o
	$(CC) $(CCFLAGS) matriz-operacoes-omp.o matriz-operacoesv3.o matrizv3.o toolsv3.o mainEx05.c -o $@ $(LDFLAGS)

gmat: matrizv3.o toolsv3.o gera_matrizv3.c
	$(CC) $(CCFLAGS) matrizv3.o toolsv3.o gera_matrizv3.c -o $@ $(LDFLAGS)

help:
	@echo
	@echo
	@echo "####### Exemplo de Execução #######" 
	@echo "./mainEx05 a3x4.example b4x3.example 4"

clean:
	rm -f *.o *~ $(TARGET) *.map *.result
