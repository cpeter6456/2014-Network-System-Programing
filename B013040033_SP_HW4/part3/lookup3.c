/*
 * lookup3 : This file does no looking up locally, but instead asks
 * a server for the answer. Communication is by named pipes.
 * The server sits listening on a "well-known" named pipe
 * (this is what is passed to lookup3 as resource)
 * The Client sets up a FIFO for reply, and bundles the FIFO
 * name with the word to be looked up. (See Client in dict.h)
 * Care must be taken to avoid deadly embrace (client & server
 * both waiting for something which can never happen)
 */

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "dict.h"

static Client me;

void cleanup(void) {
	/* Delete named pipe from system. 
	 *
	 * Fill in code. */
	unlink(me.id);



}

int lookup(Dictrec * sought, const char * resource) {
	static int write_fd;
	int read_fd;
	static int first_time = 1;

	if (first_time) {
		first_time = 0;

		/* Open existing named pipe for client->server communication.
		 *
		 * Fill in code. */
		write_fd= open(resource, O_RDWR);
		if(write_fd == -1){
			DIE(resource);
		}
		/* Create temporary named pipe for server->client communication. */
		umask(0);
		tmpnam(me.id);
		/**char *tmpnam(char *str) :: str -- 这就是指针的字符，tempname 将被保存为C字符串数组**/
		/* Fill in code. */

		read_fd = mkfifo(me.id,0666); 
		if(read_fd == -1){
			DIE(me.id);	
		}	

		/* Register cleanup handler.
		 *
		 * Fill in code. */
		/****/
		cleanup();
	}
	/* Send server the word to be found ; await reply */
	strcpy(me.word,sought->word);
	/* Fill in code. */
	cleanup();

	umask(0);
	read_fd = mkfifo(me.id,0666); 
	if(read_fd == -1){
		DIE(me.id);	
	}
	write(write_fd,&me, sizeof(me));


	/* Open the temporary named pipe to get the answer from it.
	 *
	 * Fill in code. */
	read_fd =  open(me.id, O_RDWR);
	strcpy(sought->text,"XXXX");
	/* Get the answer.
	 *
	 * Fill in code. */
	if( read(read_fd,sought,512) == 0)
		printf("Read null\n");
	/* Close the temporary named pipe as done.
	 *
	 * Fill in code. */
	close(read_fd);

	/* If word returned is not XXXX it was found. */
	if (strcmp(sought->text,"XXXX") != 0)
		return FOUND;

	return NOTFOUND;
}
