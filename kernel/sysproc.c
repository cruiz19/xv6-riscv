#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"
#include "pstat.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_wait2(void)
{
  uint64 p;
  uint64 r;

  argaddr(0, &p);
  argaddr(1, &r);

  return kwait2(p, r);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if (t == SBRK_EAGER || n < 0) {
    if (growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if (addr + n < addr)
      return -1;
    if (addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if (n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (killed(myproc())) {
      release(&tickslock);
      return -1;
    }
    sleep_prepare(&ticks);
    release(&tickslock);
    sleep();
    acquire(&tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


//add this function to create the array in kernel space and then copyout the results
//because user is not allowed to pass user-space pointer directly to the kernel function

uint64
sys_getprocs(void)
{
	uint64 addr; //addr of stats[]
	struct pstat pstats[NPROC]; //creates temp array inside kernel

	argaddr(0, &addr); //first arg passed to system call when getprocs(stats) is called

	int count = getprocs(pstats); //fills array and returns how many processes found
	if(copyout(myproc()-> pagetable, myproc()->sz,  addr, (char *)pstats, count * sizeof(struct pstat)) < 0)
		return -1; //copyout copies data from kernel memory to user memory return -1 if copyout fails
	return count; //return number of processes back to ps
}

uint64
sys_getpriority(void)
{
	return getpriority(); //calls kernel function
}

uint64
sys_setpriority(void)
{
	int priority;
	argint(0, &priority); //retrieve priority from the syscall argument - 0 used for first argument
	return setpriority(priority); //call to kernel function
}
