/*
 *Filename         :gpiotest.c
 *Description      :
 *Author           :wayneyao
 *Time             :2022/02/09 22:25:56 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>  //操作设备树头文件
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>
// #include "gpiotest.h"
// gpio设备的结构体
struct gpio_test {
  dev_t devid;
  int major;  //主设备号
  int minor;  //次设备号
};

struct gpio_test testgpio;

static int __init gpiotest_init(void) {
  int ret = 0;
  //注册字符设备驱动


  return ret;
}

static void __exit gpiotest_exit(void) {}


module_init(gpiotest_init);
module_exit(gpiotest_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wayne");
