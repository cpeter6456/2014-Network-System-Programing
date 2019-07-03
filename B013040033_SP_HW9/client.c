#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#define HSIZE 128
#define LSIZE 30	

typedef struct 
{
	char target_id[LSIZE];
	int type;

	char myid[LSIZE];
	char msg[HSIZE];
	int success;
	int private;
}mymsg;
/**
type 1 is hellow
type 2 is exit
type 3 is normal
type 4 is result
**/
void *requester;
char name[LSIZE];
char group[LSIZE];
void *contextsub ;
static void *thread_sub(void *vptr_args)
{

	contextsub = zmq_ctx_new ();
	void *subscriber = zmq_socket (contextsub, ZMQ_SUB);
	zmq_connect (subscriber, "tcp://localhost:5563");
/**
filter "" will always receice it

if i use send && sen_more 

zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, group,strlen(group));
zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, name, strlen(name) );

then SUB will receive first frame is gorup or name
**/
	//zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, "", 0 );
	zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, group,strlen(group));
	zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, name, strlen(name) );
    while(1)
 	{
	 	mymsg get;
	 	get.type=99;
		if(zmq_recv (subscriber,&get,sizeof(mymsg), 0) !=-1){
			//printf("target_id = %s\n",get.target_id);

/**
i use strcmp is to avoid this case:
in group aaa2 talk,group aaa will receive,
**/
			if(get.private==1&&(strcmp(name,get.target_id)==0)){
				printf ("<<Private Msg>>%s : %s", get.myid,get.msg);
			}
			if(get.private==0&&(strcmp(group,get.target_id)==0))
				printf ("%s : %s", get.myid,get.msg);
			//printf(">>>>");
			fflush(stdout); 
		}
		else{
			/**zmq_recv == -1**/
			break;
		}


 	}
 	zmq_close (subscriber);
    return NULL;
}

int main(int argc, char* argv[])
{
	void *context = zmq_ctx_new ();
	// Socket to talk to server
	printf ("Connecting to server...\n");
	requester = zmq_socket (context, ZMQ_REQ);
	zmq_connect (requester, "tcp://localhost:5555");

	
	

	

	strncpy(name ,argv[2],LSIZE);

	
	strncpy(group ,argv[1],LSIZE);
	if(strcmp(name,"exit")==0){
		printf ("Please don't input name as exit\n");
		return 0;
	}

	pthread_t thread;
	
 	if (pthread_create(&thread, NULL, thread_sub, NULL) != 0)
    {
        perror("creat_error");
		exit(1);
    }
/**Sending Hello**/
    mymsg hellow;
	hellow.type=1;
	strcpy(hellow.myid ,name);
	strcpy(hellow.target_id ,group);

	printf ("Sending Hello ...\n");
	zmq_send (requester,&hellow,sizeof(mymsg),0);

	mymsg get;
 	get.type=99;
	if(zmq_recv (requester,&get,sizeof(mymsg), 0) !=-1){
		//printf ("get  repply%d success=%d...\n", get.type,get.success);
	}

	mymsg normal;
	strcpy(hellow.myid ,name);
	normal.type=3;
	int exit_flag=0;//printf(">>>>");
    while(1)
	{
		
		normal.private=0;
		normal.type=3;
		char st[HSIZE];
		fgets(st,HSIZE,stdin);
		if(st[0]=='@'){
			if(strcmp(st,"@exit\n")==0){
				//printf("%s\n","EXIT" );
				normal.type=2;
				exit_flag=1;

/**
use zmq_term(contextsub) make zmq_recv(contextsub,....) return -1

if return -1 , thread break thread_while(1) and do zmq_close(contextsub)
use zmq_close(contextsub) , because zmq_term(contextsub) will block
untill contextsub be closed

and then because exit_flag=1 so break
**/
				zmq_term(contextsub );
			}
			else{
				normal.private=1;
				char *test = strtok(st, "@ \n");
				//printf("%s len = %d\n",test ,strlen(test));
				strcpy(normal.target_id,test);
				strcpy(normal.myid,name);
				test+=strlen(test)+1;
				//printf("%s len = %d\n",test ,strlen(test));
				strcpy(normal.msg ,test);

			}

		}
		/**group talk**/
		else{
			
			normal.private=0;
			strcpy(normal.target_id ,group);
			strcpy(normal.myid ,name);
			strcpy(normal.msg ,st);

		}
		zmq_send (requester,&normal,sizeof(mymsg),0);

		mymsg get;
	 	get.type=99;
		if(zmq_recv (requester,&get,sizeof(mymsg), 0) !=-1){
			//printf("HAHA%d %d %d\n",get.type,get.private,get.success);

			if( get.type==4 && get.private==1 && get.success==0)
				printf ("**%s is offline**\n",normal.target_id);
		}
		if(exit_flag)break;
	}

	
	zmq_close (requester);
	zmq_ctx_destroy (context);

	return 0;
}