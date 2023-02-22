/*
 *Filename         :platformdevice.c
 *Description      :
 *Author           :wayneyao
 *Time             :2022/02/12 21:05:47 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>  //平台操作需要
#include <linux/ioport.h>           // resource头文件
#include <linux/of.h>               //操作设备树头文件
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>


static int __init platform_drivertest_init(void) {
  int ret = 0;


  return ret;
}
static void __exit platform_drivertest_exit(void) { return; }


module_init(platform_drivertest_init);
module_exit(platform_drivertest_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wayneyao");