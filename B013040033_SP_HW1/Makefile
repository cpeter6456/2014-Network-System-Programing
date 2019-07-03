all:myshell

CC = gcc
INCLUDE = 
CFLAGS = -g -Wall -ansi

myshell:parse.o builtin.o shell.o is_background.o run_command.o
	$(CC) -o myshell parse.o builtin.o shell.o is_background.o run_command.o

%.o:%.c
	$(CC) -c -Wall $<

dep:
	$(CC) -M *.c >dependence.txt

clean:
	rm -f *.o myshell dependence.txt
