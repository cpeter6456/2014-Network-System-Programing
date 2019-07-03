# Makefile for progressive dictionary exercise

PROGS = msgq_server msgq_client
	

all : $(PROGS)

msgq_server:msgq_server.o lookup2.o
	$(CC) -o $@ -Q msgq_server.o lookup2.o
msgq_client:main.o lookup4.o
	$(CC) -o $@ -Q main.o lookup4.o
clean :
	rm -f *.o $(PROGS)
