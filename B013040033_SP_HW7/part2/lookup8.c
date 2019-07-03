/*
 * lookup8 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet TCP Sockets
 * The name of the server is passed as resource. PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
/*
	struct hostent {
		char  *h_name;		//official name of host
		char **h_aliases;	//alias list
		int    h_addrtype;	//host address type
		int    h_length;	//length of address
		char **h_addr_list;	//list of addresses
	}
	#define h_addr h_addr_list[0]  /* for backward compatibility
*/

	static int sockfd;
	static struct sockaddr_in server;
	struct hostent *host;
	static int first_time = 1;

	if (first_time) {        /* connect to socket ; resource is server name */
		first_time = 0;

		/* Set up destination address. */
		server.sin_family = AF_INET;
		/* Fill in code. */
		if ( !(host = gethostbyname(resource)) ){
			DIE("gethostbyname_error");
		}
		server.sin_port = htons(5678);
		inet_pton(AF_INET, host->h_addr, &server.sin_addr);
		/* Allocate socket.
		 * Fill in code. */
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
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
