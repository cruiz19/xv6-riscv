#ifndef _PSTAT_H_
#define _PSTAT_H_

enum procstate { UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };
struct rusage {
  uint cputime;
};
struct pstat {
  int pid;
  int priority;
  enum procstate state;
  uint64 size;
  int ppid;
  char name[16];
  uint readytime; //time process entered RUNNABLE state
};
#endif
