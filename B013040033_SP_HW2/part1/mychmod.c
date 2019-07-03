#include<sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
int trans(int num8)
{
int r,num10=0, multi=1;
	
	while (num8 > 0)
	    {
		r = num8 % 10;
		num10 = num10 + (multi * r);

		multi = multi * 8;
		num8 = num8/10;

	    }


return num10;
}



int main(int argc, char *argv[])
{
struct stat buf;
 int  num10=0;
	if(argv[1][0]>='0'&& argv[1][0]<='9')
	{
		num10=trans(atoi(argv[1]));
	
	if (stat(argv[2],&buf)) {
		perror ("Couldn’t stat file");
	}
	else
	{
		buf.st_mode = (num10) ;

	}

	if (chmod(argv[2],buf.st_mode) ==-1)
		perror("chmod_error");
	}
	else
	{

		if (stat(argv[2],&buf)) {
			perror ("Couldn’t stat file");
		}
		int type=0;
		if(argv[1][2]=='r')
			type=4;
		if(argv[1][2]=='w')
			type=2;
		if(argv[1][2]=='x')
			type=1;

		int num=0;
		if(argv[1][0]=='u')
			num=100;
		if(argv[1][0]=='g')
			num=10;
		if(argv[1][0]=='o')
			num=1;
		if(argv[1][0]=='a')
			num=111;

		num=num*type;
		printf("num=%d\n",num);
		num10=trans(num);
		printf("num10=%d\n",num10);
		if(argv[1][1]=='+')
		{
			buf.st_mode |= num10 ;
			if (chmod(argv[2],buf.st_mode) ==-1)
				perror("chmod_error+");
		}
		else if(argv[1][1]=='-')
		{
			buf.st_mode &= ~(num10) ;
			if (chmod(argv[2],buf.st_mode) ==-1)
				perror("chmod_error-");
		}
		else if(argv[1][1]=='=')
		{
			int t1;

			if(argv[1][0]=='u')
				t1 = 0700 ;
			if(argv[1][0]=='g')
				t1 = 0070 ;
			if(argv[1][0]=='o')
				t1 = 0007 ;
			if(argv[1][0]=='a')
				t1 = 0777 ;
		/**use &= ~(t1) clean at first**/
			buf.st_mode &= ~(t1) ;
			if (chmod(argv[2],buf.st_mode) ==-1)
				perror("chmod_error_=_1");

		/**use add to avoid change other**/
			buf.st_mode |= num10 ;
			if (chmod(argv[2],buf.st_mode) ==-1)
				perror("chmod_error_=_2");
		}

		else
			printf("your command is error");


	}



	return 0;
}
/* From Teacher's PTT
struct stat buf;
7
8 if (stat("filename",&buf)) {
9 perror (“Couldn’t stat file”);
10 } else {
11
12 chmod u+s,u+x,g+x
13 buf.st_mode |= S_ISUID | S_IXUSR |
S_IXGRP ;
14  chmod g-w,o-w 
15 buf.st_mode &= ~(S_IWGRP | S_IWOTH);
16
17 if (chmod("filename",buf.st_mode) ==
-1)
18 perror("chmod : filename");*/
