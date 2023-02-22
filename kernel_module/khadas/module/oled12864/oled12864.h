#ifndef _OLED_12864_
#define _OLED_12864_

#define OLED_CMD 0
#define OLED_DATA 1

#define DC_GPIO_DTS_NAME "dc-gpio"
#define RST_GPIO_DTS_NAME "res-gpio"
#define CS_GPIO_DTS_NAME "cs-gpio"


#define OLED_CMD_SET_XY 0x01 /* 显示开关*/
#define OLED_CMD_WRITE_DATAS 0x02
#define OLED_CMD_SET_XY_WRITE_DATAS 0x03
#define OLED_CMD_DISP_ON_OFF 0x04

#endif