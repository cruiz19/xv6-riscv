#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  struct pstat uproc[NPROC];
  int nprocs;
  int i;
  int currenttime;
  char *state;

  static char *states[] = {
    [SLEEPING]  "sleeping",
    [RUNNABLE]  "runnable",
    [RUNNING]   "running ",
    [ZOMBIE]    "zombie  "
  };

  nprocs = getprocs(uproc);
  if (nprocs < 0)
    exit(-1);

  // Get the current number of clock ticks.
  currenttime = uptime();

  printf("pid\tpriority\tstate\t\tsize\tppid\tage\tname\n");

  for (i = 0; i < nprocs; i++) {
    state = states[uproc[i].state];

    // Age only applies while a process is waiting in the ready queue.
    if (uproc[i].state == RUNNABLE) {
      printf("%d\t%d\t\t%s\t%lu\t%d\t%u\t%s\n",
             uproc[i].pid,
             uproc[i].priority,
             state,
             uproc[i].size,
             uproc[i].ppid,
             currenttime - uproc[i].readytime,
             uproc[i].name);
    } else {
      printf("%d\t%d\t\t%s\t%lu\t%d\t\t%s\n",
             uproc[i].pid,
             uproc[i].priority,
             state,
             uproc[i].size,
             uproc[i].ppid,
             uproc[i].name);
    }
  }

  exit(0);
}
