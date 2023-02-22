/*
 *Filename         :dtsof.c
 *Description      :
 *Author           :wayneyao
 *Time             :2022/01/28 21:05:08 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>  //操作设备树头文件
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>


/*
 *
 */
static int __init dtsof_init(void) {
  // 从设备树中找到某个节点
  int ret = 0;
  struct device_node *nd = NULL;
  struct property *prop = NULL;
  const char *buf;
  // 通过路径来查找节点


  nd = of_find_node_by_path("/wifi");
  if (nd == NULL) {
    printk("\r\n wayneyao ：without this device \r\n");
    return 0;
  }
  printk("\r\n wayneyao ：%s \r\n", nd->name);

  prop = of_find_property(nd, "irq_trigger_type", NULL);
  if (prop == NULL) {
    printk("\r\n wayneyao ：without this property \r\n");
    return 0;
  }
  printk("\r\n wayneyao ：%s  ,%s \r\n", prop->name, (char *)(prop->value));

  //   of_property_read_string(nd, prop, &buf);
  printk("\r\n wayneyao ：%s \r\n", buf);
  return ret;
}
/* wayneyao
 *
 */
static void __exit dtsof_exit(void) {}


//注册模块入口
module_init(dtsof_init);
module_exit(dtsof_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wayneyao");