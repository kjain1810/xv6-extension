# Extending the xv6 operating system

## int waitx(int\*, int\*) - System call

The system call is defined as ```int waitx(int* wtime, int* rtime)```. This function works the exact same way as the wait system call works, the only difference being that the ```rtime``` of the process is being assigned to the pointer passed and the total waiting time ```wtime``` is being calculated. These can be seen as: 
```c
*rtime = p->rtime;
*wtime = p->etime - p->ctime - p->rtime;
```
This also assigned the ```etime``` of the process as the process is finishing. The same change has been made in ```wait``` as well.

## time - User program

This program tells us the running time and waiting time of the process passed as the argument to this process. The program has been made to check if the ```waitx``` system call is working properly or not.
The program takes 1 input: the process you want to test the time of. E.g. ```time ls``` would tell us the running and waiting time of running the ```ls``` user program.
The program creates a child process and runs the argument program there. If no argument is provided, the programs runs a default process as: 

```c
volatile int i = 1;
int x;
for(x = 0; x < 100000000; x++) {
    i++;
    if(x % 2 == 0)
        i /= 2;
}
```

The parent process uses the ```waitx``` system call to wait for the child to end and then reports its ```rtime``` and ```wtime```.

The output format of the process has been slightly altered for better aesthetics. Sample output with MLFQ scheduler: 

```
PID: 1
Priority: 60
State: SLEEPING
r_time: 8
w_time: 0
n_run: 26
curq: 3
qtime: [2,2,4,0,0,]


PID: 2
Priority: 60
State: SLEEPING
r_time: 3
w_time: 0
n_run: 17
curq: 1
qtime: [1,2,0,0,0,]


PID: 3
Priority: 60
State: RUNNING
r_time: 9
w_time: 0
n_run: 10
curq: 3
qtime: [1,2,4,4,0,]
```

##  New schedulers

The ```SCHEDULER``` flag has been passed into the Makefile with the default value of RR. The ```SCHED``` macro in the code has then been set using the ```-D``` flag. Thereafter, the ```SCHED``` macro has been used throughout the code to find out which scheduler is in use. If no ```SCHEDULER``` flag has been passed, then the default Round Robin scheduler will be used as the default value.

## First Come First Serve Scheduler

To implement FCFS scheduling, we first need to find the process with the minimum runnable arrival time, ie, with the minimum value of ```ctime``` and state ```RUNNABLE```. We do this by a simple loop through the process table:

```c
uint minimum = ticks + 1000;
// uint minstart = minimum;
for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
{
    if(p->state != RUNNABLE)
    continue;
    if(p->ctime < minimum)
    minimum = p->ctime;
}
```

After finding the minimum such ```ctime```, we find the process and switch the context.
To make the scheduler non-preemptive, we need the process to not yield the system resources it is using after a single tick but only when its state changes. This can be done by a simple check in ```trap.c``` file. There, we were calling the ```yield()``` function if our process was in ```RUNNING``` state and we got caught in the time trap. However, we now put a simple check that we do this only when our scheduler is not FCFS:

```c
if(SCHED[0] != 'F') // IF NOT FCFS
     yield();
```

## Priority Based Scheduler

For this type of scheduler, we first need to find the process with the highest priority, meaning we need to find the process that is state ```RUNNABLE``` with the lowest ```priority```. We do this as:

```c
uint minimum = 101;
for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
{
    if(p->state != RUNNABLE)
    	continue;
    if(p->priority < minimum)
    	minimum = p->priority;
}
```

We then go into Round Robin selection of the processes with this priority. During the Round Robin scheduling, we need to check, at each iteration, that there is no new process that is in state ```RUNNABLE``` and has a ```priority``` lower than the ones that are in Round Robin right now. We do this as: 

```c
struct proc *q;
int newmin = 0;
for(q = ptable.proc; q < &ptable.proc[NPROC]; q++)
    if(q->state == RUNNABLE && q->priority < minimum)
        newmin = 1;
if(newmin)
    break;
```

### int set_priority(int, int) - System call

This system call is defined as ```int set_priority(int new_priority, int pid)```. The call first acquires the process table mutex lock. It then finds the process whose pid is the same as ```pid```. If no such process is found, -1 is returned. If found, the priority of the process is changed to ```new_priority``` and if this new priority is lower than the previous priority of the process, the current process yields and the scheduler is called again.

### setPriority -  User program

This user program takes 2 arguments: ```new_priority``` and ```pid```. It then sets the priority of the process with pid ```pid``` to ```new_priority```. Errors such as no process with pid specified exists, invalid pids or invalid priorities are handled.

## Multi-level Feedback Queue Scheduler

First, we need a queue data structure to make 5 queues. Since the maximum size of the queue is limited to ```NPROC```, we can implement a simple array queue as: 

```c
struct proc *queue[5][NPROC];
int en[5] = {0, 0, 0, 0, 0};
int allowed[5] = {1, 2, 4, 8, 16};
int insertqueue(int i, struct proc *p)
{
  // cprintf("INSERTING ID: %d\n", p->pid);
  for(int a = 0; a < en[i]; a++)
    if(queue[i][a]->pid == p->pid)
      return 0;
  p->lupdate = ticks;
  p->curq = i;
  queue[i][en[i]++] = p;
  return 1;
}
int removequeue(int i, struct proc *p)
{
  int idx = -1;
  // cprintf("REMOVING ID: %d\n", p->pid);
  for (int a = 0; a < en[i]; a++)
    if(queue[i][a]->pid == p->pid)
    {
      idx = a;
      break;
    }
  if(idx == -1)
    return 0;
  for(int a = idx; a < en[i]; a++)
    queue[i][a] = queue[i][a + 1];
  en[i]--;
  return 1;
}
```

Here, allowed is the array of maximum ticks a process is allowed to run as part of the that queue.

Before starting to find the next process to run, we first need to check for aging. We can calculate the time spent in the queue with the help of ```lupdate``` of the process as:

```c
for(int a = 1; a < 5; a++)
    for(int b = 0; b < en[a]; b++)
    {
        if(ticks - queue[a][b]->lupdate > 30)
        {
            struct proc* p = queue[a][b];
            removequeue(a, p);
            insertqueue(a - 1, p);
            // b--;
        }
    }
```

Next, we go through the 5 queues and find the first one which has a state ```RUNNABLE``` process. 
To allow the process to run for the allowed clock ticks, we make changes to ```trap.c``` similar to those made to implement the FCFS Scheduler: 

```c
if(SCHED[0] == 'M') // IF MLFQ
{
    if(myproc()->qtime[myproc()->curq] >= allowed[myproc()->curq])
    {
        updatequeue(myproc());
        yield();
    }      
}
```

Using this, the process yields only when it has used the allowed clock ticks and not before. Also, after using the allowed time, the process will get shifted to the next queue and we mark it such using the ```updatequeue``` function implemented in ```proc.c```.  After the process has been run, we check if the process is still in state ```RUNNABLE```. If it is, we will update and insert into the appropriate queue as required.

### Possible exploitation

Since a process leaves the queuing system when it voluntarily leaves relinquishes control of the CPU and is inserted back when it is ready again, a process could, theoretically, enter into queue 1, run for 1 clock tick, relinquish, then immediately become available again and run for just 1 clock tick again. With this, the process will never get demoted from queue 1. Although the time slice it will get will be lower, but if the number of process in queue 2, 3 and 4 is large, this will allow the process to prioritise its execution by a large factor and exploit the system.

## Bonus

![plot_10_procs_20_aging](/home/kunal/Desktop/padhai/xv6-extension/plot_10_procs_20_aging.png)

This is the plot of the 10 processes and their queues as the execution progresses. We using age limit in a certain queue here as 20.

![plot_10_procs_30_aging](/home/kunal/Desktop/padhai/xv6-extension/plot_10_procs_30_aging.png)

This is the plot of the 10 processes and their queue with age limit as 30.

As is evident, with age limit 20, the processes shift between queues much more frequently.

Note: the processes go on for much longer but since there isn't a lot of activity in that part of the processes, I cropped to make this part clearer.