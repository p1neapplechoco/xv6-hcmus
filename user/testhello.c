#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: hello <name>\n");
        exit(1);
    }
    hello(argv[1]);
    exit(0);
}