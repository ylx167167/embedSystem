// #include <linux/init.h>
#include <linux/mod ule.h>

#include <linux/platform_device.h>

MODULE_LICENSE("GPL");


// static struct platform_device hello_device = {
//     .name = "hellodevice",
//     .id = -1,
//     .dev =
//         {
//             .release = hello_release,
//         },
//     .num_resources = ARRAY_SIZE(pxa27x_ohci_resources),
//     .resource = pxa27x_ohci_resources,
// };
static int hello_probe(struct platform_device *dev) {
  printk("hello_probe\r\n");

  return 0;
}
static int hello_remove(struct platform_device *dev) {
  printk("hello_remove\r\n");
  return 0;
}
static struct platform_driver hellodriver = {
    .probe = hello_probe,
    .remove = hello_remove,
    .driver =
        {
            .name = "hellodevice",
        },
};


static int __init hello_init(void) {
  return platform_driver_register(&hellodriver);
}

static void __exit hello_exit(void) {
  platform_driver_unregister(&hellodriver);
}


module_init(hello_init);
module_exit(hello_exit);