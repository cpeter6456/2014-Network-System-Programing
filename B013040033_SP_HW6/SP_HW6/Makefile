

all :shm_client shm_server

shm_server:shm_server.o lookup5.o
	$(CC) -o shm_server shm_server.o lookup5.o -lrt
shm_client:main.o lookup6.o
	$(CC) -o shm_client main.o lookup6.o 
%.o:%.c
	$(CC) -c -Wall $<
clean :
	rm -f *.o shm_client shm_server
