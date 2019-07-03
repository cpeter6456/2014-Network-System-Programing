#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
	if(argc!=2)
		printf("Usage: mycat filename\n");
	else
	{
		FILE *inf;  /**want document*/
		char *input; 
        	
		input = argv[1]; 
		if((inf=fopen(input,"r"))==NULL)  
		      {/**if open error**/ 
		         perror("mycat.c:fopen");
	 		 exit(1);                              
		      }    

/**int  getc(FILE  *fp)當執行getc( )函數成功時，傳回值是所讀取的字元，
如果所讀取的是檔案結束字元，則此值是EOF，在stdio.h內，此值是 -1。**/
/**putc( int , FILE *);**/
		int c;
		while((c=getc(inf))!=EOF)  
               		putc(c,stdout); 
		 fclose(inf);            


	}
	
return 0;
}
