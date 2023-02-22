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

#define DEV_NAME "lm75ad"
#define LM75A_REG_TMP 0x00  //  定义LM75A的温度寄存器地址

struct i2c_lm75ad_data {
  /* data */
  //温度
  int t;
  unsigned char temper[2];
};

struct i2c_lm75ad_device {
  int irq;  //中断号3
  int gpio;
  dev_t dev_no;  //设备号
  int major;     //主设备号
  int minor;     //次设备号
  struct cdev chrdev;
  struct class *class;
  struct device *device;
  struct mutex m_lock;
  struct i2c_lm75ad_data data;
  struct i2c_client *client;
};
static struct i2c_lm75ad_device *i2c_lm75ad_dev;

// /*写寄存器*/
// static int lm75ad_ops_write_byte(struct i2c_lm75ad_device *dev, u8 reg, u8
// *buf,
//                                  u8 len) {
//   unsigned char b[256];
//   struct i2c_client *client = dev->client;
//   struct i2c_msg msg[1];
//   b[0] = reg;
//   memcpy(&b[1], buf, len);

//   msg[0].addr = client->addr;
//   msg[0].flags = 0;
//   msg[0].len = len + 1;
//   msg[0].buf = b;


//   return i2c_transfer(client->adapter, msg, 1);
// }


// static void lm75ad_write_reg(struct i2c_lm75ad_device *dev, u8 reg, u8 buf) {
//   unsigned char data = buf;
//   lm75ad_ops_write_byte(dev, reg, &data, 1);
// };

/*读寄存器*/
static int lm75ad_ops_read_byte(struct i2c_lm75ad_device *dev, u8 reg, u8 *data,
                                u16 len) {
  struct i2c_msg msg[2];
  struct i2c_client *client = dev->client;

  msg[0].addr = client->addr;
  msg[0].flags = 0;
  msg[0].buf = &reg;
  msg[0].len = 1;
  i2c_transfer(client->adapter, &msg[0], 1);
  mdelay(100);

  msg[1].addr = client->addr;
  msg[1].flags = I2C_M_RD;
  msg[1].buf = data;
  msg[1].len = len;

  return i2c_transfer(client->adapter, &msg[1], 1);
}


static void lm75ad_read_reg(struct i2c_lm75ad_device *dev, u8 reg, u8 *data,
                            u16 len) {
  lm75ad_ops_read_byte(dev, reg, data, len);
}


static int i2c_lm75ad_open(struct inode *node, struct file *file) {
  int ret = 0;
  file->private_data = i2c_lm75ad_dev;
  /*初始化设备*/
  i2c_lm75ad_dev.data.t = 0;
  i2c_lm75ad_dev.data.temper[0] = '0';
  i2c_lm75ad_dev.data.temper[1] = '0';
  pr_info("i2c_lm75ad_open \r\n");
  pr_info("Major %d\r\n ", i2c_lm75ad_dev->major);
  return ret;
}
static ssize_t i2c_lm75ad_read(struct file *file, char __user *buf, size_t size,
                               loff_t *offset) {
  int ret = 0;

  struct i2c_lm75ad_device *dev =
      (struct i2c_lm75ad_device *)file->private_data;
  pr_info("i2c_lm75ad_read %x \r\n", dev->client->addr);
  /*读取设备寄存器*/
  lm75ad_read_reg(dev, LM75A_REG_TMP, dev->data.temper, 2);
  dev->data.t = dev->data.temper[0];
  dev->data.t <<= 8 | dev->data.temper[1];
  dev->data.t = dev->data.t >> 5;

  if (copy_to_user(buf, &dev->data, sizeof(struct i2c_lm75ad_data))) {
    ret = -EFAULT;
  } else {
    ret = sizeof(struct i2c_lm75ad_data);
  }
  return ret;
}
static ssize_t i2c_lm75ad_write(struct file *file, const char __user *buf,
                                size_t size, loff_t *offset) {
  int ret = 0;
  // struct i2c_lm75ad_device *dev =
  //     (struct i2c_lm75ad_device *)file->private_data;
  pr_info("%s\r\n", buf);
  return ret;
}
static int i2c_lm75ad_release(struct inode *node, struct file *file) {
  int ret = 0;
  struct i2c_lm75ad_device *dev =
      (struct i2c_lm75ad_device *)file->private_data;
  if (!dev) {
    pr_err("private_data NULL \r\n");
  }
  return ret;
}


static const struct file_operations i2c_lm75ad_ops = {
    .owner = THIS_MODULE,
    .open = i2c_lm75ad_open,
    .read = i2c_lm75ad_read,
    .write = i2c_lm75ad_write,
    .release = i2c_lm75ad_release,
};

static int i2c_lm75ad_probe(struct i2c_client *client,
                            const struct i2c_device_id *iid) {
  int err;
  struct device_node *dts_node;

  printk("i2c_lm75ad_probe\r\n");
  //申请驱动私有数据空间
  pr_info("申请驱动私有数据空间\r\n");
  i2c_lm75ad_dev = (struct i2c_lm75ad_device *)kzalloc(
      sizeof(struct i2c_lm75ad_device), GFP_KERNEL);
  if (!i2c_lm75ad_dev) {
    return -ENOMEM;
  }
  //判断是否存在设备节点
  pr_info("判断是否存在设备节点\r\n");
  dts_node = client->dev.of_node;
  if (!dts_node) {
    printk("LM75ad dts node can not find \r\n");
    err = -EINVAL;
    goto ov_free_dev;
  }
  // 驱动私有数据初始化
  i2c_lm75ad_dev->client = client;
  // i2c_lm75ad_dev->irq = client->irq;
  //中断处理函数

  //注册字符设备 设备号->字符设备初始化->注册化类->创建设备节点
  /*内核自动分配设备号*/
  pr_info("内核自动分配设备号\r\n");
  i2c_lm75ad_dev->major = 0;
  if (i2c_lm75ad_dev->major) {
    /* 如果设定了主设备号则直接注册设备号 */
    i2c_lm75ad_dev->dev_no = MKDEV(i2c_lm75ad_dev->major, 0);
    err = register_chrdev_region(i2c_lm75ad_dev->dev_no, 1, DEV_NAME);
  } else {
    /* 未设定主设备号则自动申请一个 */
    err = alloc_chrdev_region(&i2c_lm75ad_dev->dev_no, 0, 1, DEV_NAME);
    i2c_lm75ad_dev->major = MAJOR(i2c_lm75ad_dev->dev_no);
    i2c_lm75ad_dev->minor = MINOR(i2c_lm75ad_dev->dev_no);
  }
  if (err) {
    printk("LM75ad alloc_chrdev_region failed\r\n");
    goto ov_free_irq;
  }
  pr_info("绑定文件操作 注册字符设备\r\n");
  //绑定文件操作 注册字符设备
  i2c_lm75ad_dev->chrdev.owner = THIS_MODULE;
  cdev_init(&i2c_lm75ad_dev->chrdev, &i2c_lm75ad_ops);
  err = cdev_add(&i2c_lm75ad_dev->chrdev, i2c_lm75ad_dev->dev_no, 1);
  if (err) {
    printk("LM75ad alloc_chrdev_region failed\r\n");
    goto ov_free_unrigester_cdev;
  }
  pr_info("创建class\r\n");
  //自动创建设备节点
  i2c_lm75ad_dev->class = class_create(THIS_MODULE, DEV_NAME);
  if (IS_ERR(i2c_lm75ad_dev->class)) {
    printk("LM75ad class_create failed\r\n");
    goto ov_free_cdev;
  }
  pr_info("创建device节点\r\n");
  //创建设备节点
  i2c_lm75ad_dev->device = device_create(
      i2c_lm75ad_dev->class, NULL, i2c_lm75ad_dev->dev_no, NULL, DEV_NAME);
  if (IS_ERR(i2c_lm75ad_dev->device)) {
    printk("LM75ad device_create failed\r\n");
    goto ov_free_class;
  }
  goto out;
//初始化其他参数
ov_free_class:
  class_destroy(i2c_lm75ad_dev->class);
ov_free_cdev:
  cdev_del(&i2c_lm75ad_dev->chrdev);
ov_free_unrigester_cdev:
  unregister_chrdev_region(i2c_lm75ad_dev->dev_no, 1);
ov_free_irq:;
ov_free_dev:
  kfree(i2c_lm75ad_dev);
out:
  return 0;
}


static int i2c_lm75ad_remove(struct i2c_client *pdev) {
  printk("i2c_lm75ad_remove\r\n");
  device_destroy(i2c_lm75ad_dev->class, i2c_lm75ad_dev->dev_no);
  class_destroy(i2c_lm75ad_dev->class);
  cdev_del(&i2c_lm75ad_dev->chrdev);
  unregister_chrdev_region(i2c_lm75ad_dev->dev_no, 1);
  kfree(i2c_lm75ad_dev);
  return 0;
}


static const struct of_device_id lm75adofid[] = {{.compatible = "risym,lm75ad"},
                                                 {}};
static struct i2c_device_id lm75adpdid[] = {{"haha"}, {}};
// static struct platform_device_id lm75adpdid[] = {{"haha"}, {}};

static struct i2c_driver i2c_lm75ad_driver = {
    .probe = i2c_lm75ad_probe,
    .remove = i2c_lm75ad_remove,
    .driver =
        {
            .name = "lm75ad",
            .of_match_table = lm75adofid,  //有设备数时用这个
        },
    .id_table = lm75adpdid,  //没设备树时候用这个
};

static int __init i2c_lm75ad_init(void) {
  printk("i2c_lm75ad_init\r\n");
  return i2c_add_driver(&i2c_lm75ad_driver);
  //   return platform_driver_register(&i2c_lm75ad_driver);
}
static void __exit i2c_lm75ad_exit(void) {
  printk("i2c_lm75ad_exit\r\n");
  i2c_del_driver(&i2c_lm75ad_driver);
  //   platform_driver_unregister(&i2c_lm75ad_driver);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wayne");
// module_i2c_driver(i2c_lm75ad_driver);

module_init(i2c_lm75ad_init);
module_exit(i2c_lm75ad_exit);