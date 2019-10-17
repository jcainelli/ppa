# GNU Makefile
# Revisão: ago/2019

CC = gcc
CCFLAGS = -Wall -O3
LDFLAGS = -lpthread 
TARGET = mainEx05 gmat help

all: $(TARGET)

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

%: %.o
	$(CC) $(LDFLAGS) $^ -o $@

mainEx05: mainEx05.c matrizv3.o toolsv3.o matriz-operacoesv3.o matriz-operacoes-threads.o
		$(CC) $(CCFLAGS) matriz-operacoes-threads.o matriz-operacoesv3.o matrizv3.o toolsv3.o mainEx05.c -o $@ $(LDFLAGS)

gmat: matrizv3.o toolsv3.o gera_matrizv3.c
		$(CC) $(CCFLAGS) matrizv3.o toolsv3.o gera_matrizv3.c -o $@ $(LDFLAGS)

help:
	@echo
	@echo
	@echo "####### Exemplo de Execução #######" 
	@echo "./mainEx05 mat_a3x4.example mat_b4x3.example 5"

clean:
	rm -f *.o *~ $(TARGET) *.map *.result
