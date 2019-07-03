#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include <sys/ipc.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

struct packet {
	int 	id;
	short 	dataShort[5];
	long	dataLong[5];
	double	dataDouble[5];
	char	dataByte[6];	/**sizeof(char) == sizeof(byte)**/
};
#define MAX_SIZE 5000

int main(int argc,char** argv){
	/**key_t is int/long in 32/64 bits System**/
	/**在一般的UNIX實現中，是將文件的索引節點號取出，
	前面加上子序號得到key_t的返回值。
	如指定文件的索引節點號爲65538，換算成16進製爲0x010002，
	而你指定的ID值爲38，換算成16進製爲0x26，
	則最後的key_t返回值爲0x26010002。**/
	key_t key = ftok("/etc", 'r');
	if(key ==-1){
		perror("consumer::ftok");exit(1);
	}
	int shmblocks=atoi( argv[1] );
	
	struct packet *temp,*shmaddress;
	/**create share memory**/
	int  shmid = shmget(key, sizeof(struct packet)*shmblocks, IPC_CREAT|0777);
	if(shmid == -1){
		perror("consumer:shmget");exit(1);
	}
		
	/**Mapping share memory(將share memory attached到某個process的空間內)**/
	if ( (shmaddress = (struct packet *)shmat(shmid, NULL, 0))<0 ){
		perror("consumer::shmat");exit(1);
	}
	temp = shmaddress;
	long i=0,num[shmblocks],lastnum[shmblocks],lost=0,t=0;
	for(i=0;i<shmblocks;i++){num[i]=0;lastnum[i]=0;}
	i=0;
	struct packet *temp2 = malloc(sizeof(struct packet));
	while(1){
		
		t=i%shmblocks;
		if(t==0)
			temp = shmaddress;
		memcpy(temp2, temp, sizeof(struct packet));
		num[t]=temp2->id;
		if((num[t]-lastnum[t])>shmblocks){
			
			lost+=(num[t]-lastnum[t])/shmblocks-1 ;
			
		}
		/*printf("totla lost %d num %d lastnum %d\n",lost,num[t],lastnum[t]);*/
		temp++;
		lastnum[t]=num[t];
		i++;
		
		if(i>(MAX_SIZE) && num[t]==(MAX_SIZE-1))break;
	}
	printf("LOST:%ld\n",lost);



	/**detach shared memory**/
	if( shmdt((const char*)shmaddress) < 0){
		perror("producer:shmdt");exit(1);
	}
	/**delete share memory**//*
	shmctl(shmid, IPC_RMID, 0);*/
	return 0;
}
