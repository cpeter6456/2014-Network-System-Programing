/*
 *	serve_client :
 *	The body of the thread of which one is started for each client.
 *	It receives as its argument a pointer to Servlet of which  precisely
 *	two fields : the start time and socket descriptor are non-null.
 *	The main duty of this thread is to conduct a dialog with the client.
 *	Correct termination is the string "---" in a line by itself.
 *	Incorrect termination and over verbose clients must the suitably handled.
 */

#include "sms.h"

void *serve_client(void *info) { /* body of servlet thread */
	Servlet *client = (Servlet *)info;
	char mymessage[SHORTMESS];
	client->chars_read=0;
	memset(mymessage, '\0',SHORTMESS );
	strcpy(mymessage,"Destination:\0");
	if( send(client->cust_ip,mymessage, SHORTMESS, 0) < 0) {
		DIE("send_error");
	}

	memset(client->dest, '\0',MAXDEST );
	if( recv(client->cust_ip,client->dest, MAXDEST, 0) < 0) {
		DIE("recv_error");
	}
	memset(mymessage, '\0',SHORTMESS );
	strcpy(mymessage,"Now Write your message:finish with ---\n-->\0");
	if( send(client->cust_ip,mymessage, SHORTMESS, 0) < 0) {
		DIE("send_error1");
	}
	
	memset(client->message, '\0',SHORTMESS );
	while (recv(client->cust_ip,client->message, SHORTMESS, 0) != -1){

		client->chars_read+=strlen(client->message);
/*
printf("get message [%s] len = %dfrom des=%s cust_ip = %d\n",client->message,strlen(client->message),client->dest,client->cust_ip);
*/
		if ( ! strncmp(client->message,"---",3) && strlen(client->message)==5) {
			break;
		}


		memset(mymessage, '\0',SHORTMESS );
		strcpy(mymessage,"-->\0");
		if( send(client->cust_ip, mymessage, SHORTMESS, 0) < 0) {
			DIE("send_error");
		}
		memset(client->message, '\0',SHORTMESS );
	}
	/*
	printf("des=%s cust_ip = %d END\n",client->dest,client->cust_ip);
	*/
	time( &(client->finish) );
	disconnect(client);
	return NULL;
}

/*

typedef struct str_servlet {
  int fd;						
  pthread_t thread;				
  int cust_ip;					/* IP addr of client					*
  char dest[MAXDEST];			/* addressee							*
  char message[SHORTMESS];		/* message text							*
  int chars_read;				/* size of mess so far					*
  time_t start;					/* Connection opened					*
  time_t finish;				/* Connection closed					*
  int aborted;					/* boolean flag							*
  struct str_servlet * prev;	/* Doubly linked list					*
  struct str_servlet * next;	/* Doubly linked list					*
} Servlet;
*/
