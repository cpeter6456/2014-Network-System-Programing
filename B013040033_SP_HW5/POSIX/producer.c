#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#define DIE(x) perror(x),exit(1)
struct packet {
	int 	id;
	short 	dataShort[5];
	long	dataLong[5];
	double	dataDouble[5];
	char	dataByte[6];	/**sizeof(char) == sizeof(byte)**/
};
#define MAX_SIZE 5000
int main(int argc,char** argv){
	int shmid;

	char* path ="/SHM_DEMO";

	char * shm;
	if(argc!=2)DIE("argv error");
	int shmblocks = atoi(argv[1]);
	int size = shmblocks*sizeof(struct packet);

	if ((shmid = shm_open(path,O_RDWR|O_CREAT,0666))== -1)
		DIE("Failed to create shm");
	if (ftruncate(shmid,size) == -1)
		DIE("Setting the size failed");

	if ((shm = mmap(NULL, size,PROT_READ|PROT_WRITE,MAP_SHARED,shmid,0)) == MAP_FAILED)
		DIE("mmap failed");

	int i=0;
	sleep(5);
	struct packet temp,*ptemp=(struct packet*)shm;
	for(i=0;i<MAX_SIZE;i++){
		temp.id=i;

		if(i%shmblocks==0){
			ptemp=(struct packet*)shm;
		}
		
		memcpy(ptemp,&temp,sizeof(struct packet));
		/*printf("ptemp = %x id = %d\n",ptemp,ptemp->id);*/
		ptemp++;
		usleep(100);
	}
	sleep(3);
	memset(shm,'\0',sizeof(struct packet)*shmblocks);

	shm_unlink ("/SHM_DEMO");
	return 0;
}
