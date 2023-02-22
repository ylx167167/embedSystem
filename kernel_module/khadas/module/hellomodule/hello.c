#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("kent");
struct meson_i2c_data {
  unsigned char delay_ajust;
  unsigned char div_factor;
};

static const struct meson_i2c_data i2c_meson6_data = {
    .div_factor = 4,
    .delay_ajust = 15,
};

static const struct meson_i2c_data i2c_meson8b_data = {
    .div_factor = 4,
    .delay_ajust = 15,
};

static const struct meson_i2c_data i2c_gx_data = {
    .div_factor = 4,
    .delay_ajust = 15,
};

static const struct meson_i2c_data i2c_txl_data = {
    .div_factor = 4,
    .delay_ajust = 15,
};

static const struct meson_i2c_data i2c_axg_data = {
    .div_factor = 3,
    .delay_ajust = 15,
};

static const struct meson_i2c_data i2c_txlx_data = {
    .div_factor = 3,
    .delay_ajust = 15,
};

static const struct meson_i2c_data i2c_g12a_data = {
    .div_factor = 3,
    .delay_ajust = 15,
};

/* for the stable i2c controller, div_factor=3*/
static const struct meson_i2c_data i2c_meson_data = {
    .div_factor = 3,
    .delay_ajust = 15,
};

static const struct of_device_id meson_i2c_match[] = {
    {
        .compatible = "cw201x",
    },
    // {.compatible = "amlogic,meson8b-i2c", .data = &i2c_meson8b_data},
    // {.compatible = "amlogic,meson-gx-i2c", .data = &i2c_gx_data},
    // {.compatible = "amlogic,meson-axg-i2c", .data = &i2c_axg_data},
    // {.compatible = "amlogic,meson-txl-i2c", .data = &i2c_txl_data},
    // {.compatible = "amlogic,meson-txlx-i2c", .data = &i2c_txlx_data},
    // {.compatible = "amlogic,meson-g12a-i2c", .data = &i2c_g12a_data},
    // {.compatible = "amlogic,meson-g12b-i2c", .data = &i2c_g12a_data},
    // {.compatible = "amlogic,meson-i2c", .data = &i2c_meson_data},
    {},
};

static int __init hello_init(void) {
  struct device_node *nd = NULL;
  struct device_node *nd_parient = NULL;
  // struct property *pp = NULL;
  // nd = of_find_node_by_name(NULL, "audiobus");
  // nd = of_find_node_by_type(NULL, "cpucore");
  // nd = of_find_compatible_node(NULL, NULL, "amlogic,pinctrltest");
  nd = of_find_matching_node(NULL, meson_i2c_match);
  if (nd == NULL) {
    printk(KERN_ALERT "OF_FIND_NODE_BY_NAME!\n");
    /* code */
  } else {
    nd_parient = nd->parent;
    printk(KERN_ALERT "%s \r\n", nd->name);
    printk(KERN_ALERT "%s \r\n", nd_parient->full_name);
    // pp = of_find_property(nd, "default-state", NULL);
    // printk(KERN_ALERT "property %s \r\n", (char *)pp->value);
    printk(KERN_ALERT "TYPE, world - this is the kernel speaking!\n");
  }
  return 0;
}

static void __exit hello_exit(void) {
  printk(KERN_ALERT "short is the life of a kernel module!\n");
}

module_init(hello_init);
module_exit(hello_exit);