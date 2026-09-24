#include "kernel/types.h"
#include "user/user.h"

#define N 3

int
main(void)
{
  int start[N][2];
  int results[N][2];
  int priorities[N] = {1, 5, 10};
  int pid;

  for (int i = 0; i < N; i++) {
    pipe(start[i]);
    pipe(results[i]);

    pid = fork();

    if (pid == 0) {
      char c;

      setpriority(priorities[i]);

      // Wait until parent releases us.
      read(start[i][0], &c, 1);

      // This is the first time we run after being released.
      int first_run = uptime();

      // Send first-run time to parent.
      write(results[i][1], &first_run, sizeof(first_run));

      // CPU-intensive work.
      for (volatile int j = 0; j < 50000000; j++)
        ;

      int finish = uptime();

      // Send finish time to parent.
      write(results[i][1], &finish, sizeof(finish));

      exit(0);
    }
  }

  int begin = uptime();

  // Release all three processes.
  for (int i = 0; i < N; i++)
    write(start[i][1], "X", 1);

  int total_response = 0;
  int total_turnaround = 0;

  for (int i = 0; i < N; i++) {
    int first_run;
    int finish;

    read(results[i][0], &first_run, sizeof(first_run));
    read(results[i][0], &finish, sizeof(finish));

    int response = first_run - begin;
    int turnaround = finish - begin;

    printf("Priority %d: response = %d, turnaround = %d\n",
           priorities[i], response, turnaround);

    total_response += response;
    total_turnaround += turnaround;
  }

  for (int i = 0; i < N; i++)
    wait(0);

  printf("Average response time: %d ticks\n",
         total_response / N);

  printf("Average turnaround time: %d ticks\n",
         total_turnaround / N);

  exit(0);
}
