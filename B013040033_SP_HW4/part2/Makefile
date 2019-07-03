all:myshell

CC = gcc
INCLUDE = 
CFLAGS = -g -Wall -ansi

myshell:parse.o builtin.o shell.o is_background.o run_command.o pipe_present.o pipe_command.o redirect_in.o redirect_out.o timer.o
	$(CC) -o myshell parse.o builtin.o shell.o is_background.o run_command.o pipe_present.o pipe_command.o redirect_in.o redirect_out.o timer.o

%.o:%.c
	$(CC) -c -Wall $<

dep:
	$(CC) -M *.c >dependence.txt

clean:
	rm -f *.o myshell dependence.txt
