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


//注册设备基地址
static struct resource platform_device_resource[] = {
    [0] =
        {
            .start = 0,  //不是很懂这个内存是什么意思
            .end = 0,
            .flags = IORESOURCE_MEM,
        },
};

void test_release(struct device *dev) {
  printk("[Wayneyao]: release hardware cache\r\n");
  printk("[Wayneyao]: release hardware cache\r\n");
}


//注册设备
static struct platform_device platform_device_test = {
    .name = "platform_device_test",  //不存在设备树的时候
                                     //这个platform_driver里的一样
    .id = -1,
    .dev =
        {
            .release = test_release,
        },
    //跟资源有关
    .resource = platform_device_resource,
    .num_resources = ARRAY_SIZE(platform_device_resource),
};


static int __init platform_device_init(void) {
  int ret = 0;
  //注册设备
  platform_device_register(&platform_device_test);

  return ret;
}
static void __exit platform_device_exit(void) {
  //取消设备注册
  platform_device_unregister(&platform_device_test);
  return;
}


module_init(platform_device_init);
module_exit(platform_device_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wayneyao");