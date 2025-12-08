#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void test_correctness(void)
{
    printf("=== TEST 1: Correctness ===\n");
    int pid1 = getpid();
    int pid2 = ugetpid();
    printf("getpid()=%d, ugetpid()=%d\n", pid1, pid2);
    if (pid1 == pid2)
        printf("PASS\n\n");
    else
        printf("FAIL\n\n");
}

void test_consistency(void)
{
    printf("=== TEST 2: Consistency ===\n");
    int pid1 = ugetpid();
    int ok = 1;
    for (int i = 0; i < 1000; i++)
    {
        if (ugetpid() != pid1)
        {
            ok = 0;
            break;
        }
    }
    if (ok)
        printf("PASS\n\n");
    else
        printf("FAIL\n\n");
}

void test_fork(void)
{
    printf("=== TEST 3: Fork ===\n");
    int pid = fork();
    if (pid == 0)
    {
        printf("Child: getpid()=%d, ugetpid()=%d\n", getpid(), ugetpid());
        exit(0);
    }
    else
    {
        printf("Parent: getpid()=%d, ugetpid()=%d\n", getpid(), ugetpid());
        wait(0);
        printf("PASS\n\n");
    }
}

void benchmark(void)
{
    printf("=== TEST 4: Performance ===\n");
    int count = 100000;
    volatile int pid;

    uint64 start = uptime();
    for (int i = 0; i < count; i++)
        pid = getpid();
    uint64 t1 = uptime() - start;
    printf("getpid(): %ld ticks\n", t1);

    start = uptime();
    for (int i = 0; i < count; i++)
        pid = ugetpid();
    uint64 t2 = uptime() - start;
    printf("ugetpid(): %ld ticks\n", t2);

    if (pid < 0)
    {
        printf("Error\n");
    }

    if (t2 > 0)
    {
        int speedup = (t1 * 100) / t2;
        printf("Speedup: %d.%02dx\n\n", speedup / 100, speedup % 100);
    }
}

int main(int argc, char *argv[])
{
    printf("\n=== USYSCALL Test ===\n\n");
    test_correctness();
    test_consistency();
    test_fork();
    benchmark();
    printf("=== All Tests Done ===\n\n");
    exit(0);
}
