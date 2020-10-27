
#include "types.h"
#include "user.h"

int number_of_processes = 10;

int main(int argc, char *argv[])
{
  int j;
  for (j = 0; j < number_of_processes; j++)
  {
    int pid = fork();
    if (pid < 0)
    {
      printf(1, "Fork failed\n");
      continue;
    }
    if (pid == 0)
    {
      volatile int i;
      for (volatile int k = 0; k < number_of_processes; k++)
      {
        if (k <= j)
        {
          sleep(10); //io time
        }
        else
        {
          volatile int x = 0;
          for (i = 0; i < 1000; i++)
          {
            x++; //cpu time
          }
        }
      }
    //   printf(1, "Process: %d Finished\n", j);
      exit();
    }
    else{
        for (int i = 0; i < 1000000000; i++)
          {
            for(int a = 0; a < 1000; a++)
            {
              ;
            }
          }
        int wtime, rtime;
        int ret = waitx(&wtime, &rtime);
        printf(1, "%d %d %d\n", ret, wtime, rtime);   
    //   set_priority(100-(20+j),pid); // will only matter for PBS, comment it out if not implemented yet (better priorty for more IO intensive jobs)
    }
  }
  for (j = 0; j < number_of_processes+5; j++)
  {
    
  }
  exit();
}
