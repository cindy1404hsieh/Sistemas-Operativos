#include <inc/assert.h>
#include <inc/x86.h>
#include <kern/spinlock.h>
#include <kern/env.h>
#include <kern/pmap.h>
#include <kern/monitor.h>

int cycles = 0;
int total_cycles = 0;
int numbers_sched_calls = 0;
int total_resets = 0;

void sched_halt(void);
void priority_reset(void);
void print_stats(void);
void env_run_by_priority(int init, int end, int priority);

// Choose a user environment to run and run it.
void
sched_yield(void)
{
	// Implement simple round-robin scheduling.
	//
	// Search through 'envs' for an ENV_RUNNABLE environment in
	// circular fashion starting just after the env this CPU was
	// last running.  Switch to the first such environment found.
	//
	// If no envs are runnable, but the environment previously
	// running on this CPU is still ENV_RUNNING, it's okay to
	// choose that environment.
	//
	// Never choose an environment that's currently running on
	// another CPU (env_status == ENV_RUNNING). If there are
	// no runnable environments, simply drop through to the code
	// below to halt the cpu.

	// Your code here
	numbers_sched_calls++;

	int pos_curenv = 0;

	if (curenv) {
		pos_curenv = ENVX(curenv->env_id);
	}

	//----------------- Scheduler Round Robin ------------- //

#ifdef SCHED_RR

	for (int i = 1; i <= NENV; i++) {
		int j = (pos_curenv + i) % NENV;
		if (envs[j].env_status == ENV_RUNNABLE) {
			env_run(&envs[j]);
			return;
		}
	}

#endif

	//----------------- Scheduler with Priority ------------- //

#ifdef SCHED_PR

	int priority = MAX_PRIORITY;

	if (cycles == CYCLES_FOR_PRIORITY_RESET)
		priority_reset();

	while (priority <= MIN_PRIORITY) {
		// the search for the current position begins
		env_run_by_priority(pos_curenv, NENV, priority);

		// the search for the initial post continues
		env_run_by_priority(0, pos_curenv, priority);

		priority++;
	}

#endif

	// Wihtout scheduler, keep runing the last environment while it exists
	if (curenv && curenv->env_status == ENV_RUNNING) {
		env_run(curenv);
	}

	// sched_halt never returns
	sched_halt();
}

// Halt this CPU when there is nothing to do. Wait until the
// timer interrupt wakes it up. This function never returns.
//
void
sched_halt(void)
{
	int i;

	// For debugging and testing purposes, if there are no runnable
	// environments in the system, then drop into the kernel monitor.
	for (i = 0; i < NENV; i++) {
		if ((envs[i].env_status == ENV_RUNNABLE ||
		     envs[i].env_status == ENV_RUNNING ||
		     envs[i].env_status == ENV_DYING))
			break;
	}
	if (i == NENV) {
		cprintf("No runnable environments in the system!\n");
		print_stats();
		while (1)
			monitor(NULL);
	}

	// Mark that no environment is running on this CPU
	curenv = NULL;
	lcr3(PADDR(kern_pgdir));

	// Mark that this CPU is in the HALT state, so that when
	// timer interupts come in, we know we should re-acquire the
	// big kernel lock
	xchg(&thiscpu->cpu_status, CPU_HALTED);

	// Release the big kernel lock as if we were "leaving" the kernel
	unlock_kernel();

	// Once the scheduler has finishied it's work, print statistics on
	// performance. Your code here
	// Obs: if my code is here doesn't print anything after process calls.

	// Reset stack pointer, enable interrupts and then halt.
	asm volatile("movl $0, %%ebp\n"
	             "movl %0, %%esp\n"
	             "pushl $0\n"
	             "pushl $0\n"
	             "sti\n"
	             "1:\n"
	             "hlt\n"
	             "jmp 1b\n"
	             :
	             : "a"(thiscpu->cpu_ts.ts_esp0));
}

void
priority_reset(void)
{
	cycles = 0;
	for (int i = 0; i < NENV; i++) {
		envs[i].priority = MAX_PRIORITY;
	}
	total_resets++;
}

/* Looks for the following ENV_RUNNABLE with
   priority less than or equal to the one passed as parameter
   and executes it.
*/
void
env_run_by_priority(int init, int end, int priority)
{
	int pos_curenv = init;
	for (pos_curenv; pos_curenv < end; pos_curenv++) {
		if (envs[pos_curenv].env_status == ENV_RUNNABLE &&
		    envs[pos_curenv].priority <= priority) {
			cycles++;
			envs[pos_curenv].priority++;
			total_cycles++;

			env_run(&envs[pos_curenv]);
		}
	}
}

/* Stats function
 */

void
print_stats(void)
{
	cprintf("*******************************\n");
	cprintf("\tScheduler stats\n");
	cprintf("*******************************\n");

	cprintf("Total of sched_yield calls: %d\n", numbers_sched_calls);

#ifdef SCHED_PR

	cprintf("Total cycles: %d\n", total_cycles);

	cprintf("Total resets: %d\n", total_resets);

#endif
}