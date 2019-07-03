/*
 * lookup7 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Unix TCP Sockets
 * The name of the socket is passed as resource.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static int sockfd;
	static struct sockaddr_un server;
	static int first_time = 1;
	int n;

	if (first_time) {     /* connect to socket ; resource is socket name */
		first_time = 0;

		/* Set up destination address.
		 * Fill in code. */
		sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

/**AF_UNIX用于同一台机器上的进程间通信，AF_INET对于IPV4协议的TCP和UDP 。**/
		/* Allocate socket. */
		server.sun_family = AF_UNIX;
		strcpy(server.sun_path,resource);

		

		/* Connect to the server.
		 * Fill in code. */
		if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
			DIE("connect_error");
		}
	}

	/* write query on socket ; await reply
	 * Fill in code. */
	if(send(sockfd, sought->word, WORD, 0)  < 0) {
		DIE("send_error");
	}
	
	if (recv(sockfd, sought->text, TEXT, 0) < 0) {
		DIE("recv_error");
	}
	if (strcmp(sought->text,"XXXX") != 0) {
		return FOUND;
	}

	return NOTFOUND;
}
