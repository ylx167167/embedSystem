#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "VideoFramesender.h"
#include <unistd.h>


GN_VideoFramesender client;


GN_VIDEO_SEND_ERROR GN_videoUdpClient(const char* ip, int port) {
  if ((client.sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    // ERR_EXIT("socket");
    return VIDEO_SEND_UDP_ERROR;
  }
  memset(&(client.servaddr), 0, sizeof(client.servaddr));
  client.servaddr.sin_family = AF_INET;
  client.servaddr.sin_port = htons(port);
  client.servaddr.sin_addr.s_addr = inet_addr(ip);
  client.udp_piece = udp_piece_init(64 * 1024);
  client.state = 0x00;
  if (client.udp_piece == NULL) {
    return VIDEO_SEND_UDP_PIECES_INIT;
  }
  return VIDEO_SEND_UDP_SUCCESS;
}

GN_VIDEO_SEND_ERROR GN_videoUdpClose() {
  sleep(2);
  udp_piece_deinit(client.udp_piece);
  close(client.sock);
  return VIDEO_SEND_UDP_SUCCESS;
}

/* 传入jPEG数据buffer */
GN_VIDEO_SEND_ERROR GN_videoFramsender(unsigned char* frame, int size) {
  client.sendbf = frame;
  client.sendlen = size;
  /* 一帧数据 ：分包 > 切片 */
  int pk_num;
  int pk_last_byte;
  int pieces_num;
  char* l_buf;  // buffer偏移指针
  int l_len;
  int l_state;
  pk_num = size / FRAME_ONE_PACKAGE_MAX_BYTE;
  pk_last_byte = size % FRAME_ONE_PACKAGE_MAX_BYTE;
  if (pk_num > 0) {  //一帧图像分包发送
    client.frame_subpackage_flag = 1;
    for (int i = 0; i < pk_num + 1; i++) {
      l_buf = (char*)(client.sendbf + i * FRAME_ONE_PACKAGE_MAX_BYTE);

      if (i == pk_num) {  //最后一包发送
        l_len = pk_last_byte;
      } else {
        l_len = FRAME_ONE_PACKAGE_MAX_BYTE;
      }

      //前面所有包发送 对每个包进行切片处理
      pieces_num = udp_piece_cut(client.udp_piece, l_buf, l_len);
      if (pieces_num == 0) {
        return VIDEO_SEND_UDP_PIECES_COUNT;
      }

      for (int j = 0; j < pieces_num; j++) {
        char* buf;
        int size;
        if (i == pk_num) {
          l_state = 0;
        } else {
          l_state = 1;
        }
        buf = (char*)udp_piece_get_subpackage(client.udp_piece, j, &size,
                                              l_state);
        sendto(client.sock, buf, 7, 0, (struct sockaddr*)&client.servaddr,
               sizeof(client.servaddr));
        sendto(client.sock, buf + 7, size - 7, 0,
               (struct sockaddr*)&client.servaddr, sizeof(client.servaddr));
      }
    }

  } else {  //一帧图像直接全部发送
    client.frame_subpackage_flag = 0;
    for (int i = 0; i < pk_num + 1; i++) {
      l_buf = (char*)(client.sendbf + i * FRAME_ONE_PACKAGE_MAX_BYTE);
      l_len = pk_last_byte;
      //直接将整包发送
      pieces_num = udp_piece_cut(client.udp_piece, l_buf, l_len);

      if (pieces_num == 0) {
        return VIDEO_SEND_UDP_PIECES_COUNT;
      }

      for (int j = 0; j < pieces_num; j++) {
        char* buf;
        int size;
        buf = (char*)udp_piece_get(client.udp_piece, j, &size);
        sendto(client.sock, buf, 7, 0, (struct sockaddr*)&client.servaddr,
               sizeof(client.servaddr));
        sendto(client.sock, buf + 7, size - 7, 0,
               (struct sockaddr*)&client.servaddr, sizeof(client.servaddr));
      }
    }
  }
  return VIDEO_SEND_UDP_SUCCESS;
}


GN_VIDEO_SEND_ERROR GN_videobuffersender(unsigned char* bf, int size) {
  return VIDEO_SEND_UDP_SUCCESS;
}

GN_VIDEO_SEND_ERROR GN_videoDevicesender(int dv_num) {
  return VIDEO_SEND_UDP_SUCCESS;
}

GN_VIDEO_SEND_ERROR GN_videoFilesender(char* fp) {
  return VIDEO_SEND_UDP_SUCCESS;
}