/*
 *	collect_garb :
 *	This function has the following responsibilities :
 *		1) Call zap_servlet
 *		2) Remove the head of the pending tray
 *	collect_garb is activated by the garbage_time semaphore
 */

#include "sms.h"

void *collect_garb(void *info) { /* garbage collector */
/**int sem_wait(sem_t * sem);**/
 while(1)
 {
 	sem_wait(&garbage_time); 
 	Pending *pend = pending_stack;
 	Servlet *temp = pend->head;

 	zap_servlet(temp);

 	pthread_mutex_lock(&pending_mutex);
 	pending_stack = pending_stack->tail;
 	pthread_mutex_unlock(&pending_mutex);

 	free(pend);

 }	




	return NULL;
}
