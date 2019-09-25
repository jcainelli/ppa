###################################################
### Disciplina de Programação Paralela Avançada ###
###################################################
Revisão: ago/2019
Linguagem: C
Versão: v3 (tools, matriz, matriz-operacoes)
Executáveis: mainEx01, mainEx02 , mainEx03 e gmat

mainEx01.exe: programa principal de teste das funcoes da biblioteca matrizv3.c
mainEx02.exe: programa principal de teste das funcoes da biblioteca matriz-operacoesv3.c
mainEx03.exe: programa principal de teste das funcoes da biblioteca matriz-operacoesv3.c
gmat.exe: gerador de matrizes NxM em arquivo

ARQUIVOS:
 * README-res: resultados da execucao
                gmat     ==> ./gmat 3 5 e ./gmat 5 3
                mainEx01 ==> ./mainEx01 3x5-mat.map 5x3-mat.map e ./mainEx01 3x5-mat.map 3x5-mat.map
	        mainEx02 ==> ./mainEx02 3x5-mat.map 5x3-mat.map
                mainEx03 ==> ./mainEx03 mat_a3x4.example mat_b4x3.example
* 3x5-mat.map e 5x3-mat.map - matrizes exemplo A (3x5) e B(5x3)
 * toolsv3.(c/h): biblioteca de leitura/escrita de matrizes em arquivos
 * matrizv3.h: headers das funcoes de gerencia de matrizes
 * matrizv3.c: biblioteca de manipulacao de matrizes contendo as funcoes: malocar, mgerar, mimprimir, mzerar, mliberar e mcomparar
 * matriz-operacoesv3.h: headers das funções de operações de matrizes: msomar e mmultiplicar
 * matriz-operacoesv3.c: biblioteca de manipulacao de matrizes contendo as funcoes: msomar e mmultiplicar
 * gera_matrizv3.c: fonte do programa de geração de matrizes em arquivo
 * mainEx01.c: fonte do programa de teste das operacoes de matrizes
 * mainEx02.c: fonte do programa de teste das operacoes de matrizes (soma e multiplicacao)
