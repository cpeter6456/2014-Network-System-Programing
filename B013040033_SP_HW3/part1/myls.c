#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
void list_directory( char *dir_path );
void mode_to_letters(int,char []);
int F_flag,R_flag;
int main( int argc, char* argv[] ) {
	char *p;
	char buf[80];
	F_flag=0;R_flag=0;
	
	if( (argc>=2 && !strcmp(argv[argc-1],"-F")) ||  (argc>=3 && !strcmp(argv[argc-2],"-F") ) )
		F_flag=1;
	if( (argc>=2 && !strcmp(argv[argc-1],"-R")) ||  (argc>=3 && !strcmp(argv[argc-2],"-R") ) )
		R_flag=1;
	printf("f=%d r=%d \n",F_flag,R_flag);
/**getcwd() shall return the buf argument. Otherwise, getcwd() shall return a null pointer and set errno to indicate the error**/
    	p=getcwd(buf, sizeof(buf));
	if(p==NULL)	   
	{
		perror("myls:getcwd");
		exit(errno);
	}
	
	
	list_directory( buf );
	
	return 0;
}

void list_directory( char *dir_path ) {
	
	char namelist[100][100];
	int i=0;
    	DIR *dir_ptr;
    	struct dirent *direntp;
	printf("\n\n%s\n",dir_path);
	if((dir_ptr=opendir(dir_path))==NULL)
        {
		perror("opendir");
		exit(1);
	}	
	else
	{
		while((direntp=readdir(dir_ptr))!=NULL)
		{
			
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
				
				char *uid_to_name(),*ctime(),*gid_to_name(),*filemode();
				void mode_to_letters();
				char modestr[11],t_filename[30];
	
				strcpy(t_filename, direntp->d_name);


				mode_to_letters(info.st_mode,modestr);
				if(modestr[0]=='d'){
						
					if(R_flag==1 ){
						
						strcpy(namelist[i],t_filename);
						
						i++;
						
					}

						
				}
				int d=0;
				if(F_flag==1){
					if(modestr[0]=='d'){
						d=1;
						strcat(t_filename,"/");
					}
					if(d!=1 && ( modestr[3]=='x' || modestr[6]=='x' || modestr[9]=='x' ) ){
						
						strcat(t_filename,"*");
					}
				}
	
				if( strcmp(t_filename,"./") && strcmp(t_filename,"../") && strcmp(t_filename,".") && strcmp(t_filename,"..") )
				printf("%s\n",t_filename);
			}


		}/*end while*/
		char temp[100],path[100];
		strcpy(path,dir_path);
		if(R_flag==1){
			int t=0;
			
			for(;t<i;t++)
			{
				if(strcmp(namelist[t],".")==0 || strcmp(namelist[t],"..")==0 )
					continue;
				strcpy(temp,path);
				strcat(path,"/");
				strcat(path,namelist[t]);
				/*****change path *****/
				/***this is recursive****/
				list_directory(path);				
				
				strcpy(path,temp);/**path recorver**/

			}
		
		}
		
		
		closedir(dir_ptr);
	}


	return ;
}

void mode_to_letters(int mode,char str[])
{
    strcpy(str,"----------");
    if(S_ISDIR(mode))
        str[0]='d';
    if(S_ISCHR(mode))
        str[0]='c';
    if(S_ISBLK(mode))
        str[0]='b';
 
    if(mode & S_IRUSR)
        str[1]='r';
    if(mode & S_IWUSR);
        str[2]='w';
    if(mode & S_IXUSR)
        str[3]='x';
 
    if(mode & S_IRGRP)
        str[4]='r';
    if(mode & S_IWGRP)
        str[5]='w';
    if(mode & S_IXGRP)
        str[6]='x';
 
    if(mode & S_IROTH)
        str[7]='r';
    if(mode & S_IWOTH)
        str[8]='w';
    if(mode & S_IXOTH)
        str[9]='x';
}
