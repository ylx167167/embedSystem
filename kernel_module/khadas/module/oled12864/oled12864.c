

#include <linux/slab.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
// #include <linux/spi
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>  //copy_to_user
#include <linux/mutex.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/spi/spi.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include "oled12864.h"

#define DEV_NAME "oled12864"

struct oled12864_device {
  int irq;
  int cs_gpio;   // GPIOH_6
  int rst_gpio;  // GPIOA_3
  int dc_gpio;   // GPIOA_4

  char databuf[256];
  dev_t dev_no;
  int major;
  int minor;
  struct cdev chrdev;
  struct class *class;
  struct device *device;
  struct mutex m_lock;
  //   struct oled12864_data data;
  struct spi_device *spi;
  int tm;
  struct timer_list timer;
  struct work_struct wq;
};


static struct oled12864_device *oled12864_dev;
// SPI 写指令
static int oled12864_spi_write(struct spi_device *dev, u8 *buf, int len) {
  int status;
  status = spi_write(dev, buf, len);
  return status;
}

// 向oled发送cmd
static int oled12864_write_cmd(struct oled12864_device *dev, u8 buf, u8 cmd) {
  int ret;
  if (cmd == OLED_CMD) {
    gpio_set_value(dev->dc_gpio, OLED_CMD);
  } else if (cmd == OLED_DATA) {
    gpio_set_value(dev->dc_gpio, OLED_DATA);
  }
  // 片选拉低
  gpio_set_value(oled12864_dev->cs_gpio, 0);
  ret = oled12864_spi_write(dev->spi, &buf, sizeof(buf));
  gpio_set_value(oled12864_dev->cs_gpio, 1);
  gpio_set_value(dev->dc_gpio, OLED_DATA);
  return ret;
}

// 向oled发送data
static int oled12864_write_data(struct oled12864_device *dev, u8 *buf,
                                int len) {
  int ret;
  gpio_set_value(dev->dc_gpio, OLED_DATA);
  ret = oled12864_spi_write(dev->spi, buf, len);
  return ret;
}

// oled显示屏复位
static void oled_set_rst(struct oled12864_device *dev, u16 on_off) {
  int ret;
  ret = gpio_get_value(dev->rst_gpio);
  printk("gpio_get_value dev->rst_gpio before %d\r\n", ret);
  gpio_set_value(dev->rst_gpio, on_off);
  ret = gpio_get_value(dev->rst_gpio);
  printk("gpio_get_value dev->rst_gpio after  %d\r\n", ret);
}
static void oled_reset(struct oled12864_device *dev) {
  oled_set_rst(dev, 1);
  mdelay(100);
  oled_set_rst(dev, 0);
  mdelay(100);
  oled_set_rst(dev, 1);
}

// oled显示器开关
static void oled_disp_on_off(struct oled12864_device *dev, uint8_t on_off) {
  if (on_off)
    oled12864_write_cmd(dev, 0xaf, OLED_CMD); /* set dispkay on */
  else
    oled12864_write_cmd(dev, 0xae, OLED_CMD); /* set dispkay off */
}


static int oled_set_pos(struct oled12864_device *dev, uint16_t x, uint16_t y) {
  int ret = 0;
  ret = oled12864_write_cmd(dev, 0xb0 + y, OLED_CMD);
  ret = oled12864_write_cmd(dev, ((x & 0xf0) >> 4) | 0x10, OLED_CMD);
  ret = oled12864_write_cmd(dev, (x & 0x0f) | 0x01, OLED_CMD);
  return ret;
}
static void oled_disp_clear(struct oled12864_device *dev) {  // 显示器清屏
  uint8_t x, y;
  for (y = 0; y < 8; y++) {
    oled12864_write_cmd(dev, 0xb0 + y, OLED_CMD);  // 设置页地址（0~7）
    oled12864_write_cmd(dev, 0x02, OLED_CMD);  // 设置显示位置—列低地址
    oled12864_write_cmd(dev, 0x10, OLED_CMD);  // 设置显示位置—列高地址
    for (x = 0; x < 128; x++) oled12864_write_cmd(dev, 1, OLED_DATA); /* 清零 */
  }
}
// void OLED_ShowChar(struct oled12864_device *dev, u8 x, u8 y, u8 chr) {
//   unsigned char c = 0, i = 0;
//   c = chr - ' ';  //得到偏移后的值
//   if (x > Max_Column - 1) {
//     x = 0;
//     y = y + 2;
//   }
//   if (SIZE == 16) {
//     oled_set_pos(dev, x, y);
//     for (i = 0; i < 8; i++)
//       oled12864_write_cmd(dev, F8X16[c * 16 + i], OLED_DATA);
//     oled_set_pos(dev, x, y + 1);
//     for (i = 0; i < 8; i++)
//       oled12864_write_cmd(dev, F8X16[c * 16 + i + 8], OLED_DATA);
//   } else {
//     oled_set_pos(dev, x, y + 1);
//     for (i = 0; i < 6; i++) oled12864_write_cmd(dev, F6x8[c][i], OLED_DATA);
//   }
// }

static void oled_disp_test(struct oled12864_device *dev) {
  uint8_t x, y;
  for (y = 0; y < 8; y++) {
    oled_set_pos(dev, 0, y);
    for (x = 0; x < 128; x++) {
      if (x % 2 == 0)
        oled12864_write_cmd(dev, 0, OLED_DATA);
      else
        oled12864_write_cmd(dev, 1, OLED_DATA);
    }
  }
}


static int oled_init(struct oled12864_device *dev) {
  int ret = 0;
  oled_reset(dev);
  oled12864_write_cmd(dev, 0xAE, OLED_CMD);
  oled12864_write_cmd(dev, 0x02, OLED_CMD);
  oled12864_write_cmd(dev, 0x10, OLED_CMD);
  oled12864_write_cmd(dev, 0x40, OLED_CMD);
  oled12864_write_cmd(dev, 0x81, OLED_CMD);
  oled12864_write_cmd(dev, 0xCF, OLED_CMD);
  oled12864_write_cmd(dev, 0xA1, OLED_CMD);
  oled12864_write_cmd(dev, 0xC8, OLED_CMD);
  oled12864_write_cmd(dev, 0xA6, OLED_CMD);
  oled12864_write_cmd(dev, 0xA8, OLED_CMD);
  oled12864_write_cmd(dev, 0x3f, OLED_CMD);
  oled12864_write_cmd(dev, 0xD3, OLED_CMD);
  oled12864_write_cmd(dev, 0x00, OLED_CMD);
  oled12864_write_cmd(dev, 0xd5, OLED_CMD);
  oled12864_write_cmd(dev, 0x80, OLED_CMD);
  oled12864_write_cmd(dev, 0xD9, OLED_CMD);
  oled12864_write_cmd(dev, 0xF1, OLED_CMD);
  oled12864_write_cmd(dev, 0xDA, OLED_CMD);
  oled12864_write_cmd(dev, 0x12, OLED_CMD);
  oled12864_write_cmd(dev, 0xDB, OLED_CMD);
  oled12864_write_cmd(dev, 0x40, OLED_CMD);
  oled12864_write_cmd(dev, 0x20, OLED_CMD);
  oled12864_write_cmd(dev, 0x02, OLED_CMD);  //
  oled12864_write_cmd(dev, 0x8D, OLED_CMD);
  oled12864_write_cmd(dev, 0x14, OLED_CMD);
  oled12864_write_cmd(dev, 0xA4, OLED_CMD);
  oled12864_write_cmd(dev, 0xA6, OLED_CMD);
  oled12864_write_cmd(dev, 0xAF, OLED_CMD);

  oled12864_write_cmd(dev, 0xAF, OLED_CMD); /*display ON*/
  oled_disp_clear(dev);
  oled_set_pos(dev, 0, 0);
  mdelay(500);
  oled_disp_test(dev);


  return ret;
}


static int oled12864_ops_release(struct inode *inode, struct file *file) {
  int ret = 0;
  struct oled12864_device *dev = (struct oled12864_device *)file->private_data;
  if (!dev) {
    pr_err("private_data NULL \r\n");
  }
  return ret;
}

static int oled12864_ops_open(struct inode *inode, struct file *file) {
  printk("oled12864_open\r\n");
  file->private_data = oled12864_dev;
  //   //拉低片选线
  //   gpio_set_value(oled12864_dev->cs_gpio, 0);
  oled_init(oled12864_dev);
  return 0;
}

static long oled12864_ops_unlocked_ioctl(struct file *file, unsigned int cmd,
                                         unsigned long arg) {
  int ret = 0;
  uint8_t buf[3];
  uint16_t size;
  const void __user *userspace = (const void __user *)arg;
  struct oled12864_device *dev = (struct oled12864_device *)file->private_data;
  switch (cmd & 0x0f) /* 最低字节存放命令字段 */
  {
    case OLED_CMD_DISP_ON_OFF:
      ret = copy_from_user(&buf[0], userspace, 1);
      oled_disp_on_off(dev, buf[0]);
      break;
    case OLED_CMD_SET_XY:
      ret = copy_from_user(&buf, userspace, 2);
      if (ret > 0) {
        ret = -EFAULT;
        goto exit;
      }
      // printk("x %d, y %d\r", buf[0], buf[1]);
      oled_set_pos(dev, buf[0], buf[1]);
      break;
    case OLED_CMD_WRITE_DATAS:
      size = (uint16_t)(cmd & 0xffffff00); /* 前三字节存放数据大小 */
      size >>= 8;
      // printk("size %d\r", size);
      ret = copy_from_user(dev->databuf, userspace, size);
      if (ret > 0) {
        ret = -EFAULT;
        goto exit;
      }
      oled12864_write_data(dev, dev->databuf, size);
    case OLED_CMD_SET_XY_WRITE_DATAS:
      ret = copy_from_user(buf, userspace, size);
      if (ret > 0) {
        ret = -EFAULT;
        goto exit;
      }
      break;
  }
exit:
  return 0;
}

static struct file_operations oled12864_ops = {
    .owner = THIS_MODULE,
    .open = oled12864_ops_open,
    .release = oled12864_ops_release,
    .unlocked_ioctl = oled12864_ops_unlocked_ioctl,
};


static int oled12864_probe(struct spi_device *pdev) {
  int ret = 0;
  struct device_node *dts_node, *parent_dts_node;
  pr_info("oled12864_probe\r\n");
  oled12864_dev = (struct oled12864_device *)kzalloc(
      sizeof(struct oled12864_device), GFP_KERNEL);

  // 查看是否存在设备节点
  dts_node = pdev->dev.of_node;
  if (!dts_node) {
    printk("oled espi can not found!\r\n");
    ret = -EINVAL;
    goto exit_free_dev;
  }
  parent_dts_node = of_get_parent(dts_node);
  if (!parent_dts_node) {
    printk("oled parent_dts_node can not found!\r\n");
    ret = -EINVAL;
    goto exit_free_dev;
  }

  oled12864_dev->cs_gpio =
      of_get_named_gpio(parent_dts_node, DC_GPIO_DTS_NAME, 0);
  oled12864_dev->rst_gpio =
      of_get_named_gpio(parent_dts_node, RST_GPIO_DTS_NAME, 0);
  oled12864_dev->dc_gpio =
      of_get_named_gpio(parent_dts_node, CS_GPIO_DTS_NAME, 0);
  if (!gpio_is_valid(oled12864_dev->dc_gpio ||
                     !gpio_is_valid(oled12864_dev->dc_gpio) ||
                     !gpio_is_valid(oled12864_dev->rst_gpio))) {
    printk("don't get oled %s or %s or %s !!!\n", DC_GPIO_DTS_NAME,
           RST_GPIO_DTS_NAME, CS_GPIO_DTS_NAME);
    ret = -EINVAL;
    goto exit_free_dev;
  }
  printk("cs:%d rst:%d dc:%d ", oled12864_dev->cs_gpio, oled12864_dev->rst_gpio,
         oled12864_dev->dc_gpio);
  // 设置GPIO默认输出 全为1
  ret = gpio_direction_output(oled12864_dev->cs_gpio, 1);
  ret = gpio_direction_output(oled12864_dev->rst_gpio, 1);
  ret = gpio_direction_output(oled12864_dev->dc_gpio, 1);
  //   if (!gpio_is_valid(oled12864_dev->dc_gpio) ||
  //       !gpio_is_valid(oled12864_dev->rst_gpio)) {
  //     printk("don't get oled %s!!!\n", RST_GPIO_DTS_NAME);
  //     ret = -EINVAL;
  //     goto exit_free_dev;
  //   }

  //   if (!gpio_is_valid(oled12864_dev->dc_gpio)) {
  //     printk("don't get oled %s!!!\n", CS_GPIO_DTS_NAME);
  //     ret = -EINVAL;
  //     goto exit_free_dev;
  //   }


  // 申请设备号
  oled12864_dev->major = 0;
  if (oled12864_dev->major) {
    oled12864_dev->dev_no = MKDEV(oled12864_dev->major, 0);
    ret = register_chrdev_region(oled12864_dev->dev_no, 1, DEV_NAME);
  } else {
    ret = alloc_chrdev_region(&oled12864_dev->dev_no, 0, 1, DEV_NAME);
    oled12864_dev->major = MAJOR(oled12864_dev->dev_no);
    oled12864_dev->minor = MINOR(oled12864_dev->dev_no);
  }
  if (ret) {
    pr_err("申请设备号失败\r\n");
    goto vo_chr_failure;
  }

  // 初始化字符设备
  oled12864_dev->chrdev.owner = THIS_MODULE;
  cdev_init(&oled12864_dev->chrdev, &oled12864_ops);
  ret = cdev_add(&oled12864_dev->chrdev, oled12864_dev->dev_no, 1);
  if (ret) {
    pr_err("初始化字符设备失败\r\n");
    goto vo_chr_add_failure;
  }

  // 注册类
  oled12864_dev->class = class_create(THIS_MODULE, DEV_NAME);
  if (IS_ERR(oled12864_dev->class)) {
    pr_err("创建类失败\r\n");
    goto vo_class_failure;
  }
  // 注册设备
  oled12864_dev->device = device_create(oled12864_dev->class, NULL,
                                        oled12864_dev->dev_no, NULL, DEV_NAME);
  if (IS_ERR(oled12864_dev->device)) {
    pr_err("创建设备失败\r\n");
    goto vo_device_failure;
  }
  //   //初试一个定时器工作函数
  //   INIT_WORK(&dev->wq, oled12864_workqueue_func);
  //   init_timer(&dev->timer);
  goto out;


vo_device_failure:
  class_destroy(oled12864_dev->class);
vo_class_failure:
  cdev_del(&oled12864_dev->chrdev);
vo_chr_add_failure:
  unregister_chrdev_region(oled12864_dev->dev_no, 1);
vo_chr_failure:
exit_free_dev:
  kfree(oled12864_dev);

out:
  oled12864_dev->spi = pdev;
  return ret;
}

static int oled12864_remove(struct spi_device *pdev) {
  int ret = 0;

  pr_info("oled12864_remove\r\n");
  //   del_timer(&dev->timer);
  device_destroy(oled12864_dev->class, oled12864_dev->dev_no);
  class_destroy(oled12864_dev->class);
  cdev_del(&oled12864_dev->chrdev);
  unregister_chrdev_region(oled12864_dev->dev_no, 1);
  kfree(oled12864_dev);
  return ret;
}


static const struct of_device_id oled12864_match_table[] = {
    {.compatible = "linux,spidev"}, {}};


static struct spi_device_id oled12864_device_id[] = {{.name = "linux,spidev"},
                                                     {}};

static struct spi_driver oled12864_driver = {
    .probe = oled12864_probe,
    .remove = oled12864_remove,
    .driver =
        {
            .name = "oled12864_driver",
            .of_match_table = oled12864_match_table,
        },
    .id_table = oled12864_device_id,
};

static int __init oled12864_init(void) {
  pr_info("oled12864_inits\r\n");
  if (spi_register_driver(&oled12864_driver)) {
    pr_info("failed to register ldim_dev driver module\n");
    return -ENODEV;
  }
  return 0;
}
static void __exit oled12864_exit(void) {
  pr_info("oled12864_exi6\r\n");
  spi_unregister_driver(&oled12864_driver);
}
MODULE_LICENSE("GPL");
module_init(oled12864_init);
module_exit(oled12864_exit);