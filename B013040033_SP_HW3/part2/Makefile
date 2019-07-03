all:mymv

CC = gcc
INCLUDE = 
CFLAGS = -g -Wall -ansi

mymv:mymv.o 
	$(CC) -o mymv mymv.o 

%.o:%.c
	$(CC) -c -Wall $<

dep:
	$(CC) -M *.c >dependence.txt

clean:
	rm -f *.o mymv dependence.txt
