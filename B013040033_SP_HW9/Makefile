all :chatroom chatroomd

chatroomd:server.o 
	$(CC) -o chatroomd server.o -lzmq
chatroom:client.o 
	$(CC) -o chatroom client.o  -lzmq -lpthread
%.o:%.c
	$(CC) -c -Wall -lzmq $<
clean :
	rm -f *.o chatroom chatroomd
