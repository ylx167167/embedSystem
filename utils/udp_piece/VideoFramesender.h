#ifndef _VIDEOFRAMESENDER_H
#define _VIDEOFRAMESENDER_H

#ifdef __cplusplus
extern "C" {
#endif
#include "udp-piece.h"                    //udp 传输数据切片
#define FRAME_ONE_PACKAGE_MAX_BYTE 65500  //单张图如果超过这个数值则分包发送

typedef enum GN_VIDEO_SEND_ERROR {
  VIDEO_SEND_DEVICE_ERROR = 0,
  VIDEO_SEND_FILE_ERROR,
  VIDEO_SEND_FRAMES_ERROR,
  VIDEO_SEND_BUFFER_ERROR,
  VIDEO_SEND_UDP_ERROR,               // UDP 套接字创建错误
  VIDEO_SEND_UDP_PIECES_INIT,         // UDP分片初始化
  VIDEO_SEND_UDP_PIECES_COUNT,        // UDP分片出错
  VIDEO_SEND_UDP_PIECES_MERGE_ERROR,  // UDP重组分片出错


  VIDEO_SEND_UDP_SUCCESS = 255,
} GN_VIDEO_SEND_ERROR;


typedef struct GN_VideoFramesender {
  struct sockaddr_in servaddr;
  int sock;  // UDP 套接字
  unsigned char* sendbf;
  int sendlen;
  unsigned char state;        // 正在发送 0x01 正在发送 0x00未发送
  int frame_subpackage_flag;  //是否被分包发送
  udp_piece_t* udp_piece;
} GN_VideoFramesender;

/**
 * @brief 创建UDP 链接
 * @param ip 发送的地址 port 发送端的端口
 * @return
 */
GN_VIDEO_SEND_ERROR GN_videoUdpClient(const char* ip, int port);


/**
 * @brief 关闭UDP 链接
 * @param ip 发送的地址 port 发送端的端口
 * @return
 */
GN_VIDEO_SEND_ERROR GN_videoUdpClose();


/**
 * @brief 通过设备号发送视频
 * @param dv_num 设备号
 * @return
 */
GN_VIDEO_SEND_ERROR GN_videoDevicesender(int dv_num);

/**
 * @brief 通过文件发送视频
 * @param fp
 * @return
 */
GN_VIDEO_SEND_ERROR GN_videoFilesender(char* fp);

/**
 * @brief 通过帧发送视频
 * @param frame  帧buffer
 * @return 成功则返回一个句柄，失败则返回NULL
 */
GN_VIDEO_SEND_ERROR GN_videoFramsender(unsigned char* frame, int size);

/**
 * @brief 通过压缩后的帧发送视频
 * @param bf
 * @return
 */
GN_VIDEO_SEND_ERROR GN_videobuffersender(unsigned char* bf, int size);


#ifdef __cplusplus
}
#endif
#endif