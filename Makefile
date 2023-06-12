make:
	gcc sudoku.c -o sudoku -lpthread
clean:
	rm -f sudoku sudoku_rac.out
	clear
