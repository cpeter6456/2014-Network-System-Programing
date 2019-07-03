# Makefile for progressive dictionary exercise

PROGS = fifo_server fifo_client
	

all : $(PROGS)

fifo_server:fifo_server.o lookup2.o
	$(CC) -o $@ fifo_server.o lookup2.o
fifo_client:main.o lookup3.o
	$(CC) -o $@ main.o lookup3.o
clean :
	rm -f *.o $(PROGS)
