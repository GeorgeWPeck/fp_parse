all:	fp_parse.c
	gcc -Wall -g -o fp_parse fp_parse.c -lm
