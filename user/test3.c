#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  printf("test3 pid = %d\n", getpid());

  volatile unsigned long i;

  while(1) {
    for(i = 0; i < 10000000; i++);
  }

  exit(0);
}
