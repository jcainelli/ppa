# GNU Makefile
# Revisão: ago/2019

CC = gcc
CCFLAGS = -Wall -O3
LDFLAGS =
TARGET = mainEx02 gmat mainEx01 help mainEx03a

all: $(TARGET)

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

%: %.o
	$(CC) $(LDFLAGS) $^ -o $@



gmat: matrizv3.o toolsv3.o gera_matrizv3.c
		$(CC) $(CCFLAGS) matrizv3.o toolsv3.o gera_matrizv3.c -o $@ $(LDFLAGS)

main: mainEx05.c matrizv3.o toolsv3.o matriz-operacoesv3.o matriz-operacoes-threads.c 
		$(CC) $(CCFLAGS) matriz-operacoesv3.o matriz-operacoes-threads.c  matrizv3.o toolsv3.o mainEx05.c -o  $@ $(LDFLAGS) -fopenmp
help:
	@echo
	@echo
	@echo "####### Exemplo de Execução #######"
	@echo "./main mat_a3x4.example mat_b4x3.example"

clean:
	rm -f *.o *~ $(TARGET) *.map *.result
