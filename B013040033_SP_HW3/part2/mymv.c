#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>

#include <sys/stat.h>
#include <sys/types.h>
int main( int argc, char* argv[] ) {
	int f_flag=0,dir_flag=0;
	char dir_path[100];
	char *p;
	if(argc!=3){
		perror("mymv:argc not 3");
		exit(errno);
	}
	
	p=getcwd(dir_path, sizeof(dir_path));/**get now path**/
	if(p==NULL){
		perror("myls:getcwd");
		exit(errno);
	}
	DIR *dir_ptr;
    	struct dirent *direntp;
	if((dir_ptr=opendir(dir_path))==NULL){
		perror("myls:opendir");
		exit(errno);
	}	
	else{
		while((direntp=readdir(dir_ptr))!=NULL){/**read sub_dir and file in path**/
			
			if(strcmp(argv[2],direntp->d_name)!=0)continue;
			
			/**if argv[1] same as sub_file in path**/

			f_flag=1;
			struct stat info;
			char temp2[100];
			
			strcpy(temp2,dir_path);
			
			strcat(temp2,"/");
			strcat(temp2,direntp->d_name);
			/****  temp2 == old path + direntp->d_name****/
			if(stat(temp2,&info)==-1){
				perror(direntp->d_name);
				exit(1);
			}
			else{
				
				if(S_ISDIR(info.st_mode)){
					dir_flag=1;f_flag=0;/**select is directory or not**/
				}
			}
		}/**end while**/

	}

	
		
	
	if(dir_flag==0){/**argv[2] not directory**/
		if(f_flag == 1){
			char flag;			
			printf("mymv: do you want overwrite '%s'?(y/n)",argv[2]);
			scanf("%c",&flag);	
			if(flag!='y')exit(0);
		}
		if( rename(argv[1],argv[2])==-1){
			perror("mymv:rename_1");
			exit(errno);
		}
	}
	else if(dir_flag==1){
		char temp[100];
		strcpy(temp,dir_path);
		strcat(temp,"/");
		strcat(temp,argv[2]);
		strcat(temp,"/");
		strcat(temp,argv[1]);
		
		struct stat info2;
		if ((stat (temp, &info2)) == -1){/**file not exit in directory**/
			
			
		}
		else{/**file not exit in directory, so need ask for overwrite**/
			char flag;			
			printf("mymv:overwrite '%s'in directory %s?(y/n)",argv[1],argv[2]);
			scanf("%c",&flag);	
			if(flag!='y')exit(0);
		}


		if( rename(argv[1],temp)==-1){
			perror("mymv:rename_2");
			exit(errno);
		}


	}


	return 0;
}
