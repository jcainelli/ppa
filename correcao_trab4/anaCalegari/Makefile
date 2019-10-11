# GNU Makefile
# Revisão: ago/2019

CC = gcc
CCFLAGS = -Wall -O3
LDFLAGS = -lpthread
TARGET = mainEx04 mainEx03a mainEx02 gmat mainEx01 help

all: $(TARGET)

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

%: %.o
	$(CC) $(LDFLAGS) $^ -o $@

mainEx01: mainEx01.c matrizv3.o toolsv3.o
			$(CC) $(CCFLAGS) matrizv3.o toolsv3.o mainEx01.c -o $@ $(LDFLAGS)

mainEx02: mainEx02.c matrizv3.o toolsv3.o matriz-operacoesv3.o
		$(CC) $(CCFLAGS) matriz-operacoesv3.o matrizv3.o toolsv3.o mainEx02.c -o $@ $(LDFLAGS)

mainEx03a: mainEx03a.c matrizv3.o toolsv3.o matriz-operacoesv3.o
		$(CC) $(CCFLAGS) matriz-operacoesv3.o matrizv3.o toolsv3.o mainEx03a.c -o $@ $(LDFLAGS)

mainEx04: mainEx04.c matrizv3.o toolsv3.o matriz-operacoesv3.o matriz-operacoes-threads.o
		$(CC) $(CCFLAGS) matriz-operacoes-threads.o matriz-operacoesv3.o matrizv3.o toolsv3.o mainEx04.c -o $@ $(LDFLAGS)

#mainEx04: mainEx04.c matrizv3.o toolsv3.o matriz-operacoesv3.o matriz-operacoes-threads.o
#		$(CC) $(CCFLAGS)  matriz-operacoes-threads.o matriz-operacoesv3.o matrizv3.o toolsv3.o mainEx04.c -o $@ $(LDFLAGS)

gmat: matrizv3.o toolsv3.o gera_matrizv3.c
		$(CC) $(CCFLAGS) matrizv3.o toolsv3.o gera_matrizv3.c -o $@ $(LDFLAGS)

help:
	@echo
	@echo
	@echo "####### Exemplo de Execução #######"
	@echo "./mainEx04 mat_a3x4.example mat_b4x3.example 2"

clean:
	rm -f *.o *~ $(TARGET) *.map *.result
