#include "types.h"
#include "mmu.h"
#include "param.h"
#include "proc.h"
#include "user.h"
#include "fcntl.h"

int main() {
    printf(1, "Inside ps\n");
    int ret = getallproc();
    if(ret != 1)
        printf(1, "Sorry, error in ps :(\n");
    exit();
}