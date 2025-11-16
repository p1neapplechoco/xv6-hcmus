#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  // Kiểm tra xem có đủ đối số không
  if(argc < 3){
    printf("Usage: trace <mask> <command>\n");
    exit(1);
  }
  
  // Gọi system call trace với đối số thứ hai (argv[1])
  if (trace(atoi(argv[1])) < 0) {
    printf("trace: trace system call failed\n");
    exit(1);
  }
  
  // DÒNG NÀY LÀ QUAN TRỌNG NHẤT
  // Thực thi lệnh (argv[2]) với mảng đối số bắt đầu từ argv[2]
  exec(argv[2], &argv[2]);
  
  // Nếu exec quay trở lại, nó đã thất bại. In lỗi.
  printf("trace: exec %s failed\n", argv[2]);
  exit(1);
}
