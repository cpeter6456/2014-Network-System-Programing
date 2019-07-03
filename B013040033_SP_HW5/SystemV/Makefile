all:producer consumer


CC = gcc
producer: producer.o
	$(CC) -o producer producer.o -lrt
consumer:consumer.o
	$(CC) -o consumer consumer.o -lrt



%.o:%.c
	$(CC) -c -Wall $<

clean :
	rm -f *.o producer consumer dependence.txt
