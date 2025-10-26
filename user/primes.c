#include "kernel/types.h"
#include "user/user.h"

void primes(int) __attribute__((noreturn));

void primes(int fd_in)
{
    int p, n;
    if (read(fd_in, &p, sizeof(int)) == 0)
    {
        close(fd_in);
        exit(0);
    }

    printf("prime %d\n", p);

    int fd[2];
    pipe(fd);

    if (fork() == 0)
    {
        close(fd[1]);
        close(fd_in);
        primes(fd[0]);
    }
    else
    {
        close(fd[0]);

        while (read(fd_in, &n, sizeof(int)))
        {
            if (n % p != 0)
                write(fd[1], &n, sizeof(int));
        }

        close(fd[1]);
        wait(0);
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);

    if (fork() == 0)
    {
        close(fd[1]);
        primes(fd[0]);
        close(fd[0]);
        exit(0);
    }
    else
    {
        close(fd[0]);
        for (int i = 2; i <= 280; i++)
        {
            write(fd[1], &i, sizeof(int));
        }
        close(fd[1]);
        exit(0);
    }

    exit(0);
}