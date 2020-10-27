#include "types.h"
#include "mmu.h"
#include "param.h"
#include "proc.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf(1, "Exactly 3 arguments required!\n");
        exit();
    }
    int new_priority = 0;
    for (int a = 0; argv[1][a] != '\0'; a++)
    {
        if (argv[1][a] < 48 || argv[1][a] > 57)
        {
            printf(1, "Invalid argument! Priority is set to a non negative number.\n");
            exit();
        }
        new_priority *= 10;
        new_priority += (int)((int)argv[1][a] - (int)'0');
    }
    int pid = 0;
    for (int a = 0; argv[2][a] != '\0'; a++)
    {
        if (argv[2][a] < 48 || argv[2][a] > 57)
        {
            printf(1, "Invalid argument! pid is a non negative number.\n");
            exit();
        }
        pid *= 10;
        pid += (int)((int)argv[2][a] - (int)'0');
    }
    if(new_priority > 100)
    {
        printf(1, "Invalid priority, should be in range [0, 100]\n");
        exit();
    }
    int ret = set_priority(new_priority, pid);
    if(ret < 0)
    {
        printf(1, "No process corrensponding to the pid!\n");
        exit();
    }
    printf(1, "pid: %d\nold_priority: %d\nnew_priority: %d\n", pid, ret, new_priority);
    exit();
}