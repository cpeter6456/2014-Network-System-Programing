#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
int main()
{
    	char buf[80];
	char *p;
/**getcwd() shall return the buf argument. Otherwise, getcwd() shall return a null pointer and set errno to indicate the error**/
    	p=getcwd(buf, sizeof(buf));
	if(p==NULL)	   
	{
		perror("prindir:getcwd");
		exit(errno);
	}
	else	
		printf("%s\n", buf);
return 0;
}
