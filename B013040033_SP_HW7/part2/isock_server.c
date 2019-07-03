/*
 * isock_server : listen on an internet socket ; fork ;
 *                child does lookup ; replies down same socket
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

#include "dict.h"

int main(int argc, char **argv) {
/**
sin_family指代协议族，在socket编程中只能是AF_INET
sin_port存储端口号（使用网络字节顺序）
sin_addr存储IP地址，使用in_addr这个数据结构
sin_zero是为了让sockaddr与sockaddr_in两个数据结构保持大小相同而保留的空字节。
s_addr按照网络字节顺序存储IP地址
struct  sockaddr_in {
short  int  sin_family;              * Address family : AF_INET
unsigned  short  int  sin_port;      * Port number 
struct  in_addr  sin_addr;           * Internet address 
unsigned  char  sin_zero[8];         * Same size as struct sockaddr 
};
struct  in_addr {
unsigned  long  s_addr;
};

typedef struct in_addr {
union {
            struct{
                        unsigned char s_b1,
                        s_b2,
                        s_b3,
                        s_b4;
                        } S_un_b;
           struct {
                        unsigned short s_w1,
                        s_w2;
                        } S_un_w;
            unsigned long S_addr;
          } S_un;
} IN_ADDR;
**/
	static struct sockaddr_in server;
	int sd,cd,n;
	Dictrec tryit;
	socklen_t server_len;

	if (argc != 2) {
		fprintf(stderr,"Usage : %s <datafile>\n",argv[0]);
		exit(1);
	}

	/* Create the socket.
	 * Fill in code. */
	sd = socket(AF_INET, SOCK_STREAM, 0);
	/* Initialize address.
	 * Fill in code. */
	server.sin_family = AF_INET;
	server.sin_port = htons(5678);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	int reuse_addr = 1;
	socklen_t reuse_addr_len = sizeof(reuse_addr);
/** 
int setsockopt(int sockfd, int level, int optname,const void *optval, socklen_t optlen)
**/
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, reuse_addr_len);
	/* Name and activate the socket.
	 * Fill in code. */
	if(bind(sd, (struct sockaddr *)&server, sizeof(server)) < 0) {
		DIE("bind_error");
	}
	if (listen(sd, 7) < 0) {
		DIE("listen_error");
	}
	/* main loop : accept connection; fork a child to have dialogue */
	for (;;) {

		/* Wait for a connection.
		 * Fill in code. */
		server_len = sizeof(server);
		if ( (cd = accept(sd, (struct sockaddr*)&server, &server_len)) < 0) {	
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
/* Lookup the word , handling the different cases appropriately */
					switch(lookup(&tryit,argv[1]) ) {
						/* Write response back to the client. */
						case FOUND:
/** ssize_t send(int sockfd, const void *buf, size_t len, int flags); **/
							if(send(cd, tryit.text, TEXT, 0) < 0) {
								DIE("send");
							}
							/* Fill in code. */
							break;
						case NOTFOUND:
							if(send(cd, "XXXX", TEXT, 0) < 0) {
								DIE("send");
							}
							/* Fill in code. */
							 break;
						case UNAVAIL:
							DIE(argv[1]);
					} /* end lookup switch */
				} /* end of client dialog */
				exit(0); /* child does not want to be a parent */

			default :
				close(cd);
				break;
		} /* end fork switch */
	} /* end forever loop */
} /* end main */
