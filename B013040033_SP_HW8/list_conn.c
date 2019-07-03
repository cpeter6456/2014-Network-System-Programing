/*
 *	list_conn looks for live connections
 *	It reports the total number and the age of the oldest
 */

#include "sms.h"

void list_conn(void) {	/* list number of current connections */
 	int lives=0,old_time=0;
 	pthread_mutex_lock(&door_mutex);
 	Servlet *temp=door;
 	time_t now;
 	time( &now);
	while(temp!=NULL){
		if(  (now - temp->start) > old_time)
			old_time=(now - temp->start);
		lives++;
		temp=temp->next;
	}
	pthread_mutex_unlock(&door_mutex);
	printf("There are %d live connections\nThe oldest began %d seconds ago\n",lives,old_time);

 	
}
