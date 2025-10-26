// user/pingpong.c

#include "kernel/types.h"
#include "user/user.h"

static void die(const char *msg) {
  printf("%s\n", msg);
  exit(1);
}

static int xread(int fd, void *buf, int n) {
  int got = 0;
  while (got < n) {
    int r = read(fd, (char*)buf + got, n - got);
    if (r < 0) return -1;       // lỗi I/O
    if (r == 0) break;          // EOF sớm -> lỗi giao thức
    got += r;
  }
  return got;
}

static int xwrite(int fd, const void *buf, int n) {
  int sent = 0;
  while (sent < n) {
    int w = write(fd, (const char*)buf + sent, n - sent);
    if (w <= 0) return -1;      // lỗi I/O
    sent += w;
  }
  return sent;
}

int pingpong(int count) {
  if (count <= 0 || count > 100000) die("invalid count (use 1..100000)");

  int p2c[2], c2p[2];
  if (pipe(p2c) < 0) die("pipe p2c failed");
  if (pipe(c2p) < 0) die("pipe c2p failed");

  int pid = fork();
  if (pid < 0) die("fork failed");

  char byte = 'X';
  char buf;

  if (pid == 0) {
    // --- Child ---
    close(p2c[1]);  // chỉ đọc từ p2c
    close(c2p[0]);  // chỉ ghi vào c2p

    for (int i = 1; i <= count; i++) {
      if (xread(p2c[0], &buf, 1) != 1) die("child read failed");
      if (count == 1)
        printf("%d: received ping\n", getpid());
      else
        printf("%d: received ping %d/%d\n", getpid(), i, count);

      if (xwrite(c2p[1], &buf, 1) != 1) die("child write failed");
    }

    close(p2c[0]);
    close(c2p[1]);
    exit(0);
  } else {
    // --- Parent ---
    close(p2c[0]);  // chỉ ghi vào p2c
    close(c2p[1]);  // chỉ đọc từ c2p

    for (int i = 1; i <= count; i++) {
      if (xwrite(p2c[1], &byte, 1) != 1) die("parent write failed");
      if (xread(c2p[0], &buf, 1) != 1) die("parent read failed");

      if (count == 1)
        printf("%d: received pong\n", getpid());
      else
        printf("%d: received pong %d/%d\n", getpid(), i, count);
    }

    close(p2c[1]);
    close(c2p[0]);
    wait(0);
    return 0;
  }
}

#ifndef PINGPONG_LIBRARY
int main(int argc, char *argv[]) {
  int count = 1;
  if (argc >= 2) {
    count = atoi(argv[1]);
  }
  pingpong(count);
  return 0;
}
#endif