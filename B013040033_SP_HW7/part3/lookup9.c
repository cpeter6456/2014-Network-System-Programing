/*
 * lookup9 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet UDP Sockets
 * The name of the server is passed as resource. PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static int sockfd;
	static struct sockaddr_in server;
	struct hostent *host;
	static int first_time = 1;
	socklen_t server_len, reuse_addr_len;
	if (first_time) {  /* Set up server address & create local UDP socket */
		first_time = 0;

		/* Set up destination address.
		 * Fill in code. */
		server.sin_family = AF_INET;
		if ( !(host = gethostbyname(resource)) ){
			DIE("gethostbyname_error");
		}
		server.sin_port = htons(5678);
		inet_pton(AF_INET, host->h_addr, &server.sin_addr);
		/* Allocate a socket.
		 * Fill in code. */
		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	}

	/* Send a datagram & await reply
	 * Fill in code. */
	server_len = sizeof(server);
	if( sendto(sockfd, sought->word, WORD, 0, (struct sockaddr*)&server, server_len) < 0) {
		DIE("sendto_error");
	}
	if ( recvfrom(sockfd, sought->text, TEXT, 0, (struct sockaddr*)&server, &server_len) < 0) {
		DIE("recvfrom_error");
	}
	if (strcmp(sought->text,"XXXX") != 0) {
		return FOUND;
	}

	return NOTFOUND;
}
