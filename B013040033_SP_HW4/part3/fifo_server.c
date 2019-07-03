/*
 * fifo_server : listen on a named pipe; do lookup ; reply
 *               down another named pipe, the name of which
 *               will be sent by the client (in cli.id)
 *               argv[1] is the name of the local fil
 *               argv[2] is the name of the "well-known" FIFO
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "dict.h"
int main(int argc, char **argv) {
	
	int read_fd,write_fd;
	Client cli;
	Dictrec tryit;

	if (argc != 3) {
		fprintf(stderr,"Usage : %s <dictionary source> ""<resource / FIFO>\n",argv[0]);
		exit(errno);
	}

	

	/* Check for existence of dictionary and FIFO (both must exist)
	 *
	 * Fill in code. */
	if(access(argv[1],F_OK) < 0 )DIE("Not_Existence");
	if(access(argv[2],F_OK) < 0 )DIE("Not_Existence");

	/* Open FIFO for reading (blocks until a client connects)
	 *
	 * Fill in code. */
	read_fd = open(argv[2],O_RDONLY);

	/* Sit in a loop. lookup word sent and send reply down the given FIFO */
	for (;;) {

		/* Read request.
		 *
		 * Fill in code. */
		read(read_fd,&cli,sizeof(cli));

		/* Get name of reply fifo and attempt to open it.
		 *
		 * Fill in code. */
		write_fd = open(cli.id, O_WRONLY);
		strcpy(tryit.word,cli.word);
		strcpy(tryit.text,"XXXX");

		/* lookup the word , handling the different cases appropriately
		 *
		 * Fill in code. */

		switch(lookup(&tryit,argv[1]) ) {
			case FOUND:
				/* Fill in code. */
				write(write_fd,&tryit,sizeof(tryit));
				break;
			case NOTFOUND:
				
				/* Fill in code. */
				write(write_fd,&tryit,sizeof(tryit));
				break;
			case UNAVAIL:
				DIE("UNAVAIL");

				/* Fill in code. */
		}
		/* close connection to this client (server is stateless)
		 *
		 * Fill in code. */
		close(write_fd);
	}
}
