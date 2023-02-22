// #include <opencv2/opencv.hpp>
// using namespace cv;
// int main() {
//   Mat picture = imread("/home/khadas/nfs/work/bs_/resources/image/1.jpg");
//   //图片必须添加到工程目下
//   //也就是和test.cpp文件放在一个文件夹下！！！
//   imshow("测试程序", picture);
//   waitKey(20150901);
// }

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "udp_piece/udp-piece.h"
#include "udp_piece/VideoFramesender.h"
#include "log/logger.hpp" //打开log
#define HAVE_OPENCV_HIGHGUI
#define HAVE_OPENCV_VIDEO // 开启视频选项
#define HAVE_OPENCV_VIDEOIO
#include "opencv2/opencv.hpp"
#include "opencv2/videoio/legacy/constants_c.h"
#include <vector>
using namespace cv;
using namespace std;

/*
 * 编译:gcc -o udp-piece-client udp-piece-client.c  udp-piece.c
 * circular_buffer.c
 */

#define MYPORT 8887
#define JPG_QUALITY 50
char *SERVERIP = "192.168.137.1";
// char *SERVERIP = "127.0.0.1";

#define ERR_EXIT(m)     \
  do                    \
  {                     \
    perror(m);          \
    exit(EXIT_FAILURE); \
  } while (0)

int file_size(char *filename) // 获取文件名为filename的文件大小。
{
  FILE *fp = fopen(filename, "rb"); // 打开文件。
  int size;
  if (fp == NULL)
  {
    return -1;
  }                       // 打开文件失败
  fseek(fp, 0, SEEK_END); // 定位文件指针到文件尾。
  size = ftell(fp);       // 获取文件指针偏移量，即文件大小。
  fclose(fp);             // 关闭文件。
  return size;
}

void echo_movie_client()
{
  // int sock;
  // if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) ERR_EXIT("socket");
  // struct sockaddr_in servaddr;
  // memset(&servaddr, 0, sizeof(servaddr));
  // servaddr.sin_family = AF_INET;
  // servaddr.sin_port = htons(MYPORT);
  // servaddr.sin_addr.s_addr = inet_addr(SERVERIP);
  GN_videoUdpClient(SERVERIP, MYPORT); // 初始化udp客户端
  int ret;
  int len;
  int pieces;
  int send_len = 0;
  static int i = 0; // 帧
  VideoCapture captrue("./Robotica_1080.wmv");
  Mat capframe, frame, frame_small, gray;
  udp_piece_t *udp_piece = udp_piece_init(200 * 1024);

  if (!captrue.isOpened())
  {
    LOG__DEBUG("video not open.");
  }
  captrue >> capframe; // 将视频读取到frame
  /* 计算视频一些参数*/
  int framesize = sizeof(frame);
  double datasize = capframe.dataend - capframe.datastart;
  double shrinksize = datasize / 16;
  double frame_count = captrue.get(CAP_PROP_FRAME_COUNT);
  double frame_width = captrue.get(CAP_PROP_FRAME_WIDTH);
  double frame_height = captrue.get(CAP_PROP_FRAME_HEIGHT);
  double frame_velocity = captrue.get(CAP_PROP_FPS);
  LOG__DEBUG("datasize ：" << datasize);
  LOG__DEBUG("shrinksize ：" << shrinksize);
  LOG__DEBUG("frame_count ：" << frame_count);
  LOG__DEBUG("frame_height ：" << frame_height);
  LOG__DEBUG("frame_width ：" << frame_width);
  LOG__DEBUG("frame_velocity ：" << frame_velocity); // FPS达到3

  std::vector<unsigned char> encoded_jpeg;
  unsigned char *encode_jpeg_c_str = NULL;
  std::vector<int> compression_params;
  compression_params.push_back(IMWRITE_JPEG_QUALITY);
  compression_params.push_back(JPG_QUALITY);

  while (captrue.isOpened())
  {
    captrue.read(frame);
    if (frame.empty())
    {
      LOG__DEBUG("empty\r\n");
      break;
    }

    resize(frame, frame_small, Size(), 1, 1);
    // flip(frame_small, frame_small, -1);
    cv::cvtColor(frame_small, gray, cv::COLOR_BGR2GRAY);
    /* 将帧数据作为jpeg格式*/
    cv::imencode(".jpg", frame_small, encoded_jpeg, compression_params);
    encode_jpeg_c_str = &encoded_jpeg[0];
    LOG__DEBUG(encoded_jpeg.size());
    /* 将待发送帧通过udp-piece插件进行分包*/
    // pieces = udp_piece_cut(udp_piece, (char *)encode_jpeg_c_str,
    //                        encoded_jpeg.size());  //每个分片小于512
    // LOG__DEBUG("encoded_jpeg :" << encoded_jpeg.size());
    // LOG__DEBUG("pieces :" << pieces);
    // // /*将每一帧的组包发送*/
    // for (int i = 0; i < pieces; i++) {
    //   char *buf;
    //   int size;
    //   buf = (char *)udp_piece_get(udp_piece, i, &size);
    //   // 519+5
    //   sendto(sock, buf, 5, 0, (struct sockaddr *)&servaddr,
    //   sizeof(servaddr)); sendto(sock, buf + 5, size - 5, 0, (struct sockaddr
    //   *)&servaddr,
    //          sizeof(servaddr));
    //   if (send_len != size) {
    //   }
    // }
    // // break;
    GN_videoFramsender(encode_jpeg_c_str, encoded_jpeg.size());
  }
  //
  GN_videoUdpClose();
}
