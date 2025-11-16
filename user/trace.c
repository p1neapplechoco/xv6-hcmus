#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) // Input: (mask, command)
{
  // Check xem có đủ đối số không
  if(argc < 3){
    printf("Usage: trace <mask> <command>\n");
    exit(1);
  }
  
  // Gọi system call trace với đối số thứ hai (argv[1])
  if (trace(atoi(argv[1])) < 0) {
    printf("trace: trace system call failed\n");
    exit(1);
  }
  
  // Thực thi lệnh (argv[2]) với mảng đối số bắt đầu từ argv[2]
  exec(argv[2], &argv[2]);
  
  // Nếu exec quay lại, nó đã fail, lỗi.
  printf("trace: exec %s failed\n", argv[2]);
  exit(1);
}
