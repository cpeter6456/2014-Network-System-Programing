/*
 *	zero_stats just frees the stats stack (means "history")
 */

#include "sms.h"

void zero_stats(void) { /* re-start statistics */
/**
i think clean stat may use mutex
**/

 	Stats * temp;
 	pthread_mutex_lock(&history_mutex);
 	while(history!=NULL)
 	{
 		temp=history;
 		history=history->next;
 		free(temp);
 	}
 	pthread_mutex_unlock(&history_mutex);

}
