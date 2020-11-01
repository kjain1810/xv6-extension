
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
      for (volatile int k = 0; k < 20; k++)
      {
          volatile int x = 0;
          for (i = 0; i < 10000000; i++)
          {
            x++; //cpu time
          }
      }
      exit();
    }
  }
  for (j = 0; j < number_of_processes; j++)
  {
    wait();
  }
  exit();
}
