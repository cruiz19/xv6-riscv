#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	if(argc < 2){
    		printf("usage: time1 command [args...]\n");
    		exit(1);
	}
	int start = uptime();
	int pid = fork();

    	if(pid == 0){
           	exec(argv[1], &argv[1]);

     	   	printf("time1: exec failed\n");
       	   	exit(1);
	}

	wait(0);

	int end = uptime();

	int elapsed = end - start;

    	printf("elapsed time: %d ticks\n", elapsed);

    	exit(0);
}
