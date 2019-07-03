/*
 *	get_stale merely locates a stale servlet
 *	It returns a valid pointer on success
 *	It returns NULL if no stale servlets are found
 */

#include "sms.h"

Servlet *get_stale(int secs) { /* find ONE stale client */
	time_t fin;
	time(&fin);
	pthread_mutex_lock(&door_mutex);
	Servlet *temp = door;
	while(temp!=NULL)
	{
		printf("%d\n", (int)(fin - temp->start));
		if( (fin - temp->start)>secs ){
			pthread_mutex_unlock(&door_mutex);
			return temp;
		}
		temp=temp->next;
	}
	pthread_mutex_unlock(&door_mutex);
	return NULL;

}
