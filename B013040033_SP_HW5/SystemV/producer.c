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
		perror("producer::ftok");exit(1);
	}
	int shmblocks=atoi( argv[1] );
	printf("%d\n",shmblocks);
	struct packet *temp,*shmaddress;
	/**create share memory**/
	int  shmid = shmget(key, sizeof(struct packet)*shmblocks, IPC_CREAT|0777);
	if(shmid == -1){
		perror("producer:shmget");exit(1);
	}
		
	/**Mapping share memory(將share memory attached到某個process的空間內)**/
	if ( (shmaddress = (struct packet *)shmat(shmid, NULL, 0))<0 ){
		perror("producer::shmat");exit(1);
	}
	
	int i=0;
	sleep(5);
	for(i=0;i<MAX_SIZE;i++){
		

		if(i%shmblocks==0)
			temp = shmaddress;
		
		temp->id=i;
printf("p_id = %d\n",temp->id);
		temp++;
		
	}
	sleep(5);
	memset(shmaddress,'\0',sizeof(struct packet)*shmblocks);


	/**detach shared memory**/
	if( shmdt((const char*)shmaddress) < 0){
		perror("producer:shmdt");exit(1);
	}
	/**delete share memory**/
	shmctl(shmid, IPC_RMID, 0);
	return 0;
}
