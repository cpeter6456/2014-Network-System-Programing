#include <stdio.h>
#include <time.h>
 
int main(void)
{
    time_t t1 = time(NULL);
    struct tm *nPtr = localtime(&t1);
    if(nPtr==NULL)perror("mydate:localtime_error:");		
	
 char now[30];
 strftime(now, 60, "%b %d(%a), %Y %I:%M %p", nPtr);
   printf("%s\n", now);

return 0;
}
