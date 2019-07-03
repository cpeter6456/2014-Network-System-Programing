/*
 * lookup6 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by shared memory
 *
 * Synchronization is by two semaphores:
 *   One excludes other clients while we talk to server
 *   The other is used BOTH to alert the server and to await his reply
 *   Client raises a semaphore by 2. Server drops by 1 before and
 *   after doing lookup
 * The same key (which is passed as resource) is used BOTH 
 * for shared memory & semaphores.
 */

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h> 
#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static int shmid,semid;
/**
当base值为0时则是采用10进制做转换，但遇到如''0x''前置字符则会使用16进制做转换。
**/
	long key = strtol(resource,(char **)NULL,0);
	static Dictrec * shm;
/**
semop操作中：sembuf结构的sem_flg成员可以为0、IPC_NOWAIT、SEM_UNDO 。为SEM_UNDO时，它将使操作系统跟踪当前进程对这个信号量的修改情况，如果这个进程在没有释放该信号量的情况下终止，操作系统将自动释放该进程持有的。
**/
	struct sembuf grab    = {0,-1,SEM_UNDO};   /* mutex other clients  */
	struct sembuf release = {0,1,SEM_UNDO};    /* release mtx to other clients */
	struct sembuf alert   = {1,2,SEM_UNDO};    /* wake up server       */
	struct sembuf await   = {1,0,0};           /* wait for server      */
	static int first_time = 1;

	if (first_time) {        /* attach to shared memory & semaphores */
		first_time = 0;

		/* Connect to shared memory.
		 * Fill in code. */
		int  size = sizeof(Dictrec);

		shmid = shmget(key, size, IPC_CREAT|0666);
		if(shmid<0){perror("client:lookup6:shmget");exit(1);}
		/* Get shared memory virtual address.
		 * Fill in code. */
	/**Mapping share memory(將share memory attached到某個process的空間內)**/
		if ( (shm = (Dictrec *)shmat(shmid, NULL, 0))<0 ){
			perror("client:lookup6:shmat");exit(1);
		}

		/* Get semaphore.
		 * Fill in code. */
		semid = semget(key,2,IPC_CREAT|0666);
		if(semid < 0 ){perror("client:lookup6:semget");exit(1);}


	}

	/* Reserve semaphore so other clients will wait.
	 * Fill in code. */

	semop(semid, &grab, 1);
	strcpy(shm->word,sought->word);

	/* Alert server.  Bump semaphore 1 up by 2.
	 * Fill in code. */
	semop(semid, &alert, 1);

	/* Wait for server to finish.  Server will have set sem 1 to zero.
	 * Fill in code. */
	semop(semid, &await, 1);
	/* Done using the server.  Release to other clients.
	 * Fill in code. */
	semop(semid, &release, 1);	


	if (strcmp(shm->text,"XXXX") != 0) {
		strcpy(sought->text,shm->text);
		return FOUND;
	}

	return NOTFOUND;
}
