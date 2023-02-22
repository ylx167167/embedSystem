// #include <linux/slab.h>

// #include <linux/i2c.h>
// #include <linux/uaccess.h>
// #include <linux/init.h>
// #include <linux/module.h>
// #include <linux/fs.h>
// #include <linux/cdev.h>
// #include <linux/mutex.h>
// #include <linux/platform_device.h>
// #include <linux/device.h>

#include <linux/slab.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>  //copy_to_user
#include <linux/mutex.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/workqueue.h>

#include "adxl345.h"

#define DEV_NAME "adxl345"


struct adxl345_data {
  unsigned char devid;
  unsigned char x;
  unsigned char y;
  unsigned char z;
  unsigned char xyzbuf[8];
};
struct adxl345_device {
  int irq;
  int gpio;
  dev_t dev_no;
  int major;
  int minor;
  struct cdev chrdev;
  struct class *class;
  struct device *device;
  struct mutex m_lock;
  struct adxl345_data data;
  struct i2c_client *client;

  int tm;
  struct timer_list timer;
  struct work_struct wq;
};

static struct adxl345_device *adxl345_dev;


static int adxl345_ops_release(struct inode *inode, struct file *file) {
  int ret = 0;
  struct adxl345_device *dev = (struct adxl345_device *)file->private_data;
  if (!dev) {
    pr_err("private_data NULL \r\n");
  }
  return ret;
}

/* 写寄存器 */
static int adxl345_write_byte(struct adxl345_device *dev, unsigned char *buf,
                              int count) {
  struct i2c_client *client = dev->client;
  struct i2c_msg msg;

  msg.addr = client->addr;
  msg.flags = 0;
  msg.len = count;
  msg.buf = buf;
  return i2c_transfer(client->adapter, &msg, 1);
}
/* 写寄存器 */
static int adxl345_set_reg(struct adxl345_device *dev, unsigned char reg,
                           unsigned char value) {
  unsigned char buf[2];
  buf[0] = reg;
  buf[1] = value;
  return adxl345_write_byte(dev, buf, 2);
}

/* 读寄存器 */
static int adxl345_read_reg(struct adxl345_device *dev, unsigned char *buf,
                            char reg, int len) {
  struct i2c_client *client = dev->client;
  struct i2c_msg msg[2];
  msg[0].addr = client->addr;
  msg[0].flags = 0;
  msg[0].len = 1;
  msg[0].buf = &reg;

  msg[1].addr = client->addr;
  msg[1].flags = I2C_M_RD;
  msg[1].len = len;
  msg[1].buf = buf;
  return i2c_transfer(client->adapter, msg, 2);
}
/* 初始化定时器*/
void adxl345_workqueue_func(struct work_struct *work) {
  int16_t x_data = 0, y_data = 0, z_data = 0;
  struct adxl345_device *dev = container_of(work, struct adxl345_device, wq);
  adxl345_read_reg(dev, &dev->data.xyzbuf[0], ADXL345_REG_DATAX0, 6);
  x_data = dev->data.xyzbuf[1] << 8 | dev->data.xyzbuf[0];
  y_data = dev->data.xyzbuf[2] << 8 | dev->data.xyzbuf[3];
  z_data = dev->data.xyzbuf[4] << 8 | dev->data.xyzbuf[5];
  printk("x_data=%d,y_data=%d,z_data=%d \n", x_data, y_data, z_data);
}

static void adxl345_timer_func(unsigned long data) {
  struct adxl345_device *dev = (struct adxl345_device *)data;
  schedule_work(&dev->wq);
  if (dev->tm == 0) {
    mod_timer(&dev->timer, jiffies + HZ);
  } else {
    printk("adxl345_timer_func jiffies %d\r\n", dev->tm);
    mod_timer(&dev->timer, jiffies + msecs_to_jiffies(dev->tm));
  }
}
static void adxl345_timer_init(struct adxl345_device *dev) {
  dev->timer.function = adxl345_timer_func;
  dev->timer.expires = jiffies + HZ;
  dev->timer.data = (unsigned long)dev;
  add_timer(&dev->timer);
}


static int adxl345_ops_open(struct inode *inode, struct file *file) {
  int ret = 0;
  file->private_data = adxl345_dev;
  adxl345_dev->data.devid = 1;

  pr_info("==================adxl345 open=====================\r\n");
  adxl345_set_reg(adxl345_dev, ADXL345_REG_DATA_FORMAT, 0x0B);
  adxl345_set_reg(adxl345_dev, ADXL345_REG_BW_RATE, 0x08);
  adxl345_set_reg(adxl345_dev, ADXL345_REG_POWER_CTL, 0x08);
  adxl345_set_reg(adxl345_dev, ADXL345_REG_INT_ENABLE, 0x80);
  adxl345_set_reg(adxl345_dev, ADXL345_REG_OFSX, 0x00);
  adxl345_set_reg(adxl345_dev, ADXL345_REG_OFSY, 0x00);
  adxl345_set_reg(adxl345_dev, ADXL345_REG_OFSZ, 0x05);
  adxl345_read_reg(adxl345_dev, &adxl345_dev->data.devid, ADXL345_REG_DEVID, 1);
  pr_info("==================adxl345 devid %x \r\n", adxl345_dev->data.devid);
  adxl345_timer_init(adxl345_dev);
  return ret;
}
static long adxl345_ops_unlocked_ioctl(struct file *file, unsigned int cmd,
                                       unsigned long arg) {
  long ret = 0;
  struct adxl345_device *dev = (struct adxl345_device *)file->private_data;
  switch (cmd) {
    case ADXL345_CMD_X:
      adxl345_read_reg(dev, &dev->data.xyzbuf[0], ADXL345_REG_DATAX0, 2);
      break;
    case ADXL345_CMD_Y:
      adxl345_read_reg(dev, &dev->data.xyzbuf[2], ADXL345_REG_DATAY0, 2);
      break;
    case ADXL345_CMD_Z:
      adxl345_read_reg(dev, &dev->data.xyzbuf[4], ADXL345_REG_DATAZ0, 2);
      break;
    case ADXL345_CMD_XYZ:
      adxl345_read_reg(dev, &dev->data.xyzbuf[0], ADXL345_REG_DATAX0, 6);
      break;
    case ADXL345_CMD_TIME:
      ret = copy_from_user(&dev->tm, (void __user *)arg, sizeof(int));
      if (ret) {
        pr_info("copy_from_user failure  %ld \r\n", ret);
        return -EFAULT;
      } else {
        ret = sizeof(struct adxl345_data);
      }
      //   adxl345_read_reg(dev, &dev->data.xyzbuf[0], ADXL345_REG_DATAX0, 6);
      break;
    default:
      printk("invalid argument\n");
      return -EINVAL;
      break;
  }

  ret = copy_to_user((void __user *)arg, &dev->data, sizeof(dev->data));
  if (ret) {
    pr_info("copy_to_user failure  %ld \r\n", ret);
    return -EFAULT;
  } else {
    ret = sizeof(struct adxl345_data);
  }
  return ret;
}

static struct file_operations adxl345_ops = {
    .owner = THIS_MODULE,
    .open = adxl345_ops_open,
    .release = adxl345_ops_release,
    .unlocked_ioctl = adxl345_ops_unlocked_ioctl,
};


static int adxl345_probe(struct i2c_client *client,
                         const struct i2c_device_id *i2c_id) {
  int ret;
  adxl345_dev = (struct adxl345_device *)kzalloc(sizeof(struct adxl345_device),
                                                 GFP_KERNEL);
  adxl345_dev->client = client;

  // 申请设备号
  adxl345_dev->major = 0;
  if (adxl345_dev->major) {
    adxl345_dev->dev_no = MKDEV(adxl345_dev->major, 0);
    ret = register_chrdev_region(adxl345_dev->dev_no, 1, DEV_NAME);
  } else {
    ret = alloc_chrdev_region(&adxl345_dev->dev_no, 0, 1, DEV_NAME);
    adxl345_dev->major = MAJOR(adxl345_dev->dev_no);
    adxl345_dev->minor = MINOR(adxl345_dev->dev_no);
  }
  if (ret) {
    pr_err("申请设备号失败\r\n");
    goto vo_chr_failure;
  }

  // 初始化字符设备
  adxl345_dev->chrdev.owner = THIS_MODULE;
  cdev_init(&adxl345_dev->chrdev, &adxl345_ops);
  ret = cdev_add(&adxl345_dev->chrdev, adxl345_dev->dev_no, 1);
  if (ret) {
    pr_err("初始化字符设备失败\r\n");
    goto vo_chr_add_failure;
  }

  // 注册类
  adxl345_dev->class = class_create(THIS_MODULE, DEV_NAME);
  if (IS_ERR(adxl345_dev->class)) {
    pr_err("创建类失败\r\n");
    goto vo_class_failure;
  }
  // 注册设备
  adxl345_dev->device = device_create(adxl345_dev->class, NULL,
                                      adxl345_dev->dev_no, NULL, DEV_NAME);
  if (IS_ERR(adxl345_dev->device)) {
    pr_err("创建设备失败\r\n");
    goto vo_device_failure;
  }
  // 初试一个定时器工作函数
  //  INIT_WORK(&dev->wq, adxl345_workqueue_func);
  //  init_timer(&dev->timer);
  goto out;

vo_device_failure:
  class_destroy(adxl345_dev->class);
vo_class_failure:
  cdev_del(&adxl345_dev->chrdev);
vo_chr_add_failure:
  unregister_chrdev_region(adxl345_dev->dev_no, 1);
vo_chr_failure:
  kfree(adxl345_dev);

out:
  return ret;
}
static int adxl345_remove(struct i2c_client *client) {
  int ret = 0;
  pr_info("adxl345_remove\r\n");
  // del_timer(&dev->timer);
  device_destroy(adxl345_dev->class, adxl345_dev->dev_no);
  class_destroy(adxl345_dev->class);
  cdev_del(&adxl345_dev->chrdev);
  unregister_chrdev_region(adxl345_dev->dev_no, 1);
  kfree(adxl345_dev);
  return ret;
}


static const struct of_device_id adcl345_match_table[] = {
    {.compatible = "risym,adxl345"}, {}};


static struct i2c_device_id adcl345_device_id[] = {{.name = "haha"}, {}};
static struct i2c_driver adxl345_driver = {
    .probe = adxl345_probe,
    .remove = adxl345_remove,
    .driver =
        {
            .name = "adxl345_driver",
            .of_match_table = adcl345_match_table,
        },
    .id_table = adcl345_device_id,
};

static int __init adxl345_init(void) {
  pr_info("adxl345_inits\r\n");
  return i2c_add_driver(&adxl345_driver);
}
static void __exit adxl345_exit(void) {
  pr_info("adxl345_exit\r\n");

  return i2c_del_driver(&adxl345_driver);
}


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wayne");
module_init(adxl345_init);
module_exit(adxl345_exit);