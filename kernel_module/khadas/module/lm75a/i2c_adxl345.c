#include <linux/module.h>
#include <linux/platform_device.h>
MODULE_LICENSE("GPL");


static int __init i2c_adxl345_init(void) { return 0; }
static void __exit i2c_adcl345_exit(void) {}


module_init(i2c_adxl345_init);
module_exit(i2c_adcl345_exit);