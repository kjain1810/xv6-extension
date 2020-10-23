#include "types.h"
#include "mmu.h"
#include "param.h"
#include "proc.h"
#include "user.h"

int main(int argc, char *argv[]) {
    if(argc != 1)
    {
        printf(1, "No arguments required!\n");
        exit();
    }
    int pid = fork();
    int rtime, wtime;
    if(pid == 0) {
        int i;
        volatile int x;
        for(i = 0; i < 1000000000; i++) {
            x++;
            x /= 2;
        }
        exit();
    }
    if(pid == 1) {
        printf(1, "Fork failed!\n");
        exit();
    }
    waitx(&wtime, &rtime);
    printf(1, "Running time: %d ticks\n", rtime);
    printf(1, "Wait time: %d ticks\n", wtime);
    exit();
}