/*
 *	disconnect is passed a pointer to a Servlet
 *	It should perform the following :
 *		1) Take the servlet out of the circular list
 *		2) Close the socket connection
 *		3) Create a Pending item and push pending stack
 *		4) Alert the garbage collector
 */

#include "sms.h"

void disconnect(Servlet *done) {
	Servlet *temp=door;
	if(done==NULL){DIE("done == NULL");}
	
	char mymessage[SHORTMESS];
	if(done->aborted==1)
	{
		memset(mymessage, '\0',SHORTMESS );
		strcpy(mymessage,"Sorry time is up\n");
		if( send(done->cust_ip, mymessage, SHORTMESS, 0) < 0) {
			DIE("send_error");
		}
	}
	else
	{
		memset(mymessage, '\0',SHORTMESS );
		strcpy(mymessage,"Bye Bye\n");
		if( send(done->cust_ip, mymessage, SHORTMESS, 0) < 0) {
			DIE("send_error");
		}
	}

	pthread_mutex_lock(&door_mutex);
	if(done->prev==NULL)
	{
		door=done->next;
		if(door!=NULL)/**when door==NULL => door==done && just one in list**/
			door->prev=NULL;
	}
	else if(done->prev!=NULL && done->next==NULL)
	{
		done->prev->next=door->next;
	}
	else if(done->prev!=NULL && done->next!=NULL)
	{
		done->prev->next=door->next;
		door->next->prev=door->prev ;
	}
	else
	{
		DIE("1) Take the servlet out of the circular list");
	}
	close(done->cust_ip);
	pthread_mutex_unlock(&door_mutex);


	pthread_mutex_lock(&pending_mutex);
	Pending *pend = (Pending *)malloc(sizeof(Pending));
	pend->head=done;pend->tail=NULL;
	if(pending_stack == NULL)
	{
		pending_stack = pend;
	}
	else
	{
		pend->tail = pending_stack;
		pending_stack = pend;
	}
	pthread_mutex_unlock(&pending_mutex);

	sem_post(&garbage_time);

}
