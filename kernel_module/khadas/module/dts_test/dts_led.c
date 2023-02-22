#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

static int __init dts_led_init(void) {
  int ret;
  return ret;
}


static void __exit dts_led_exit(void) {}


module_init(dts_led_init);
module_exit(dts_led_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wayneyao");
