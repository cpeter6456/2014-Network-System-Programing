/*
 * msgq_server : serve clients using a message queue.
 *               rcv.content.id contains the type of the message:
 *                 type 1 messages are for the server
 *                 type n messages are for client whose pid is n
 *               argv[1] is the name of the local file
 *               argv[2] is the message-queue key
 *               To convert from string to long, use strtol, since
 *               it copes sensibly with numbers like 0xff
 */
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <sys/types.h>
#include <stdio.h>

#include "dict.h"

int main(int argc, char **argv) {
	int qid;
	Dictrec tryit;
	static ClientMessage rcv;
	ServerMessage snd;
	struct stat stbuff;
	int n;
	sscanf(argv[2], "%x", &n);    /** 字串s轉數字n**/
	
	if (argc != 3) {
		fprintf(stderr,"Usage : %s <dictionary source>"
			 " <resource / message queue key>\n",argv[0]);
		exit(errno);
	}

	/* Cannot find local database file. */
	if (stat(argv[1],&stbuff) == -1) {
		DIE(argv[1]);
	}
	/* Get the message queue, key is based on commandline second argument.
	 *
	 * Fill in code. */
	qid = msgget( n, IPC_CREAT | 0777);
	if (qid == -1) {
		 perror("server_msgget");
		 exit(1);
	}
	for (;;) { /* await client messages ; reply immediately */
		/* Wait for / receive a message.
		 *
		 * Fill in code. */
		if (msgrcv(qid, &rcv, (sizeof(rcv)-sizeof(long)), 1L, 0) == -1) {
			perror("server_msgrcv");
		}
		strcpy(tryit.word,rcv.content.word);/* Get the word to lookup. */
		snd.type = atol(rcv.content.id);	/* Get sender to set msg type.*/

		switch(lookup(&tryit,argv[1])) {	/* Lookup word in db. */
			case FOUND: 
				strcpy(snd.text,tryit.text);	/* Found.  Put result in return msg. */
				break;
			case NOTFOUND : 				/* Not found.  Return XXXX. */
				strcpy(snd.text,"XXXX");
				break;
			case UNAVAIL  : DIE(argv[1]);	/* Other problem. */
		}
		/* Send response.
		 *
		 * Fill in code. */
		if(msgsnd(qid,&snd,(sizeof(snd)-sizeof(long)),0) == -1){
			perror("server_msgsnd");
		}
	}
}
