#include "types.h"
#include "mmu.h"
#include "param.h"
#include "proc.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]) {
    int pid = fork();
    int rtime, wtime;
    if(pid == 0) {
        if (argc == 1)
        {
            printf(1, "Arguments required!\nUsing default process\n");
            volatile int i = 1;
            int x;
            for(x = 0; x < 100000000; x++) {
                i++;
                if(x % 2 == 0)
                    i /= 2;
            }
            exit();
        }
        exec(argv[1], argv + 1);
        exit();
    }
    if(pid == -1) {
        printf(1, "Fork failed!\n");
        exit();
    }
    waitx(&wtime, &rtime);
    printf(1, "Running time: %d ticks\n", rtime);
    printf(1, "Wait time: %d ticks\n", wtime);
    exit();
}