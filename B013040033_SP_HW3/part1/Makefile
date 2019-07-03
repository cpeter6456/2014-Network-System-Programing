all:myls

CC = gcc
INCLUDE = 
CFLAGS = -g -Wall -ansi

myls:myls.o 
	$(CC) -o myls myls.o 

%.o:%.c
	$(CC) -c -Wall $<

dep:
	$(CC) -M *.c >dependence.txt

clean:
	rm -f *.o myls dependence.txt
