/*
 *	sms.h :
 *	header file for short message service project
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/socket.h>
#include <stropts.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#define DIE(x)   perror(x),exit(errno)
#define SHORTMESS 128
#define MAXDEST 64
#define PORT 53033

/*---------------------------DATA STRUCTURES----------------------------*/
typedef struct str_servlet {
  int fd;						/* Socket connection					*/
  pthread_t thread;				/* For control							*/
  int cust_ip;					/* IP addr of client					*/
  char dest[MAXDEST];			/* addressee							*/
  char message[SHORTMESS];		/* message text							*/
  int chars_read;				/* size of mess so far					*/
  time_t start;					/* Connection opened					*/
  time_t finish;				/* Connection closed					*/
  int aborted;					/* boolean flag							*/
  struct str_servlet * prev;	/* Doubly linked list					*/
  struct str_servlet * next;	/* Doubly linked list					*/
} Servlet;

typedef struct str_stats {
  int cust_ip;					/* IP addr of client					*/
  char dest[MAXDEST];			/* addressee							*/
  int size;						/* message size							*/
  int connect;					/* connect time in secs					*/
  int aborted;					/* boolean flag							*/
  struct str_stats * next;		/* Linked list							*/
} Stats;

typedef struct str_pending {
  Servlet * head;				/* recently terminated					*/
  struct str_pending * tail;	/* linked list							*/
} Pending;

typedef struct {
  char * chat;
  void (* act)(void);
} Menu_item ;

/*---------------------------FUNCTION PROTOTYPES------------------------*/
void *collect_garb(void *);		/* garbage collector					*/
void *listen_port(void *);		/* body of port listener				*/
void menu(Menu_item *);			/* user interface menu					*/
void *serve_client(void *); 	/* body of servlet thread				*/
void disconnect(Servlet * );	/* close socket,timestamp,alert garbo	*/
void zap_servlet(Servlet *);	/* store info, unlink and free memory	*/
void list_conn(void);			/* list number of current connections 	*/
void list_stats(void);  		/* Summarise statistics					*/
void zap_stale(void);			/* Call disconnect when needed			*/
void zero_stats(void);  		/* re-start statistics					*/
Servlet *get_stale(int);		/* find ONE stale client				*/

/*---------------------------GLOBAL VARIABLES---------------------------*/
extern Servlet *door;			/* entrance to doubly linked list		*/
extern Pending *pending_stack;	/* pile of stale servlets				*/
extern Stats * history;			/* start of stats linked list			*/
extern sem_t garbage_time;		/* schedule garbage collection			*/
extern pthread_attr_t attr;		/* set to make ALL threads detached		*/

extern pthread_mutex_t door_mutex;   
extern pthread_mutex_t history_mutex;   
extern pthread_mutex_t pending_mutex;   
