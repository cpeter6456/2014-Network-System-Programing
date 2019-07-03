/*
 *	sms_server.c :
 *	1. All global variables are defined in sms.h
 *	2. main, which does the following
 *		a) Initialize the circular list
 *		b) Fire off threads for collect_garb and listen_port
 *		c) Run the menu
 */

#include "sms.h"
Servlet *door=NULL;		/* entrance to doubly linked list		*/
Pending *pending_stack = NULL;	/* pile of stale servlets				*/
Stats * history=NULL;			/* start of stats linked list			*/
sem_t garbage_time;		/* schedule garbage collection			*/
pthread_attr_t attr;		/* set to make ALL threads detached		*/

pthread_mutex_t door_mutex;   
pthread_mutex_t history_mutex;   
pthread_mutex_t pending_mutex;   

void main(int argc, char **argv) {

	 pthread_t listen_port_pthread,collect_garb_pthread;


	 pthread_mutex_init(&door_mutex, NULL);
	 pthread_mutex_init(&history_mutex, NULL);
	 pthread_mutex_init(&pending_mutex, NULL);

	 pthread_attr_init(&attr);
	 pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);				/**set detach **/


	 pthread_create(&listen_port_pthread	, &attr, &listen_port, 	NULL);    /** 執行緒 listen_port 		**/
	 pthread_create(&collect_garb_pthread	, &attr, &collect_garb, NULL);    /** 執行緒 collect_garb 	**/

	 Menu_item mymenu;
	 menu(&mymenu);







}
