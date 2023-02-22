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

//驱动绑定设备号和file_operation
#include <linux/types.h>  //设备号
#include <linux/fs.h>     //
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>  //copy_from_user

#include <linux/timer.h>    //定时器
#include <linux/jiffies.h>  //设置定时器

#include <linux/ioctl.h>  //调用file_operations中的unlocked_ioctl ioctl
//在/sys/bus/platform 下的名称
#define PINCTCL_PIN_NAME "pinctrl_test"    //引脚名 供系统
#define PINCTCL_DEVICE_NAME "pinctrl-sys"  // 设备名

//在/dev下的名称  设备数量
#define DTS_DEV_NAME "pinctrl_test"
#define DTS_DEV_CNT 1

// gpio编号
static int pin_gpios = 0;

//字符设备那一套+定时器
struct pinctrl_test_dev {
  struct cdev cdev_test;
  dev_t device_id;
  struct class *class;
  struct device *device;
  int major;
  int minor;
  struct timer_list timer;
  struct device_node *nd;
};


struct pinctrl_test_dev dev_pinctrl;


static int pinctl_test_open(struct inode *inode, struct file *file) {
  file->private_data = &dev_pinctrl;
  return 0;
}
static ssize_t pinctl_test_write(struct file *file, const char __user *buf,
                                 size_t count, loff_t *loff) {
  int retvalue;
  unsigned char databuf[1];

  struct pinctrl_test_dev *dev = (struct pinctrl_test_dev *)file->private_data;
  pr_info("pinctl_test_write \r\n");
  if (!dev) {
    pr_err("private_data NULL \r\n");
  }
  retvalue = copy_from_user(databuf, buf, count);
  if (retvalue < 0) {
    return -EFAULT;
  }

  if (databuf[0] == '0') {
    pr_err("databuf[0] = 0\r\n");
    gpio_set_value(pin_gpios, 0);
  } else if (databuf[0] == '1') {
    pr_err("databuf[0] = 1\r\n");
    gpio_set_value(pin_gpios, 1);
  }


  return 0;
}

static ssize_t pinctl_test_read(struct file *file, char __user *buf,
                                size_t count, loff_t *loff) {
  return 0;
}
static int pinctl_test_release(struct inode *inode, struct file *file) {
  struct pinctrl_test_dev *dev = (struct pinctrl_test_dev *)file->private_data;
  if (!dev) {
    pr_err("private_data NULL \r\n");
  }
  return 0;
}
static struct file_operations pinctrl_fops = {
    .owner = THIS_MODULE,
    .open = pinctl_test_open,
    .write = pinctl_test_write,
    .read = pinctl_test_read,
    .release = pinctl_test_release,
};


static int pinctrl_test_probe(struct platform_device *pdv) {
  struct aml_pinctrl_dev *pdev;
  struct aml_pinctrl_dev *pdev2;
  struct device_node *node;
  enum of_gpio_flags flags;
  // struct resource *res; 有了pinctrl系统就不用gpio地址映射了
  int ret;

  pr_info("pinctrl_test_probe  init \r\n");
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
  gpio_set_value(pdev2->d.pin, 1);
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

static void timer_operate(unsigned long arg) {
#if 0
  struct pinctrl_test_dev *dev = (struct pinctrl_test_dev *)arg;
  static int ret = 0;
  //   ret = gpio_get_value(pin_gpios);
  ret = !ret;
  gpio_set_value(pin_gpios, ret);
  mod_timer(&dev->timer, jiffies + msecs_to_jiffies(500));  //重置定时器
  pr_info("timer_opreate print: major = %d\r\n", dev->major);
#endif
}

static int register_pinctrl_test_dev(void) {
  int ret = 0;
  dev_pinctrl.major = 0;

  /* 1.申请设备号*/
  if (dev_pinctrl.major) {
    pr_info("register_chrdev_region\r\n");
    //如果给定设备号则注册直接注册设备号
    dev_pinctrl.device_id = MKDEV(dev_pinctrl.major, 0);
    ret = register_chrdev_region(dev_pinctrl.device_id, DTS_DEV_CNT,
                                 DTS_DEV_NAME);
  } else {
    //申请个设备号
    pr_info("alloc_chrdev_region\r\n");
    ret = alloc_chrdev_region(&dev_pinctrl.device_id, 0, DTS_DEV_CNT,
                              DTS_DEV_NAME);
    dev_pinctrl.major = MAJOR(dev_pinctrl.device_id);
    dev_pinctrl.minor = MINOR(dev_pinctrl.device_id);
  }
  if (ret < 0) {
    pr_info("rigest chrdev fail\r\n");
    goto fail_devid;
  }

  /*2.注册字符设备*/
  dev_pinctrl.cdev_test.owner = THIS_MODULE;
  cdev_init(&dev_pinctrl.cdev_test, &pinctrl_fops);
  ret = cdev_add(&dev_pinctrl.cdev_test, dev_pinctrl.device_id, DTS_DEV_CNT);
  if (ret < 0) {
    pr_info("cdev_add fail\r\n");
    goto fail_cdev;
  }
  /* 3.自动创建设备节点  类->设备*/
  dev_pinctrl.class = class_create(THIS_MODULE, DTS_DEV_NAME);
  if (IS_ERR(dev_pinctrl.class)) {
    ret = PTR_ERR(dev_pinctrl.class);
    pr_info("class_create fail\r\n");
    goto fail_class;
  }
  dev_pinctrl.device = device_create(dev_pinctrl.class, NULL,
                                     dev_pinctrl.device_id, NULL, DTS_DEV_NAME);
  if (IS_ERR(dev_pinctrl.device)) {
    ret = PTR_ERR(dev_pinctrl.device);
    pr_info("device_create fail\r\n");
    goto fail_device;
  }

  return 0;

fail_class:
  cdev_del(&dev_pinctrl.cdev_test);
fail_device:
  class_destroy(dev_pinctrl.class);
fail_cdev:
  unregister_chrdev_region(dev_pinctrl.device_id, DTS_DEV_CNT);
fail_devid:
  return ret;
}

static int register_pinctrl_test_timer(void) {
  //初始化定时器
  init_timer(&dev_pinctrl.timer);
  dev_pinctrl.timer.function = timer_operate;  //指定定时器处理函数
  dev_pinctrl.timer.expires =
      jiffies + msecs_to_jiffies(1000);  //定时器重置周期
  dev_pinctrl.timer.data =
      (unsigned long)(&dev_pinctrl);  //传给终端处理函数的地址
  //将定时器添加到系统
  add_timer(&dev_pinctrl.timer);
  return 0;
}
static int register_pinctrl_test_interrupt(void) {
  int ret = 0;
  ret = gpio_to_irq(pin_gpios);  //获取终端号
  if (ret) pr_info("irq num is:(%d)\n", ret);
  // return 0;
  dev_pinctrl.nd = of_find_node_by_path("/isp");
  if (dev_pinctrl.nd == NULL) {
    pr_info("HAHA\n");
  }

  ret = irq_of_parse_and_map(dev_pinctrl.nd, 1);
  if (ret) {
    pr_info("irq num is:(%d)\n", ret);
  } else {
    pr_info("no irq num\n");
  }
  return 0;
}

static int __init pinctrl_init(void) {
  pr_info("pinctrl_driver module init \n");
  // platform注册设备 会主动调用probe函数
  if (platform_driver_register(&pinctrl_driver)) {
    pr_err("failed to register driver\n");
    return -ENODEV;
  } else {
    pr_info("platform_driver_register module success \n");
  }
  //注册设备号
  register_pinctrl_test_dev();
  //注册定时器
  register_pinctrl_test_timer();
  //注册中断
  register_pinctrl_test_interrupt();
  return 0;
}
static void __exit pinctrl_exit(void) {
  pr_info("pinctrl_driver module exit\n");

  /*删除字符设备*/
  cdev_del(&dev_pinctrl.cdev_test);
  del_timer(&dev_pinctrl.timer);
  /* 删除class和device*/
  device_destroy(dev_pinctrl.class, dev_pinctrl.device_id);
  class_destroy(dev_pinctrl.class);

  /* 删除设备号*/
  unregister_chrdev_region(dev_pinctrl.device_id, DTS_DEV_CNT);

  /* 卸载驱动注册 */
  platform_driver_unregister(&pinctrl_driver);
  /* 释放gpio */
  gpio_free(pin_gpios);
}


// module_platform_driver(gpio_led_driver);
// 不能这么用


module_init(pinctrl_init);
module_exit(pinctrl_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wayneyao");