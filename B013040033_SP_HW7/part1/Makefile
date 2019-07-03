# Makefile for progressive dictionary exercise

PROGS = \
	usock_client		\
	usock_server			

all : $(PROGS)

usock_client : lookup7.o main.o
	$(CC) -o $@ lookup7.o main.o

usock_server : lookup5.o usock_server.o
	$(CC) -o $@ lookup5.o usock_server.o

clean :
	rm -f *.o $(PROGS)
