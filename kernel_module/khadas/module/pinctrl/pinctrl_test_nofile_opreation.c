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
#include "pinctrl_test.h"
#include <linux/ioport.h>  // resouce.h

#define PINCTCL_PIN_NAME "pinctrl_test"    //引脚名 供系统
#define PINCTCL_DEVICE_NAME "pinctrl-sys"  // 设备名


static int pinctrl_test_probe(struct platform_device *pdv) {
  struct aml_pinctrl_dev *pdev;
  struct aml_pinctrl_dev *pdev2;
  struct device_node *node;
  enum of_gpio_flags flags;
  // struct resource *res; 有了pinctrl系统就不用gpio地址映射了

  int pin_gpios;
  int ret;

  pr_info("开始分析\r\n");
  //存储用户在probe()中主动申请的内存区域的指针以防止丢失platform_get_drvdata()
  pdev = kzalloc(sizeof(struct aml_pinctrl_dev), GFP_KERNEL);
  platform_set_drvdata(pdv, pdev);
  // //从设备树种获取节点和gpio

  node = pdv->dev.of_node;  //驱动和设备匹配以后
                            //设备信息就会从设备树节点转为platform_device
  if (!node) {
    pr_err("failed to find node for %s\n", PINCTCL_PIN_NAME);
    return -ENODEV;
  }
  pr_info("of_node.name  is %s\r\n", node->name);
  pin_gpios = of_get_named_gpio_flags(node, "gpios", 0, &flags);
  if (!gpio_is_valid(pin_gpios)) {
    pr_err("gpio %d is not valid\n", pin_gpios);
    return -EINVAL;
  }
  pr_info("pin_gpios  %d is valid\r\n", pin_gpios);
  //注册设备号


  //控制节点信息
  pdev2 = platform_get_drvdata(pdv);
  pdev2->d.pin = pin_gpios;
  pr_info("gpio_state %d \r\n", gpio_get_value(pdev2->d.pin));
  pdev2->d.name = PINCTCL_DEVICE_NAME;
  pdev2->d.active_low = flags & OF_GPIO_ACTIVE_LOW;
  ret = gpio_request(pdev2->d.pin, PINCTCL_PIN_NAME);  //为gpio 设置label
  if (ret) {
    pr_err("faile to request the led gpio %d \r\n", pdev2->d.pin);
    goto gpio_failed;
  }

  ret = gpio_direction_output(pdev2->d.pin, 1);  // 为gpio设置输出方向
  gpio_set_value(pdev2->d.pin, 0);
  pr_info("gpio_state %d \r\n", gpio_get_value(pdev2->d.pin));

  if (ret) {
    goto gpio_failed;
  }

  //解析完成
  return 0;

gpio_failed:
  gpio_free(pdev2->d.pin);
  return 0;
}


static void pinctrl_test_shutdown(struct platform_device *pdv) {
  pr_info("关闭\r\n");
  ;
}


/* wayneyao
  struct platform_driver {
    int (*probe)(struct platform_device *);
    int (*remove)(struct platform_device *);
    void (*shutdown)(struct platform_device *);
    int (*suspend)(struct platform_device *, pm_message_t state);
    int (*resume)(struct platform_device *);
    struct device_driver driver;
    const struct platform_device_id *id_table;
    bool prevent_deferred_probe;
  };
 *现在platform中注册该驱动
 */
static const struct of_device_id of_pinctrl_test_match[] = {
    {.compatible = "amlogic,pinctrltest"},
    {},
};

static struct platform_driver pinctrl_driver = {
    .probe = pinctrl_test_probe,
    .shutdown = pinctrl_test_shutdown,
    .driver =
        {
            .of_match_table = of_pinctrl_test_match,  //注册一个of_match_table
            .name = "pinctrl_test",
            .owner = THIS_MODULE,
        },

};


static int __init pinctrl_init(void) {
  if (platform_driver_register(&pinctrl_driver)) {
    pr_err("failed to register driver\n");
    return -ENODEV;
  }


  return 0;
}
static void __exit pinctrl_exit(void) {
  pr_info("pinctrl_driver module exit\n");
  platform_driver_unregister(&pinctrl_driver);
}


// module_platform_driver(gpio_led_driver);
// 不能这么用


module_init(pinctrl_init);
module_exit(pinctrl_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wayneyao");