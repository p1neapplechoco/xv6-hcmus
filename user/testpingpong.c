#define PINGPONG_LIBRARY
#include "kernel/types.h"
#include "user/user.h"
#include "pingpong.c"

int main()
{
    pingpong(36);
    exit(0);
}