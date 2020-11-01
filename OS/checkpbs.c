#include "types.h"
#include "mmu.h"
#include "param.h"
#include "proc.h"
#include "user.h"
#include "fcntl.h"

int main()
{
    int pid1 = fork();
    if(pid1 == 0)
    {
        for(int a = 0; a < 1000; a++)
        {
            printf(1, "Inside pid1\n");
        }
        exit();
    }
    set_priority(80, pid1);
    sleep(10);
    int pid2 = fork();
    if(pid2 == 0)
    {
        for(int a = 0; a < 1000; a++)
        {
            printf(1, "Inside pid2\n");
        }
        exit();
    }
    set_priority(75, pid2);
    sleep(10);
    set_priority(70, pid2);
    sleep(10);
    set_priority(65, pid1);
    wait();
    wait();
    exit();
}