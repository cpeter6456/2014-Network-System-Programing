all:hostinfo mycat mychmod mydate pipe_ls printdir

CC = gcc
INCLUDE = 
CFLAGS = -g -Wall -ansi

hostinfo:hostinfo.o
	$(CC) -o hostinfo hostinfo.o
mycat:mycat.o
	$(CC) -o mycat mycat.o
mychmod:mychmod.o
	$(CC) -o mychmod mychmod.o
mydate:mydate.o
	$(CC) -o mydate mydate.o
pipe_ls:pipe_ls.o
	$(CC) -o pipe_ls pipe_ls.o
printdir:printdir.o
	$(CC) -o printdir printdir.o

%.o:%.c
	$(CC) -c -Wall $<

dep:
	$(CC) -M *.c >dependence.txt

clean:
	rm -f *.o hostinfo mycat mychmod mydate pipe_ls printdir dependence.txt
