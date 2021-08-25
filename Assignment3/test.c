#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	long i=0;
	int inputnice;
	scanf("%d", &inputnice);
	inputnice = inputnice*100;
	pid_t pid = fork();
	struct timeval t1;

	if(pid<0)
	{
		printf("cannot fork");
		return 1;
	}
	gettimeofday(&t1, NULL);
	if(pid==0)
	{
		struct timeval t2;
		printf("child process created with process ID %d\n", getpid());
		int res = syscall(440, getpid(), inputnice);
		perror("rtnice");
		for(int i=0;i<100000000;i++);
		gettimeofday(&t2, NULL);
		printf("Child process ended with execution time %ld \n", ((t2.tv_sec*1000000+t2.tv_usec) - (t1.tv_sec*1000000+t1.tv_usec)));
	}
	else
	{
		struct timeval t2;
		printf("parent process created with process ID %d\n", getpid());
		for(int i=0;i<100000000;i++);
		gettimeofday(&t2, NULL);
		printf("Parent process ended with execution time %ld \n", ((t2.tv_sec*1000000+t2.tv_usec) - (t1.tv_sec*1000000+t1.tv_usec)));
		wait(NULL);
	}
	return 0;
}