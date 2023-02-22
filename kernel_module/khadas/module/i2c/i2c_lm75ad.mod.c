#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xcaefceab, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x8b9550fb, __VMLINUX_SYMBOL_STR(i2c_del_driver) },
	{ 0xa24393a7, __VMLINUX_SYMBOL_STR(i2c_register_driver) },
	{ 0x16e6d77f, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0xecd514ef, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x10b26a8f, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xe1ce0d18, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0xeae3dfd6, __VMLINUX_SYMBOL_STR(__const_udelay) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xd0c8d5ca, __VMLINUX_SYMBOL_STR(i2c_transfer) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x771c345d, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x589f256a, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x56fddb17, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x1fdc7df2, __VMLINUX_SYMBOL_STR(_mcount) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

