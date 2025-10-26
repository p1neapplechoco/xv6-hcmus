#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    printf("sleeptest: start\n");
    sleep(100);
    printf("sleeptest: end\n");
    exit(0);
}