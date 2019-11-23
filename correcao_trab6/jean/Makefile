# GNU Makefile
# Revisão: ago/2019

CC = mpicc
CCFLAGS = -Wall -O3
LDFLAGS =  
TARGET = mainEx06 gmat help

all: $(TARGET)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< $(LDFLAGS)

%: %.o
	$(CC) $(LDFLAGS) $^ -o $@

mainEx06: mainEx06.c matrizv3.o toolsv3.o matriz-operacoesv3.o matriz-operacoes-mpi.o
		$(CC) $(CCFLAGS) matriz-operacoes-mpi.o matriz-operacoesv3.o matrizv3.o toolsv3.o mainEx06.c -o $@ $(LDFLAGS)

gmat: matrizv3.o toolsv3.o gera_matrizv3.c
		$(CC) $(CCFLAGS) matrizv3.o toolsv3.o gera_matrizv3.c -o $@ $(LDFLAGS)

help:
	@echo
	@echo
	@echo "####### Exemplo de Execução #######" 
	@echo "./mainEx06 mat_a1000x1000.example mat_b1000x1000.example 4"

clean:
	rm -f *.o *~ $(TARGET) *.map *.result
