#include <linux/module.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#define __int
// #define __exit
#define GLOBALMEM_SIZE 0X1000  // 4KB
#define MEM_CLEAR 0x1
#define GLOBALMEM_MAJOR 200

static int globalmem_major = GLOBALMEM_MAJOR;

struct global_dev {
  struct cdev cdev;  // 字符设备都要的成员
  unsigned char mem[GLOBALMEM_SIZE];
};
struct global_dev *global_devp;


static ssize_t global_read(struct file *filp, char __user *buf, size_t size,
                           loff_t *ppos) {
  unsigned long p = *ppos;
  unsigned int count = size;
  int ret = 0;
  struct global_dev *dev = filp->private_data;
  if (p >= GLOBALMEM_SIZE) {
    return count ? -ENXIO : 0;
  }

  if (count > GLOBALMEM_SIZE - p) count = GLOBALMEM_SIZE - p;
  // 复制到用户空间
  if (copy_to_user(buf, (void *)(dev->mem), count))
    ret = -EFAULT;
  else {
    *ppos += count;
    ret = count;
    printk(KERN_INFO "read %d bytes from %ld\n", count, p);
  }
  return ret;
}
static ssize_t global_write(struct file *filp, const char __user *buf,
                            size_t size, loff_t *ppos) {
  unsigned long p = *ppos;
  unsigned int count = size;
  int ret = 0;
  struct global_dev *dev = filp->private_data;
  printk(KERN_INFO "Begin of Read: p = %ld, count = %d\n", p, count);
  if (p >= GLOBALMEM_SIZE) return count ? -ENXIO : 0;

  if (count > GLOBALMEM_SIZE - p) count = GLOBALMEM_SIZE - p;

  if (copy_from_user(dev->mem + p, buf, count))
    ret = -EFAULT;
  else {
    *ppos += count;
    ret = count;
    printk(KERN_INFO "Written %d bytes from %ld\n", count, p);
  }
  return ret;
}
static long global_unlocked_ioctl(struct file *filp, unsigned int cmd,
                                  unsigned long) {
  struct global_dev *dev = filp->private_data;
  switch (cmd) {
    case MEM_CLEAR:
      memset(dev->mem, 0, GLOBALMEM_SIZE);
      printk(KERN_INFO "Globalmem is set to zero\n");
      break;

    default:
      break;
  }
  return 0;
}

static int global_open(struct inode *inode, struct file *filp) {
  struct global_dev *dev;
  // 通过结构中的某个变量的地址获取结构本身的指针
  dev = container_of(inode->i_cdev, struct global_dev, cdev);
  filp->private_data = dev;
  return 0;
}
static int global_release(struct inode *, struct file *) { return 0; }
static loff_t global_llseek(struct file *filp, loff_t offset, int orig) {
  loff_t ret = 0;
  switch (orig) {
    case 0:
      if (offset < 0) {
        ret = -EINVAL;
        break;
      }
      if ((unsigned int)offset > GLOBALMEM_SIZE) {
        ret = -EINVAL;
        break;
      }
      filp->f_pos = (unsigned int)offset;
      ret = filp->f_pos;
      break;
    case 1:
      if ((filp->f_pos + offset) > GLOBALMEM_SIZE) {
        ret = -EINVAL;
        break;
      }
      if ((filp->f_pos + offset) < 0) {
        ret = -EINVAL;
        break;
      }
      filp->f_pos += offset;
      ret = filp->f_pos;
      break;
    default:
      ret = -EINVAL;
      break;
  }
  return ret;
}
// 创建文件
static const struct file_operations golbalmem_fops = {
    .owner = THIS_MODULE,
    .read = global_read,
    .write = global_write,
    .unlocked_ioctl = global_unlocked_ioctl,
    .open = global_open,
    .release = global_release,  // 关闭
    .llseek = global_llseek,    // 用于定位设备文件位置。
};

static void globalmem_setup_cdev(struct global_dev *dev, int index) {
  int err, devno = MKDEV(globalmem_major, index);
  // 注册字符设备
  cdev_init(&dev->cdev, &golbalmem_fops);
  dev->cdev.owner = THIS_MODULE;
  dev->cdev.ops = &golbalmem_fops;
  err = cdev_add(&dev->cdev, devno, 1);
  if (err) printk(KERN_NOTICE "Error %d adding globalmem", err);
}

static int __int global_init(void) {
  int result;
  dev_t devno = MKDEV(globalmem_major, 0);
  if (globalmem_major) {
    result = register_chrdev_region(devno, 3, "wayneyaoglobalmem");
  } else {
    result = alloc_chrdev_region(&devno, 0, 3, "wayneyaoglobalmem");
    globalmem_major = MAJOR(devno);
  }
  if (result < 0) return result;

  // 为global_devp动态生成两个字符设备空间
  global_devp = kmalloc(3 * sizeof(struct global_dev), GFP_KERNEL);
  if (!global_devp) {
    result = -ENOMEM;
    goto fail_malloc;
  }
  memset(global_devp, 0, 3 * sizeof(struct global_dev));
  globalmem_setup_cdev(&global_devp[0], 0);
  globalmem_setup_cdev(&global_devp[1], 1);
  globalmem_setup_cdev(&global_devp[2], 2);
  return 0;
fail_malloc:
  unregister_chrdev_region(devno, 1);
  return result;
}
static void __exit global_exit(void) {
  // 删除字符社诶
  cdev_del(&global_devp[0].cdev);
  cdev_del(&global_devp[1].cdev);
  cdev_del(&global_devp[2].cdev);
  // 回收字符内存
  kfree(global_devp);
  // 取消注册
  unregister_chrdev_region(MKDEV(globalmem_major, 0), 3);
}

MODULE_AUTHOR("wayne");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("global test");
module_init(global_init);
module_exit(global_exit);
