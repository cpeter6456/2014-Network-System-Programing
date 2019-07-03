/*
 *	listen_port :
 *	The body of the thread which listens on a particular
 *	port number (it's sole argument).
 *	Whenever a new connection is requested it calls
 *	make_servelet_list and then fires off a new thread running.
 *	It must set two fields in the Servlet structure :
 *		1) fd
 *		2) start
 */

#include "sms.h"

void make_servelet_list();
int sd,cd;
void *listen_port(void *info) { /* body of port listener */
/**
close-----关闭本进程的socket id，但链接还是开着的，用这个socket id的其它进程还能用这个链接，能读或写这个socket id
shutdown--则破坏了socket 链接
注意:
    1>. 如果有多个进程共享一个套接字，close每被调用一次，计数减1，直到计数为0时，也就是所用进程都调用了close，套接字将被释放。
    2>. 在多进程中如果一个进程中shutdown(sfd, SHUT_RDWR)后其它的进程将无法进行通信.
**/
     pthread_attr_t attr;
	 pthread_attr_init(&attr);
	 pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);				/**set detach **/
	 struct sockaddr_in server;
	 sd=0;cd=0;
	 socklen_t server_len;		
 /**
int socket(int domain,int type,int protocol);
SOCK_STREAM 提供双向连续且可信赖的数据流，即TCP。支持OOB 机制，在所有数据传送前必须使用connect()来建立连线状态。
 **/

	if ( (sd= socket (AF_INET,SOCK_STREAM,0 ))<0 ) {
            DIE("socket_error");
    }
	int reuse_addr = 1;
	socklen_t reuse_addr_len = sizeof(reuse_addr);
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, reuse_addr_len);

	server.sin_family = AF_INET;
	/**PORT is 53033**/
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	

	/* Name and activate the socket.
	 * Fill in code. */
	if(bind(sd, (struct sockaddr *)&server, sizeof(server)) < 0) {
		DIE("bind_error");
	}
	if (listen(sd, 7) < 0) {
		DIE("listen_error");
	}
	for(;;)
	{
		if ( (cd = accept(sd, (struct sockaddr*)&server, &server_len)) < 0) {	
				DIE("accept_error");
		}
		

		pthread_mutex_lock(&door_mutex);
		make_servelet_list();
		pthread_mutex_unlock(&door_mutex);
		pthread_create(&(door->thread), &attr, &serve_client, 	(void *)door);


	}/**forever for loop**/

	return NULL;
}

void make_servelet_list() {
	/**time_t time(time_t *t); **/
	if(door == NULL)
	{
		/*printf("\ndoor == NULL ,and it will make new list because new one come\n");*/
		door = (Servlet *)malloc(sizeof(Servlet));
		door->prev=NULL;door->next=NULL;
		door->fd=sd;
		door->cust_ip=cd;
		time( &(door->start) );
	}
	else
	{
		/*printf("\ndoor == %x ,and it will change list because new one come\n",door);*/
		Servlet * temp= (Servlet *)malloc(sizeof(Servlet));
		door->prev=temp;
		temp->next=door;
		door=temp;
		door->fd=sd;
		door->cust_ip=cd;
		time( &(door->start) );

	}
}
/*

typedef struct str_servlet {
  int fd;						
  pthread_t thread;				
  int cust_ip;					* IP addr of client					*
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