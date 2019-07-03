#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include <sys/ipc.h>
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
	
	char* path ="/SHM_DEMO";
	if(argc!=2)DIE("argv error");
	int shmblocks=atoi( argv[1] );
	int shmid;
	char * shm;
	int size = shmblocks*sizeof(struct packet);
	/**create share memory**/
	if ((shmid = shm_open(path,O_RDWR|O_CREAT,0666))== -1)
		DIE("Failed to create shm");
	if (ftruncate(shmid,size) == -1)
		DIE("Setting the size failed");
	
	/**Mapping share memory(將share memory attached到某個process的空間內)**/
	if ((shm = mmap(NULL, size,PROT_READ|PROT_WRITE,MAP_SHARED,shmid,0)) == MAP_FAILED)
		DIE("mmap failed");

	
	struct packet *ptemp=(struct packet*)shm;

	long i=0,num[shmblocks],lastnum[shmblocks],lost=0,t=0;
	for(i=0;i<shmblocks;i++){num[i]=0;lastnum[i]=0;}
	i=0;
	struct packet *temp2 = malloc(sizeof(struct packet));
	while(1){
		
		t=i%shmblocks;
		if(t==0)
			ptemp = (struct packet*)shm;
		memcpy(temp2, ptemp, sizeof(struct packet));
		num[t]=temp2->id;
		if((num[t]-lastnum[t])>shmblocks){
			
			lost+=(num[t]-lastnum[t])/shmblocks-1 ;
			
		}
		/*printf("totla lost %d num %d lastnum %d\n",lost,num[t],lastnum[t]);*/
		ptemp++;
		lastnum[t]=num[t];
		i++;
		
		if(num[t]==(MAX_SIZE-1))break;
	}
	printf("LOST:%ld\n",lost);

	if(munmap(shm,size)<0)	
		DIE("munmap");
	
	return 0;
}
