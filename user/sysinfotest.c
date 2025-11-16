#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/sysinfo.h"
#include "user/user.h"


void
sinfo(struct sysinfo *info) {
  if (sysinfo(info) < 0) {
    printf("FAIL: sysinfo failed");
    exit(1);
  }
}

void testcall() {
  struct sysinfo info;
  sinfo(&info);
  printf("sysinfo: freemem=%ld bytes, nproc=%ld processes\n", info.freemem, info.nproc);
}

void testmem() {
  struct sysinfo info;
  sinfo(&info);
  if (info.freemem == 0) {
    printf("FAIL: freemem is zero\n");
    exit(1);
  }
}

void testproc() {
  struct sysinfo info;
  sinfo(&info);
  if (info.nproc == 0) {
    printf("FAIL: nproc is zero\n");
    exit(1);
  }
}

int
main(int argc, char *argv[])
{
  printf("sysinfotest: start\n");
  testcall();
  testmem();
  testproc();
  printf("sysinfotest: OK\n");
  exit(0);
}
