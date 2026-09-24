#include "kernel/types.h"
#include "user/user.h"


//
int
main(int argc, char *argv[])
{
	if (argc < 2){
		printf("Usage: prtest <name>\n");
		exit(1);
	}

	//assures that pexec successfully assigned/inherited the requested priority
	printf("%s started with priority %d\n", argv[1], getpriority());
	//gives some CPU work to do
	for (volatile int i = 0; i < 50000000; i++);
	printf("%s finished\n", argv[1]);

	exit(0);
}
