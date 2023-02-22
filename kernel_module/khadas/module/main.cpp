
#include "stdio.h"
// #include <vector>
// #include <iostream>
// #include "Slam_/slam.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
namespace driver
{
}; // namespace driver

int test6()
{
    int fd;
    int nread, nwrite;
    char buf[1024];

    fd = open("/dev/globalmem", O_RDWR);
    if (fd < 0)
    {
        perror("open /dev/globalmem error");
        return -1;
    }

    strcpy(buf, "hello, this is globalmem device driver");
    nwrite = write(fd, buf, strlen(buf));
    if (nwrite < 0)
    {
        perror("write /dev/globalmem error");
        return -1;
    }
    printf("write %d(bytes) to globalmem: %s\n", nwrite, buf);

    nread = read(fd, buf, sizeof(buf));
    if (nread < 0)
    {
        perror("read /dev/globalmem error");
        return -1;
    }
    buf[nread] = '\0';
    printf("read %d(bytes) from globalmem: %s\n", nread, buf);
    close(fd);
    return 0;
}
// typedef struct A {
//   char t : 4;
//   char k : 4;
//   unsigned short i;
//   unsigned short d;
//   unsigned short f;
//   unsigned short h;
//   unsigned short j;
//   unsigned long m;
//   /* data */
// } A;

int main(int args, char *argv[])
{
    // A a;
    // static char ts[] = "hahaha\0";
    // printf("%d ", sizeof(a));
    // // printf("%d %d ", sizeof(unsigned long), sizeof(unsigned short));
    // std::vector<int> b(10);
    // for (int i = 0; i < 10; i++) {
    //   b[i] = i;
    //   /* code */
    // }
    // showss(b);
    // driver::test6();
    test6();
    return 0;
}
