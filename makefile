all : main 


main : test.c
	gcc -o test test.c
	