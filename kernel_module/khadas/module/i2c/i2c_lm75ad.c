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
#include <linux/ioport.h>  // resouce.h

#include <linux/i2c.h>  //i2c必备

#include "i2c_lm75ad.h"


#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/fs.h>

#include <linux/delay.h>

// 在/dev下的名称  设备数量
#define DTS_DEV_NAME "i2c_lm75ad"
#define DTS_DEV_CNT 1
// 创建字符设备
struct i2c_lm75ad_dev {
  struct cdev cdev_lm75ad;
  int major;
  int minor;
  struct class *class;
  struct device *device;
  dev_t device_id;
  void *privatte_client;
};

struct i2c_lm75ad_dev lm75ad_dev;
// 注册i2c总线上的设备id
static const struct i2c_device_id lm75ad_id[] = {{"amlogic,lm75ad"}, {}};


// 通过of_match_table匹配设备树中的设备名
static const struct of_device_id lm75ad_dt_ids[] = {
    {.compatible = "amlogic,lm75ad", .data = (void *)NULL},
    {},
};


/*读取lm75ad的N个寄存器的值*/

static int lm75ad_read_regs(struct i2c_lm75ad_dev *dev, u8 reg, void *val,
                            int len) {
  int ret;
  struct i2c_client *client = (struct i2c_client *)dev->privatte_client;
  struct i2c_msg msg[] = {
      {
          .addr = client->addr, /*I2C从机地址*/
          .flags = 0,
          .buf = &reg, /*写入到i2c设备的地址*/
          .len = 1,    /*i2c从设备地址的长度*/
      },
      {
          .addr = client->addr,
          .flags = I2C_M_RD, /*read data, from slave to master */
          .buf = val,        /*将读到的数据保存到这里*/
          .len = len,        /*要读取的寄存器的长度*/
      },
  };

  ret = i2c_transfer(client->adapter, msg, 2);  // 发送第几个消息
  if (ret < 0) {
    // pr_err(" % s : i2c transfer failed, ret : % d\n ", __func__, ret);
    pr_err("i2c transfer failed, ret : % d\n ", ret);
    return ret;
  }
  return 0;
}
/*写入lm75ad的N个寄存器的值 u8取决于是设备的位数*/
static int lm75ad_write_regs(struct i2c_lm75ad_dev *dev, u8 reg, void *buf,
                             int len) {
  int ret;
  u8 b[256];
  struct i2c_msg msg[0];
  struct i2c_client *client = (struct i2c_client *)dev->privatte_client;

  /* 构建要发送的数据，也就是寄存器首地址+实际数据地址*/
  b[0] = reg;  // 目标从设备的起始地址

  memcpy(&b[1], buf, len);    // 内容
  msg[0].addr = client->addr; /*I2C从机地址*/
  msg[0].flags = 0;
  msg[0].buf = b;       /*写入到i2c设备的地址*/
  msg[0].len = len + 1; /*i2c从设备地址的长度*/

  ret = i2c_transfer(client->adapter, msg, 1);  // 发送第几个消息
  if (ret < 0) {
    // pr_err(" % s : i2c transfer failed, ret : % d\n ", __func__, ret);
    pr_err("i2c transfer failed, ret : % d\n ", ret);
    return ret;
  }
  return 0;
}

static unsigned char lm75ad_read_reg(struct i2c_lm75ad_dev *dev, u8 reg) {
  u8 data = 0;
  lm75ad_read_regs(dev, reg, &data, 1);
  return data;
}

static void lm75ad_write_reg(struct i2c_lm75ad_dev *dev, u8 reg, u8 data) {
  int ret;
  u8 buf = 0;
  buf = data;
  ret = lm75ad_write_regs(dev, reg, &buf, 1);
  if (ret < 0) {
    pr_err("lm75ad_write_reg transfer failed, ret : % d\n ", ret);
  }
}

static int fops_lm75ad_open(struct inode *inode, struct file *file) {
  file->private_data = &lm75ad_dev;
  printk("fops_lm75ad_open");
  // 当外部文件调用时初始化lm75ad
  lm75ad_write_reg(&lm75ad_dev, LM75AD_FLAG1, 0X01);
  mdelay(50);
  lm75ad_read_reg(&lm75ad_dev, LM75AD_FLAG2);
  return 0;
}
static int fops_lm75ad_release(struct inode *inode, struct file *file) {
  file->private_data = &lm75ad_dev;
  return 0;
}
static ssize_t fops_lm75ad_read(struct file *file, char __user *user,
                                size_t cnt, loff_t *loff_t) {
  file->private_data = &lm75ad_dev;
  /* 向用户返回传感器原始数据*/

  return 0;
}
static ssize_t fops_lm75ad_write(struct file *file, const char __user *user,
                                 size_t cnt, loff_t *loff_t) {
  file->private_data = &lm75ad_dev;
  return 0;
}
static struct file_operations lm75ad_fops = {
    .owner = THIS_MODULE,
    .open = fops_lm75ad_open,
    .write = fops_lm75ad_write,
    .read = fops_lm75ad_read,
    .release = fops_lm75ad_release,
};

// 注册设备号
static int register_i2c_dev(void) {
  int ret;
  lm75ad_dev.major = 0;
  // 申请设备号
  if (lm75ad_dev.major) {
    lm75ad_dev.device_id = MKDEV(lm75ad_dev.major, 0);
    ret =
        register_chrdev_region(lm75ad_dev.device_id, DTS_DEV_CNT, DTS_DEV_NAME);
  } else {
    ret = alloc_chrdev_region(&lm75ad_dev.device_id, 0, DTS_DEV_CNT,
                              DTS_DEV_NAME);
    lm75ad_dev.major = MAJOR(lm75ad_dev.device_id);
    lm75ad_dev.minor = MINOR(lm75ad_dev.device_id);
  }
  if (ret < 0) {
    pr_info("rigest chrdev fail\r\n");
    goto fail_devid;
  }
  // 注册字符设备
  lm75ad_dev.cdev_lm75ad.owner = THIS_MODULE;
  cdev_init(&lm75ad_dev.cdev_lm75ad, &lm75ad_fops);
  ret = cdev_add(&lm75ad_dev.cdev_lm75ad, lm75ad_dev.device_id, DTS_DEV_CNT);
  if (ret < 0) {
    pr_info("cdev_add fail\r\n");
    goto fail_cdev;
  }

  // 创建类
  lm75ad_dev.class = class_create(THIS_MODULE, DTS_DEV_NAME);
  if (IS_ERR(lm75ad_dev.class)) {
    ret = PTR_ERR(lm75ad_dev.class);
    pr_info("class_create fail\r\n");
    goto fail_class;
  }
  // 创建设备
  lm75ad_dev.device = device_create(lm75ad_dev.class, NULL,
                                    lm75ad_dev.device_id, NULL, DTS_DEV_NAME);
  if (IS_ERR(lm75ad_dev.device)) {
    ret = PTR_ERR(lm75ad_dev.device);
    pr_info("device_create fail\r\n");
    goto fail_device;
  }

  return 0;

fail_device:
  class_destroy(lm75ad_dev.class);
fail_class:
  cdev_del(&lm75ad_dev.cdev_lm75ad);
fail_cdev:
  unregister_chrdev_region(lm75ad_dev.device_id, DTS_DEV_CNT);
fail_devid:
  return ret;
}


static int lm75ad_drv_probe(struct i2c_client *client,
                            const struct i2c_device_id *dev_id) {
  pr_info("lm75ad I2c Driver probe\r\n");
  // 注册字符设备
  register_i2c_dev();
  lm75ad_dev.privatte_client = client;
  return 0;
}

static int lm75ad_drv_remove(struct i2c_client *client) {
  cdev_del(&lm75ad_dev.cdev_lm75ad);
  class_destroy(lm75ad_dev.class);
  device_destroy(lm75ad_dev.class, lm75ad_dev.device_id);
  unregister_chrdev_region(lm75ad_dev.device_id, DTS_DEV_CNT);
  return 0;
}


// 注册i2c_driver
static struct i2c_driver lm75ad_drv = {
    .driver =
        {
            .name = "aml_lm75ad",
            .owner = THIS_MODULE,
            .of_match_table = lm75ad_dt_ids,
        },
    .probe = lm75ad_drv_probe,
    .remove = lm75ad_drv_remove,
    .id_table = lm75ad_id,
};

static int __init i2c_lm75ad_init(void) {
  int ret;
  ret = i2c_add_driver(&lm75ad_drv);
  if (ret != 0) {
    pr_err("lm75ad I2c Driver add failure\r\n");
    return ret;
  }
  return 0;
}

static void __exit i2c_lm75ad_exit(void) {
  cdev_del(&lm75ad_dev.cdev_lm75ad);
  class_destroy(lm75ad_dev.class);
  device_destroy(lm75ad_dev.class, lm75ad_dev.device_id);
  unregister_chrdev_region(lm75ad_dev.device_id, DTS_DEV_CNT);
  i2c_del_driver(&lm75ad_drv);
}

module_init(i2c_lm75ad_init);
module_exit(i2c_lm75ad_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("i2c_lm75ad ");
MODULE_AUTHOR("wayneyao");
