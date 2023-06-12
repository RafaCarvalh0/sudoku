# sudoku

I. O PROBLEMA
O Sudoku é um quebra-cabeça que usa uma grade NxN formada por linhas e colunas, assim
como sub-grades AxB que dividem a original, de forma que A*B = N. Cada posição contêm
dígitos de 1 a N, de forma que cada coluna não pode ter números repetidos, assim como cada
linha e cada sub-grade. Tome como exemplo um Sudoku 9x9 em que cada sub-grade tenha
tamanho 3x3, ou seja, cada posição do quebra-cabeça contêm números de 1 a 9


Assim, essa atividade consiste em desenvolver uma aplicação multithread que determine se a
solução do Sudoku apresentada é válida. Considerando que a menor grade será 9x9, é
obrigatório que se use no mínimo 12 threads no código desenvolvido, e que as mesmas
estejam associadas ao processamento da validação, e não a etapas relacionadas a entrada e
saída. Soluções que não cumpram tais requisitos, serão completamente descartadas.
O arquivo de entrada tera a seguinte formatação:
[TAMANHO DA GRADE]
[TAMANHO DA SUB-GRADE]
[SUDOKU]
Por exemplo:
9x9
3x3
6 2 4 5 3 9 1 8 7
5 1 9 7 2 8 6 3 4
8 3 7 6 1 4 2 9 5
1 4 3 8 6 5 7 2 9
9 5 8 2 4 7 3 6 1
7 6 2 3 9 1 4 5 8
3 7 1 9 5 6 8 4 2
4 9 6 1 8 2 5 7 3
2 8 5 4 7 3 9 1 6


A compilação é feita por Makefile, através simplesmente do comando make via terminal e retornar o arquivo com nome "sudoku" executável,
e um arquivo de saída "sudoku_rac.out" contendo FAIL ou SUCCESS de acordo com o sudoku colocado dentro do arquivo, em caso de não achar o arquivo o terminal faz o print de "Error to read file", em casa de receber um valor invalido de parametros faz um print de "Invalid number of parameters" e em casa de ter algum erro dentro do arquivo é feito um print de "File out of format".

make -> ./sudoku nome_do_arquivo.txt
