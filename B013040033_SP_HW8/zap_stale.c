/*
 *	zap_stale does the following
 *	for each stale client found
 *		1) Set abort flag
 *		2) Kill the thread
 *		3) Call disconnect
 */

#include "sms.h"

void zap_stale(void) {	/* call disconnect when needed */
	printf("How many seconds counts as stale ?");
	int limit;
	scanf("%d",&limit);

	Servlet *temp=NULL;
	temp = get_stale(limit);
	if(temp==NULL)
	{
		printf("Not Found a stale one\n");
	}
	else
	{
		temp->aborted=1;
		printf("Found a stale one\n");
		pthread_cancel(temp->thread);
		disconnect(temp);

	}
}
