#include "kernel/types.h"
#include "user/user.h"

void
run_test(char *name, int priority, int fd)
{
  char start;

  setpriority(priority);

  // Wait until the parent tells us to start.
  read(fd, &start, 1);

  printf("%s started with priority %d\n", name, getpriority());

  for (volatile int i = 0; i < 50000000; i++)
    ;

  printf("%s finished\n", name);
  exit(0);
}

int
main(void)
{
  int p[2];
  int pid;

  pipe(p);

  pid = fork();
  if (pid == 0)
    run_test("LOW", 1, p[0]);

  pid = fork();
  if (pid == 0)
    run_test("MEDIUM", 5, p[0]);

  pid = fork();
  if (pid == 0)
    run_test("HIGH", 10, p[0]);

  // Release all three children.
  write(p[1], "xxx", 3);

  wait(0);
  wait(0);
  wait(0);

  exit(0);
}
