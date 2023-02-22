#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc, char* argv[]) {
  int ret;
  int fd;  //文件描述符
  char* filename;

  if (argc < 2) {
    printf("参数较少\r\n");
  }
  filename = argv[1];
  fd = open(filename, O_RDWR);
  if (fd < 0) {
    printf("Can't open file %s\r\n", filename);
    return -1;
  }
  ret = write(fd, argv[2], sizeof(argv[2]));
  if (ret < 0) {
    printf("LED control failure!\r\n");
    close(fd);
    return -1;
  }

  ret = close(fd);
  if (ret < 0) {
    printf("file %s close failed!\r\n", argv[1]);
    return -1;
  }
  return 0;
}