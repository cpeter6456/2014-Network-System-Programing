/*
 * usock_server : listen on a Unix socket ; fork ;
 *                child does lookup ; replies down same socket
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include "dict.h"

int main(int argc, char **argv) {
    
/**
进程间通信的一种方式是使用UNIX套接字，人们在使用这种方式时往往用的不是网络套接字，而是一种称为本地套接字的方式。这样做可以避免为黑客留下后门。

创建
使用套接字函数socket创建，不过传递的参数与网络套接字不同。域参数应该是PF_LOCAL或者PF_UNIX，而不能用PF_INET之类。本地套接字的通讯类型应该是SOCK_STREAM或SOCK_DGRAM，协议为默认协议。
	struct sockaddr_un {
		sa_family_t sun_family;               // AF_UNIX 
		char        sun_path[UNIX_PATH_MAX];  // pathname 
	};
**/
	struct sockaddr_un server;
    int sd,cd,n;
    Dictrec tryit;
	socklen_t server_len;

    if (argc != 3) {
      fprintf(stderr,"Usage : %s <dictionary source>"
          "<Socket name>\n",argv[0]);
      exit(errno);
    }
	/** int socket(int domain, int type, int protocol) **/
    /* Setup socket.
     * Fill in code. */
    sd = socket(AF_UNIX, SOCK_STREAM, 0);


    /* Initialize address.
     * Fill in code. */
	server.sun_family = AF_UNIX; 
	strcpy(server.sun_path, argv[2]);
	unlink( server.sun_path ); /* Otherwise bind could fail */

    /* Name and activate the socket.
     * Fill in code. */
	if( bind(sd, (struct sockaddr*)&server, sizeof(server) ) <0 ){
		DIE("bind_error");
	}

/**if no listen , it will error at cd = accept**/
	if (listen(sd, 7) < 0) {
		DIE("listen_error");
	}
    /* main loop : accept connection; fork a child to have dialogue */
    for (;;) {
		/* Wait for a connection.
		 * Fill in code. */
		server_len = sizeof(server);
		/** int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) **/
		if ((cd = accept(sd, (struct sockaddr*)&server, &server_len)) < 0) {	
			DIE("accept_error");
		}
		/* Handle new client in a subprocess. */
		switch (fork()) {
			case -1 : 
				DIE("fork_error");
			case 0 :
				close (sd);	/* Rendezvous socket is for parent only. */
				/* Get next request.
				 * Fill in code. */
				while (recv(cd, tryit.word, WORD, 0) != -1){

					/* Lookup request. */
					switch(lookup(&tryit,argv[1]) ) {
						/* Write response back to client. */
						case FOUND: 
							/* Fill in code. */
							if (send(cd, tryit.text, TEXT, 0)  < 0){
								DIE("send1_error");
							}
							break;
						case NOTFOUND: 
							/* Fill in code. */
							if (send(cd, "XXXX", TEXT, 0)  < 0) {
								DIE("send2_error");
							}
							break;
						case UNAVAIL:
							DIE(argv[1]);
					} /* end lookup switch */

				} /* end of client dialog */

				/* Terminate child process.  It is done. */
				exit(0);

			/* Parent continues here. */
			default :
				close(cd);
				break;
		} /* end fork switch */
    } /* end forever loop */
} /* end main */
