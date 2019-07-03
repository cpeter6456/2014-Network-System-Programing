# Makefile for progressive dictionary exercise

PROGS = \
	udp_client		\
	udp_server
	

all : $(PROGS)

udp_client : lookup9.o main.o
	$(CC) -o $@ lookup9.o main.o -lnsl

udp_server : lookup5.o udp_server.o
	$(CC) -o $@ lookup5.o udp_server.o -lnsl

clean :
	rm -f *.o $(PROGS)
