# Makefile for progressive dictionary exercise

PROGS = \
	isock_client		\
	isock_server		
	

all : $(PROGS)

isock_client : lookup8.o main.o
	$(CC) -o $@ lookup8.o main.o -lnsl

isock_server : lookup5.o isock_server.o
	$(CC) -o $@ lookup5.o isock_server.o -lnsl

clean :
	rm -f *.o $(PROGS)
