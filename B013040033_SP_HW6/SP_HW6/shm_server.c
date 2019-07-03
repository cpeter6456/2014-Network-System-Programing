/*
 * shm_server : wait on a semaphore; do lookup ; use shared
 *              memory for communication ; notify using same 
 *              semaphore (client raises by 2; server lowers
 *              by 1 both to wait and to notify)
 *
 *              argv[1] is the name of the local file
 *              argv[2] is the key for semaphores + shared mem
 */

#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "dict.h"

int main(int argc, char **argv) {
	int shmid,semid,ctl;
	long key;
	Dictrec * shm;
	struct stat stbuff;
	extern int errno;
	unsigned short vals[2] = { 1 , 0 };     /* Initial values of semaphores */

	union semun {                        	/* Needed for semctl for setup. */
		int             val;		/*SETVAL用的值*/
		struct semid_ds *buf;
		unsigned short  *array;		/*SETALL、GETALL用的数组值*/
	} setit ;

	struct sembuf wait = {1,-1,SEM_UNDO}; /* used BOTH to wait & notify */

	if (argc != 3) {
		fprintf(stderr,"Usage : %s <dictionary source> <Resource/Key>\n",argv[0]);
		exit(errno);
	}

	/* Create & initialize shared memory & semaphores */
	
	/* Verify database resource is present. */
	if (stat(argv[1],&stbuff) == -1)
		DIE(argv[1]);

	/* Get key from commandline argument. */
	key = strtol(argv[2],(char **)NULL,0);
	/* Map one record's worth of shared memory.
	 * The word of the sent value will be the request,
	 * and the text of the result will be returned as the answer.
	 *
	 * Fill in code. */
	

	int size = sizeof(Dictrec);

	shmid = shmget(key, size, IPC_CREAT|0666);
	if(shmid<0){perror("shm_sever:shmget");exit(1);}
	


	/* Allocate a group of two semaphores.  Use same key as for shmem.
	 * Fill in code. */

	semid = semget(key,2,IPC_CREAT|0666);
	if(semid < 0 ){perror("shm_sever:semget");exit(1);}

	/* Get shared memory virtual address.
	 * Fill in code. */
	if ( (shm = ( Dictrec *)shmat(shmid, NULL, 0))<0 ){
		perror("shm_sever:shmat");exit(1);
	}



	/* Set up semaphore group. */
	setit.array = vals;
	/* Fill in code. */

	ctl = semctl(semid,0,SETALL,setit);
	if(ctl<0){ perror("shm_sever:semctl_0");exit(1);}
/*
	ctl = semctl(semid,1,SETVAL,setit);
	if(ctl<0){ perror("shm_sever:semctl_1");exit(1);}
*/

	/* Main working loop */
	for (;;) {
		/*unsigned short vals[2] = { 1 , 0 }; =>semaphore(1).val ==1  */

		/* When we are first started, value is zero.  Client sets to two to wake us up.
		 * Try to decrement sem 1.  
		 * Then we will wait here until the semaphore is non-zero
		 *
		 * Fill in code. */

		/**use semctl debug**/
		ctl = semctl(semid,1,GETVAL,setit);
		/**in this case semop will wait until alert 2 , and when it wake up
		it will do sembuf wait, it means semaphore(1).val -1

		**/
		semop(semid, &wait, 1);

		ctl = semctl(semid,1,GETVAL,setit);



		/* Do the lookup here.  Write result directly into shared memory. */
		switch(lookup(shm,argv[1]) ) {
			case FOUND: 
				break;
			case NOTFOUND: 
				strcpy(shm->text,"XXXX");
				break;
			case UNAVAIL:
				DIE(argv[1]);
		}	
		/* Decrement again so that we will block at the top of the for loop again until a client wakes us up again.
		 *
		 * Fill in code. */
		semop(semid, &wait, 1);



	} /* end for */
}
