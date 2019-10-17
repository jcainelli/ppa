# GNU Makefile
# Revisão: ago/2019

CC = gcc
CCFLAGS = -Wall -O3
LDFLAGS = -lpthread 
TARGET = mainEx04 gmat help

all: $(TARGET)

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

%: %.o
	$(CC) $(LDFLAGS) $^ -o $@

mainEx04: mainEx04.c matrizv3.o toolsv3.o matriz-operacoesv3.o matriz-operacoes-threads.o
		$(CC) $(CCFLAGS) matriz-operacoes-threads.o matriz-operacoesv3.o matrizv3.o toolsv3.o mainEx04.c -o $@ $(LDFLAGS)

gmat: matrizv3.o toolsv3.o gera_matrizv3.c
		$(CC) $(CCFLAGS) matrizv3.o toolsv3.o gera_matrizv3.c -o $@ $(LDFLAGS)

help:
	@echo
	@echo
	@echo "####### Exemplo de Execução #######" 
	@echo "./mainEx03 mat_a3x4.example mat_b4x3.example 5"

clean:
	rm -f *.o *~ $(TARGET) *.map *.result
