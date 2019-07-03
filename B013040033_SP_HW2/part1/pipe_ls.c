#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>		//pipe
#include <fcntl.h>		//parameter for open
#include <sys/wait.h>		//waitpid
#include <signal.h>
#include <errno.h>
#define MAX 101

int main ()
{
int fd[2], ppid = 0;
if(pipe(&fd[0]) <0 )
	perror("pipe_ls:pipe_error:");
ppid = fork();

if(ppid > 0){

	/**dup2(old,target)**/
	close(fd[1]);
	if(dup2(fd[0], STDIN_FILENO) <0 )
		perror("P_process_dup2:");
	char a;
	while(scanf("%c",&a)!=EOF)
	printf("%c",a);

	waitpid(ppid, NULL, 0);
}
else if (ppid==0){

	close(STDIN_FILENO);
	close(fd[0]);
	if(dup2(fd[1], STDOUT_FILENO) < 0)
		perror("C_process_dup2:");
	if( execlp("ls","ls", "-l",NULL) < 0 )
            /* Handle error return from exec */
			exit(errno);
}				
else
{
	perror("fork");
      	exit(errno);

}
return 0;
}
