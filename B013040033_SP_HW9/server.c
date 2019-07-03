#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "assert.h"
//#include "zhelpers.h"
/** 
核心提供了一套標準的環狀雙向鏈結串列，以供核心程式使用。
有關於此串列的程式與相關資訊都存放在 裡。當程式須要使用此串列機制時，只須將此標頭檔引入即可。除了開發核心程式外，在一般的程式中我們亦可借助於此機制，將list.h拷貝一份至我們的開發專案中。
在list.h中，定義一個list_head的結構，此結構包含了兩個指標用以指向後一個節點與前一個節點：
struct list_head {
          struct list_head *next, *prev;
};
 **/
#include "list.h"
#define HSIZE 128
#define LSIZE 30	

 struct list_node
{
	char myid[LSIZE];
	char group[LSIZE];
	struct list_head node;

};
typedef struct list_node mylist_node;


 struct msg
{
	char target_id[LSIZE];
	int type;
	
	char myid[LSIZE];
	char msg[HSIZE];
	int success;
	int private;
};
typedef struct msg mymsg;


int main (void)
{
	void *context = zmq_ctx_new ();
	// Socket to talk to clients
	void *responder = zmq_socket (context, ZMQ_REP);
	int rc=zmq_bind (responder, "tcp://*:5555");
	/**assert(0) will exit**/
	assert(rc==0);

	void *contextpub = zmq_ctx_new ();
	void *publisher = zmq_socket (contextpub, ZMQ_PUB);
	rc=zmq_bind (publisher, "tcp://*:5563");
	assert(rc==0);


	/**head in list.h had benn define,it's equal top**/
	LIST_HEAD(head);
	mylist_node *temp_node;
    

	while (1) {

		struct list_head *iterator;
		// Wait for next request from client
		mymsg from_client;
		from_client.type=88;
		zmq_recv (responder,&from_client,sizeof(mymsg), 0);
		//printf ("Received type=%d id =%s\n",from_client.type,from_client.myid);
		
		fflush(stdout);
		mymsg rep,pub;
		char st[HSIZE];
		int s_flag=1;
		switch(from_client.type)
		{
			case 1: /**Hellow format**/
				printf("%s is online \n",from_client.myid);
				fflush(stdout);
				temp_node = malloc(sizeof(mylist_node));
				int i=0;
				for(i=0;i<LSIZE;i++){
					temp_node->myid[i]=from_client.myid[i];
					temp_node->group[i]=from_client.target_id[i];
				}
				list_add(&temp_node->node, &head);

				/**all member**/
				list_for_each(iterator, &head) {
			        //printf("id = %s    ", list_entry(iterator,mylist_node, node)->myid);
			        //printf("group = %s\n", list_entry(iterator,mylist_node, node)->group);
			    }

			    
				rep.type=4;rep.success=1;
				zmq_send(responder,&rep,sizeof(mymsg), 0) ;
			break;

			case 2: /**Exit format**/

				printf("%s is offline\n",from_client.myid);

				list_for_each(iterator, &head) {
					mylist_node *temp = list_entry(iterator,mylist_node, node);
					if(strcmp(temp->myid,from_client.myid)==0){
						list_del( &(temp->node) );
						free(temp);
						break;
					}
				}

				
				rep.type=4;rep.success=1;
				zmq_send(responder,&rep,sizeof(mymsg), 0);
			break;

			case 3: /**Normal format**/
				//printf("In the case 3 myid=%s\n",from_client.myid);
				s_flag=0;rep.private=1;
				strncpy(st,from_client.msg,HSIZE);
				/****/
				list_for_each(iterator, &head) {
					mylist_node *temp = list_entry(iterator,mylist_node, node);
					if(from_client.private==0){
						//printf("send group %s\n",from_client.msg);
						/**group**/
						if(strcmp(temp->group,from_client.target_id)==0){
							//printf("%s\n","GGG" );
							s_flag=1;
							rep.private=0;
							strncpy(pub.target_id,from_client.target_id,LSIZE);
							pub.private=0;strncpy(pub.myid,from_client.myid,LSIZE);
							pub.type=2;strncpy(pub.msg,from_client.msg,HSIZE);
							zmq_send(publisher,&pub,sizeof(mymsg), 0);
							break;
						}
					}
					else{

						/**find @name exist**/
						//printf("@@%s\n",from_client.target_id );
						if(strcmp(temp->myid,from_client.target_id)==0){
							//printf("%s\n","IN@@@");
							s_flag=1;
							strncpy(pub.target_id,from_client.target_id,LSIZE);
							pub.private=1;strncpy(pub.myid,from_client.myid,LSIZE);
							//printf("myid%s\n",from_client.myid );
							pub.type=2;strncpy(pub.msg,from_client.msg,HSIZE);
							zmq_send(publisher,&pub,sizeof(mymsg), 0);
							break;
						}

					}
				}/**each member**/
				//printf("s_flag=%d ",s_flag);
				if(s_flag==1)	
					rep.success=1;
				else			
					rep.success=0;
				rep.type=4;
				zmq_send(responder,&rep,sizeof(mymsg), 0);
				fflush(stdout);

			break;

			case 4: /**Result format**/
			break;

			default:
				printf("\nERROR type\n");
			break;

		}

			// Do some 'work'
			
			// Send reply back to client
		/*
		mymsg rep;
		rep.type=4;
		zmq_send(responder,&rep,sizeof(mymsg), 0) ;*/

	}
	// We never get here but if we did, this would be how we end
	zmq_close (responder);
	zmq_ctx_destroy (context);



	return 0;
}


