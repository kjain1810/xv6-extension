#include "types.h"
#include "mmu.h"
#include "param.h"
#include "proc.h"
#include "user.h"
#include "fcntl.h"

int main(int agrc, char *agrv[])
{
    for (int a = 0; a < 20; a++)
    {
        int pid = fork();
        if (pid == 0)
        {
            volatile int i;
            for (i = 0; i < 10000000; i++)
                ;
            for (i = 0; i < 10000000; i++)
                ;
            for (i = 0; i < 10000000; i++)
                ;
            for (i = 0; i < 10000000; i++)
                ;
            for (i = 0; i < 10000000; i++)
                ;
            for (i = 0; i < 10000000; i++)
                ;
            for (i = 0; i < 10000000; i++)
                ;
            for (i = 0; i < 10000000; i++)
                ;
            for (i = 0; i < 10000000; i++)
                ;
            for (i = 0; i < 10000000; i++)
                ;
            for (i = 0; i < 10000000; i++)
                ;
            exit();
        }
    }
    int sltime = 0;
    for (int a = 0; agrv[1][a] != '\0'; a++)
        sltime = sltime * 10 + (int)(agrv[1][a] - '0');
    sleep(sltime);
    getallproc();
    for (int a = 0; a < 20; a++)
        wait();
    exit();
}