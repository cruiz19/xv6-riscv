#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
	int priority;

	priority = getpriority();
	printf("Initial priority: %d\n", priority);

	setpriority(5);

	priority = getpriority();
	printf("New priority: %d\n", priority);

	printf("Press enter to exit...\n");

	char buf[10];
	read(0, buf, sizeof(buf));

	exit(0);
}
