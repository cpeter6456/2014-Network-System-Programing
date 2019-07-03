/*
 *  timer.c : contains two timing functions to measure process time
 */

#include <sys/times.h>
#include <unistd.h>
#include <stdio.h>
#include "shell.h"



/**

1.实际时间（real time）：从命令行执行到运行终止的消逝时间
2.用户CPU时间（user CPU time）：命令执行完成花费的系统CPU时间，即命令在用户态中执行时的总和
3.系统CPU时间（system CPU time）：命令执行完成花费的系统CPU时间，即命令在核心态中执行时间的总和。

 times()  stores  the  current  process times in the struct tms that buf
       points to.  The struct tms is as defined in <sys/times.h>:
struct tms{
         clock_t  tms_utime;    user cpu time 
         clock_t  tms_stime;    system cpu time 
         clock_t  tms_cutime;   user cpu time of children 
         clock_t  tms_cstime;   system cpu time of children 
   };

   注： 这些时间都是时钟滴答数，而不是秒数。 使用sysconf函数能把时钟滴答数转为秒数， 这个函数把它的参数转换成在运行时定义的系统限制值或选项值， 返回类型为long
   ＿SC＿CLK＿TCK  is the number of clock ticks per second as it may vary from system to system。
   
 times()  returns  the  number of clock ticks that have elapsed since an
       arbitrary point in the past.  The return value may overflow the  possi‐
       ble  range  of  type  clock_t.  On error, (clock_t) -1 is returned, and
       errno is set appropriately.
**/

/* Storage for baseline times. */
static clock_t start_time;
static struct tms st_cpu;
static struct tms en_cpu;
/* Save a baseline of user and system CPU times, plus wallclock time. */
void set_timer(void) {
	/*struct tms tmbuf;*/
	/* Fill in code. */
	start_time = times(&st_cpu);
	
}


/* Get second set of times, and take delta to get wallclock time.  Display. */
void stop_timer(void) {
	/*struct tms tmbuf;*/
	clock_t end_time;

	end_time = times(&en_cpu);

	double ticks;

	ticks = sysconf(_SC_CLK_TCK);

	/* Get delta times and print them out.
	 *
	 * Fill in code. */


	printf("\nReal Time: %.2lf \nUser Time %.2lf\nSystem Time %.2lf\n",
	(double)((end_time-start_time)/ticks),
	(double)((en_cpu.tms_utime - st_cpu.tms_utime)/ticks),
	(double)((en_cpu.tms_stime - st_cpu.tms_stime)/ticks)
	);
	

}

