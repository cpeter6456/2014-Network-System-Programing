#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int  main() {

 struct utsname uts;

 if( uname(&uts) == -1 ) {
	 perror("hostinfo.c");
	 exit(1);
 }
long id = gethostid();

 printf("hostname: %s\n", uts.nodename);
 printf("Linux release: %s %s\n", uts.release,uts.machine);

 printf("hostid: %ld\n", id);
return 0;
 }
