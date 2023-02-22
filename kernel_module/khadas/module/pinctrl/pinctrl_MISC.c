/*
 *Filename         :pinctrl_test.c
 *Description      :
 *Author           :wayneyao
 *Time             :2022/02/13 00:34:36 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>  //平台操作需要
#include <linux/ioport.h>           // resource头文件
#include <linux/of.h>               //操作设备树头文件
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>

#include <linux/slab.h>
#include "pinctrl_test.h "