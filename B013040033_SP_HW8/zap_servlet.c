/*
 *	zap_servlet does the following :
 *		1) Append the message to an appropriate file
 *		2) Capture statistics and push onto stat stack
 *		3) Dispose of the Servlet
 */

#include "sms.h"

void zap_servlet(Servlet *victim) { /* remove and free memeory */
	pthread_mutex_lock(&history_mutex);

	time( &(victim->finish) );
	Stats *temp = malloc(sizeof(Stats));
	temp->next=NULL;
	temp->cust_ip = victim->cust_ip;
	temp->size = victim->chars_read;
	temp->aborted = victim->aborted;
	temp->connect = (int)(victim->finish - victim->start);
	if(history == NULL)
	{
		history=temp;
	}
	else
	{	
		
		temp->next = history;
		history = temp;
		
	}
	pthread_mutex_unlock(&history_mutex);
	
}

/**
typedef struct str_stats {
  int cust_ip;					/* IP addr of client					*
  char dest[MAXDEST];			/* addressee							*
  int size;						/* message size							*
  int connect;					/* connect time in secs					*
  int aborted;					/* boolean flag							*
  struct str_stats * next;		/* Linked list							*
} Stats;
**/