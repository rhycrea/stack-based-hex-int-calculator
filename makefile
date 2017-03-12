calculator : calculator.c int.h hex.h datastructures.h
	gcc -o calculator calculator.c -Wall -lm
clean :
	rm calculator

