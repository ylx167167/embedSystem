#ifndef _ADXL345_H_
#define _ADXL345_H_

#define ADXL345_REG_DEVID 0x00  //器件ID
//保留，不要操作 保留，不要操作
#define ADXL345_REG_THRESH_TAP 0x1D      // 敲击阈值
#define ADXL345_REG_OFSX 0x1E            // X轴偏移
#define ADXL345_REG_OFSY 0x1F            // Y轴偏移
#define ADXL345_REG_OFSZ 0x20            // Z轴偏移
#define ADXL345_REG_DUR 0x21             // 敲击持续时间
#define ADXL345_REG_LATENT 0x22          // 敲击延迟
#define ADXL345_REG_Window 0x23          // 敲击窗口
#define ADXL345_REG_THRESH_ACT 0x24      // 活动阈值
#define ADXL345_REG_THRESH_INACT 0x25    // 静止阈值
#define ADXL345_REG_TIME_INACT 0x26      // 静止时间
#define ADXL345_REG_ACT_INACT_CTL 0x27   // 轴使能控制活动和静止检测
#define ADXL345_REG_THRESH_FF 0x28       // 自由落体阈值
#define ADXL345_REG_TIME_FF 0x29         // 自由落体时间
#define ADXL345_REG_TAP_AXES 0x2A        // 单击/双击轴控制
#define ADXL345_REG_ACT_TAP_STATUS 0x2B  // 单击/双击源
#define ADXL345_REG_BW_RATE 0x2C         // 数据速率及功率模式控制
#define ADXL345_REG_POWER_CTL 0x2D       // 省电特性控制
#define ADXL345_REG_INT_ENABLE 0x2E      // 中断使能控制
#define ADXL345_REG_INT_MAP 0x02F        // 中断映射控制
#define ADXL345_REG_INT_SOURCE 0x30      // 中断源
#define ADXL345_REG_DATA_FORMAT 0x31     // 数据格式控制
#define ADXL345_REG_DATAX0 0x32          // X轴数据0
#define ADXL345_REG_DATAX1 0x33          // X轴数据1
#define ADXL345_REG_DATAY0 0x34          // Y轴数据0
#define ADXL345_REG_DATAY1 0x35          // Y轴数据1
#define ADXL345_REG_DATAZ0 0x36          // Z轴数据0
#define ADXL345_REG_DATAZ1 0x37          // Z轴数据1
#define ADXL345_REG_FIFO_CTL 0x38        // FIFO控制
#define ADXL345_REG_FIFO_STATUS 0x39     // FIFO状态

#define ADXL345_CMD_X _IO('y', 0x01)
#define ADXL345_CMD_Y _IO('y', 0x02)
#define ADXL345_CMD_Z _IO('y', 0x03)
#define ADXL345_CMD_XYZ _IO('y', 0x04)
#define ADXL345_CMD_TIME _IOWR('y', 0x05, int)

#endif