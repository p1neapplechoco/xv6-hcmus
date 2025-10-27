#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// Hàm này sẽ giúp chúng ta lấy ra tên tệp từ một đường dẫn đầy đủ.
// Ví dụ: từ "a/b/c.txt" sẽ trả về "c.txt".
char* fmtname(char *path) {
  static char buf[DIRSIZ+1];
  char *p;

  // Tìm ký tự '/' cuối cùng trong đường dẫn.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Sao chép tên tệp vào buffer.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  buf[strlen(p)] = 0;
  return buf;
}

// Hàm tìm kiếm đệ quy.
void find(char *path, char *filename) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  // Mở tệp hoặc thư mục.
  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  // Lấy thông tin (stat) của tệp/thư mục.
  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type) {
  // Trường hợp 1: Nếu 'path' là một TỆP.
  case T_FILE:
    // So sánh tên tệp (không bao gồm đường dẫn) với tên tệp cần tìm.
    // strcmp trả về 0 nếu hai chuỗi giống hệt nhau.
    if (strcmp(fmtname(path), filename) == 0) {
      // Nếu khớp, in ra toàn bộ đường dẫn.
      printf("%s\n", path);
    }
    break;

  // Trường hợp 2: Nếu 'path' là một THƯ MỤC.
  case T_DIR:
    // Kiểm tra để đảm bảo đường dẫn không quá dài, tránh tràn bộ đệm.
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
      printf("find: path too long\n");
      break;
    }
    // Sao chép đường dẫn hiện tại vào buffer.
    strcpy(buf, path);
    p = buf + strlen(buf);
    // Thêm dấu '/' vào cuối đường dẫn để chuẩn bị nối tên mục con.
    *p++ = '/';

    // Đọc từng mục (entry) trong thư mục.
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      // Bỏ qua các mục không hợp lệ.
      if (de.inum == 0)
        continue;
      
      // Quan trọng: Bỏ qua thư mục '.' và '..' để tránh đệ quy vô hạn.
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;

      // Nối tên mục con vào đường dẫn trong buffer.
      memmove(p, de.name, DIRSIZ);
      // Đảm bảo chuỗi kết thúc bằng null.
      p[DIRSIZ] = 0;

      // Gọi đệ quy hàm find với đường dẫn mới vừa được tạo.
      find(buf, filename);
    }
    break;
  }
  // Đóng file descriptor sau khi đã xử lý xong.
  close(fd);
}

// Hàm main, điểm vào của chương trình.
int main(int argc, char *argv[]) {
  // Kiểm tra số lượng đối số dòng lệnh.
  if (argc != 3) {
    fprintf(2, "Usage: find <directory> <filename>\n");
    exit(1);
  }

  // Gọi hàm find với đường dẫn và tên tệp từ đối số.
  find(argv[1], argv[2]);

  exit(0);
}
