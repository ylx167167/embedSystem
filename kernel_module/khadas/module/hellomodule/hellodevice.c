// #include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
MODULE_LICENSE("GPL");


static struct resource pxa27x_ohci_resources[] = {
    [0] =
        {
            .start = 0x4C000000,
            .end = 0x4C00ff6f,
            .flags = IORESOURCE_MEM,
        },
    [1] =
        {
            .start = 0x4C00ff6f,
            .end = 0x4C00ff9f,
            .flags = IORESOURCE_IRQ,
        },
};


void hello_release(struct device *dev) { printk("hello_release\r\n"); }

static struct platform_device hello_device = {
    .name = "hellodevice",
    .id = -1,
    .dev =
        {
            .release = hello_release,
        },
    .num_resources = ARRAY_SIZE(pxa27x_ohci_resources),
    .resource = pxa27x_ohci_resources,
};
static int __init hello_init(void) {
  //   platform_add_devices(&hello_device);
  return platform_device_register(&hello_device);
}
static void __exit hello_exit(void) {
  platform_device_unregister(&hello_device);
}

module_init(hello_init);
module_exit(hello_exit);