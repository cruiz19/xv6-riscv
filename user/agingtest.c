#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  int start[2];
  int ready[2];
  int pid;
  char c;

  pipe(start);
  pipe(ready);

  printf("Starting aging test...\n");

  // HIGH-priority child
  pid = fork();

  if (pid == 0) {
    setpriority(10);

    // Tell parent HIGH is ready.
    write(ready[1], "H", 1);

    // Wait for parent to release us.
    read(start[0], &c, 1);

    printf("HIGH started with priority %d\n", getpriority());

    for (int j = 0; j < 20; j++){
	for(volatile int i = 0; i < 100000000; i++)
	  ;
    }

    printf("HIGH finished\n");
    exit(0);
  }

  // LOW-priority child
  pid = fork();

  if (pid == 0) {
    setpriority(1);

    // Tell parent LOW is ready.
    write(ready[1], "L", 1);

    // Wait for parent to release us.
    read(start[0], &c, 1);

    printf("LOW started with priority %d\n", getpriority());

    for (volatile int i = 0; i < 10000000; i++)
      ;

    printf("LOW finished\n");
    exit(0);
  }

  // Wait until BOTH children have set their priorities
  // and reached the starting gate.
  read(ready[0], &c, 1);
  read(ready[0], &c, 1);

  printf("Both processes ready. Starting test.\n");

  // Release both children.
  write(start[1], "XX", 2);

  wait(0);
  wait(0);

  printf("Aging test complete.\n");

  exit(0);
}
