#include "stdio.h"
#include <sys/ioctl.h>
#include "fcntl.h"
#include "unistd.h"
#include "string.h"
#include <fcntl.h>
#include <math.h>
#define pingpong(x) ((x) & (0x1U))
#define isPong(x) (pingpong(x))
#define RADARDEMO_AOAESTBF_PIOVER180 \
  (3.141592653589793 / 180.0)                       //!< define the pi/180
#define RADARDEMO_AOAESTBF_PI (3.141592653589793f)  //!< define pi


struct i2c_lm75ad_data {
  /* data */
  //温度
  int t;
  unsigned char temper[2];
};

struct adxl345_data {
  unsigned char devid;
  unsigned char x;
  unsigned char y;
  unsigned char z;
  unsigned char xyzbuf[6];
};

#define ADXL345_CMD_X 0x01
#define ADXL345_CMD_Y 0x02
#define ADXL345_CMD_Z 0x03
#define ADXL345_CMD_XYZ 0x04
#define ADXL345_CMD_TIME _IOWR('y', 0x05, int)
//*********************************************************
unsigned char ge, shi, bai, qian, wan;  //显示变量
void conversion(unsigned int temp_data) {
  wan = temp_data / 10000 + 0x30;
  temp_data = temp_data % 10000;  //取余运算
  qian = temp_data / 1000 + 0x30;
  temp_data = temp_data % 1000;  //取余运算
  bai = temp_data / 100 + 0x30;
  temp_data = temp_data % 100;  //取余运算
  shi = temp_data / 10 + 0x30;
  temp_data = temp_data % 10;  //取余运算
  ge = temp_data + 0x30;
}

typedef struct _CPLXF {
  float real;
  float imag;
} cplxf_t;

typedef struct cmplx16 {
  int real; /*!< @brief real part */
  int imag; /*!< @brief imaginary part */
} cmplx16ReIm_t;
typedef struct cmplx16r {
  int imag; /*!< @brief imaginary part */
  int real; /*!< @brief real part */
} cmplx16ImRe_t;


void Re16bitIm16bit_swap(cmplx16ReIm_t *input, cmplx16ImRe_t *output, int n) {
  int i;
  for (i = 0; i < n; i++) {
    output[i].real = input[i].real;
    output[i].imag = input[i].imag;
  }
}


void ODDemo_Heatmap_steeringVecGen(cplxf_t *steeringVec) {
  int nRxAnt = 8;
  int steeringVecSize = 48;
  int estAngleRange = 60;
  int estAngleResolution = (estAngleRange * 2) / steeringVecSize;
  int i, j;
  double ftemp1, freal1, fimag1, frealJ, fimagJ;
  // 48 * 8
  //  Ant0's steeringVec is 1 for all angle possiblities, so we don't save them
  for (i = 0; i < steeringVecSize; i++) {
    ftemp1 = (double)sin((-estAngleRange + (double)i * estAngleResolution) *
                         (double)RADARDEMO_AOAESTBF_PIOVER180);
    freal1 = (double)cos(-RADARDEMO_AOAESTBF_PI * ftemp1);
    fimag1 = (double)sin(-RADARDEMO_AOAESTBF_PI * ftemp1);
    frealJ = freal1;
    fimagJ = fimag1;
    steeringVec[(nRxAnt - 1) * i + 0].real = (float)frealJ;
    steeringVec[(nRxAnt - 1) * i + 0].imag = (float)fimagJ;
    for (j = 2; j < nRxAnt; j++) {
      ftemp1 = frealJ;
      frealJ = frealJ * freal1 - fimagJ * fimag1;
      fimagJ = ftemp1 * fimag1 + fimagJ * freal1;
      // printf("%f\r\n", fimagJ);
      steeringVec[(nRxAnt - 1) * i + j - 1].real = (float)frealJ;
      steeringVec[(nRxAnt - 1) * i + j - 1].imag = (float)fimagJ;
    }
  }
}

int main() {
  // int fd;
  // int err;
  // char *buf = "haha";
  // struct i2c_lm75ad_data data;
  // int data_temp = 0;
  // float temp;
  // fd = open("/dev/adxl345", O_RDWR);
  // if (fd < 0) {
  //   printf("open  device failed. %d \n", fd);
  //   return -1;
  // }
  // while (1) {
  //   read(fd, &data, sizeof(struct i2c_lm75ad_data));
  //   data_temp = data.temper[0];
  //   data_temp = data_temp << 8 | data.temper[1];
  //   data_temp = data_temp >> 5;

  //   if (data.t & 0x4000) {
  //     data.t = ~data.t + 1 * -0.125f;
  //   } else {
  //     data.t = data.t * -0.125f;
  //   }
  //   temp = data_temp * 0.125;
  //   // printf("temper :%x%x \r\n", data.temper[0], data.temper[1]);
  //   printf("temper %f\r\n", temp);
  //   /* code */
  // }

  //// printf("temper %d\r\n", data.t);
  //// write(fd, "haha", strlen("haha"));
  int fd;
  int err;
  int tm = 1;
  // char *buf = "haha";
  struct adxl345_data data;
  int data_temp = 0;
  char x_data, y_data, z_data;
  // fd = open("/dev/oled12864", O_RDWR);
  // if (fd < 0) {
  //   printf("open  device failed. %d \n", fd);
  //   return -1;
  // }

#if 0
  sleep(10);
  if (ioctl(fd, ADXL345_CMD_TIME, &tm) < 0) {
    printf("error IOC_GET_ACCEL\r\n");
  }
  // printf("%x\r\n", data.devid);
  // while (1) {
  //   if (ioctl(fd, ADXL345_CMD_X, &data) < 0) {
  //     printf("error IOC_GET_ACCEL\r\n");
  //   }
  //   x_data = (data.xyzbuf[1] << 8 | data.xyzbuf[0]);
  //   y_data = (data.xyzbuf[3] << 8 | data.xyzbuf[2]);
  //   z_data = (data.xyzbuf[5] << 8 | data.xyzbuf[4]);
  //   printf("x: %d   y: %d    z: %d\r\n", x_data, y_data, z_data);

  //   /* code */
  // }

  if (ioctl(fd, ADXL345_CMD_X, &data) < 0) {
    printf("error IOC_GET_ACCEL\r\n");
  }

  // printf("%x%x ", data.xyzbuf[0], data.xyzbuf[1]);
  // int x_data = 65535 - (data.xyzbuf[1] << 8 | data.xyzbuf[0]);
  // int x_data = (data.xyzbuf[0] << 8 | data.xyzbuf[1]);
  // printf("%d ", x_data);
  // printf("%x%x ", data.xyzbuf[2], data.xyzbuf[3]);
  // printf("%x%x", data.xyzbuf[4], data.xyzbuf[5]);
  // while (1) {
  //   read(fd, &data, sizeof(struct adxl345_data));
  //   data_temp = data.temper[0];
  //   data_temp = data_temp << 8 | data.temper[1];
  //   data_temp = data_temp >> 5;

  //   if (data.t & 0x4000) {
  //     data.t = ~data.t + 1 * -0.125f;
  //   } else {
  //     data.t = data.t * -0.125f;
  //   }
  //   temp = data_temp * 0.125;
  //   // printf("temper :%x%x \r\n", data.temper[0], data.temper[1]);
  //   printf("temper %f\r\n", temp);
  //   /* code */
  // }

#endif
#if 0
  char buf[10] = {'1'};
  char *p = &buf[0];
  for (int i = 0; i < strlen(buf); i++) {
    printf("%c \r\n", buf[i]);
  }
  p = (char *)&buf[0];
  printf("%c\r\n", p[0]);
  printf("%d\r\n", &buf[0]);
  printf("%d\r\n", &p[0]);
#endif

#if 0
  cplxf_t bufsteeringVec[48 * 7] = {0};
  ODDemo_Heatmap_steeringVecGen(bufsteeringVec);
  for (size_t i = 0; i < 48; i++) {
    printf("real:img :");
    for (size_t j = 0; j < 7; j++) {
      printf("(%lf,%lf) ", bufsteeringVec[i * 7 + j].real,
             bufsteeringVec[i * 7 + j].imag);
    }
    printf("\r\n");
  }
#endif

  cmplx16ReIm_t inputAntSamples[] = {
      {
          .real = 9,
          .imag = 10,
      },
      {
          .real = 8,
          .imag = 7,
      },
  };
  cmplx16ImRe_t *oddemo_swapBuf;

  int chirpStep = 2;
  Re16bitIm16bit_swap(inputAntSamples, oddemo_swapBuf, chirpStep);
  // printf("%d %d", oddemo_swapBuf[0].imag, oddemo_swapBuf[0].real);
  // printf("%d %d", oddemo_swapBuf[1].imag, oddemo_swapBuf[1].real);
  unsigned char *ptr = (unsigned char *)inputAntSamples;
  printf("ptr %crn", ptr[0]);
  return 0;
}
