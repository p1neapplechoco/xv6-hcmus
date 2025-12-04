// user/pgacc.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PGSIZE 4096

static void
printmask(uint32 m, int npages)
{
  printf("mask=0x%x bits=", m);
  for (int i = npages - 1; i >= 0; i--) {
    printf("%d", (m >> i) & 1);
  }
  printf("\n");
}

int
main(void)
{
  const int np = 8;              
  uint32 mask = 0;

  char *raw = sbrk(PGSIZE * (np + 1));
  if (raw == (char*)-1) {
    fprintf(2, "sbrk failed\n");
    exit(1);
  }

  uint64 r = (uint64)raw;
  char *buf = (char*)((r + PGSIZE - 1) & ~(uint64)(PGSIZE - 1));

  volatile char sink = 0;

  buf[0] = 1;                 // page 0 (write)
  sink = buf[2 * PGSIZE];     // page 2 (read)
  buf[5 * PGSIZE] = 7;        // page 5 (write)

  if (pgaccess(buf, np, &mask) < 0) {
    fprintf(2, "pgaccess failed\n");
    exit(1);
  }

  printf("Expect bits: page0,page2,page5 => %s\n", "0x25 (binary 00100101)");
  printmask(mask, np);

  mask = 0;
  if (pgaccess(buf, np, &mask) < 0) {
    fprintf(2, "pgaccess failed (2)\n");
    exit(1);
  }
  printf("Expect mask=0 after clear\n");
  printmask(mask, np);

  buf[1 * PGSIZE] = 9;
  sink = buf[7 * PGSIZE];

  mask = 0;
  if (pgaccess(buf, np, &mask) < 0) {
    fprintf(2, "pgaccess failed (3)\n");
    exit(1);
  }
  printf("Expect bits: page1,page7 => %s\n", "0x82 (binary 10000010)");
  printmask(mask, np);

  if (sink == 123) printf("wow\n");

  printf("pgacc done\n");
  exit(0);
}
