/*
 *	list_stats produces a report from the stats heap
 */

#include "sms.h"

void list_stats(void) { /* Summmarise statistics */
	double total_size=0,total_time=0,total_aborted=0,total_stats=0;
	
	pthread_mutex_lock(&history_mutex);
	Stats *temp = history;
	while(temp!=NULL)
	{
		
		total_stats++;
		total_size+=temp->size;
		total_time+=temp->connect;
		total_aborted+=temp->aborted;

		temp=temp->next;
	}
	pthread_mutex_unlock(&history_mutex);
	if(total_stats==0)
	{
		printf("%s\n","NULL history");
	}
	else
	{
		printf("%.0lf connections %.1lf%% aborted,average connect = %.1lf sec\n",total_stats,(total_aborted/total_stats)*100,total_time/total_stats);
		printf("average size = %.1lf bytes\n",total_size/total_stats);
	}

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