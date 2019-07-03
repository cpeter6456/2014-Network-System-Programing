all:file_lookup convert

CC = gcc
INCLUDE = 
CFLAGS = -g -Wall -ansi

file_lookup:main.o lookup1.o 
	$(CC) -o file_lookup main.o lookup1.o 

convert:convert.o
	$(CC) -o convert convert.o
%.o:%.c
	$(CC) -c -Wall $<

dep:
	$(CC) -M *.c >dependence.txt

clean:
	rm -f *.o file_lookup convert dependence.txt
