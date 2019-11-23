# GNU Makefile

CC = mpicc
CCFLAGS = -Wall -O3 #-llikwid #-funroll-all-loops
LDFLAGS =
TARGET = mainEx06 gmat help
EXE = ./gmat

all: $(TARGET)

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

%: %.o
	$(CC) $(LDFLAGS) $^ -o $@

mainEx06: mainEx06.c matrizv3.o toolsv3.o matriz-operacoesv3.o matriz-operacoes-MPI.o
	$(CC) $(CCFLAGS) matriz-operacoes-MPI.o matriz-operacoesv3.o matrizv3.o toolsv3.o mainEx06.c -o $@ $(LDFLAGS)

gmat: matrizv3.o toolsv3.o gera_matrizv3.c
	$(CC) $(CCFLAGS) matrizv3.o toolsv3.o gera_matrizv3.c -o $@ $(LDFLAGS)


help:
	@echo
	@echo
	@echo "####### Exemplo de Geração de Matriz #######"
	@echo "./gmat 100 100"
	@echo
	@echo
	@echo "####### Exemplo de Execução #######"
	@echo "mpirun -np 8 ./mainEx06 100x100-mat.map 100x100-mat.map"
	@echo
	@echo

clean:
	rm -f *.o *~ $(TARGET) *.map *.result
