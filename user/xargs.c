#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    char buf[512];
    char *args[MAXARG];
    int cnt_args = 0; // count for args
    int i = 1;

    if (argc > 2 && strcmp(argv[1], "-n") == 0) {
        i = 3;
    }
    while (i < argc) {
        args[cnt_args++] = argv[i++];
    }

    int n;
    while ((n = read(0, buf, sizeof(buf))) > 0) {
        int head = 0;
        for (int j = 0; j < n; j++) {
            if (buf[j] == '\n') {
                buf[j] = 0;

                args[cnt_args] = buf + head; // move pointer to offset head
                args[cnt_args + 1] = 0;

                if (fork() == 0) {
                    exec(args[0], args);
                    exit(1); // error
                }
                wait((int*)0);

                head = j + 1;
            }
        }
    }
    
    exit(0);
}