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
	{ 0xa050700e, __VMLINUX_SYMBOL_STR(driver_unregister) },
	{ 0x2bbc7361, __VMLINUX_SYMBOL_STR(__spi_register_driver) },
	{ 0x84bc974b, __VMLINUX_SYMBOL_STR(__arch_copy_from_user) },
	{ 0x88db9f48, __VMLINUX_SYMBOL_STR(__check_object_size) },
	{ 0xeae3dfd6, __VMLINUX_SYMBOL_STR(__const_udelay) },
	{ 0xe75fc210, __VMLINUX_SYMBOL_STR(spi_sync) },
	{ 0xdcb764ad, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xeb692dde, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x16e6d77f, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0xecd514ef, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x10b26a8f, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xe1ce0d18, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0xfda470a1, __VMLINUX_SYMBOL_STR(gpiod_direction_output_raw) },
	{ 0xc5517246, __VMLINUX_SYMBOL_STR(of_get_named_gpio_flags) },
	{ 0x4f019916, __VMLINUX_SYMBOL_STR(of_get_parent) },
	{ 0xe30fbf, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x8f0e4589, __VMLINUX_SYMBOL_STR(gpiod_set_raw_value) },
	{ 0xf434ca7d, __VMLINUX_SYMBOL_STR(gpiod_get_raw_value) },
	{ 0xbec87752, __VMLINUX_SYMBOL_STR(gpio_to_desc) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x56fddb17, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x589f256a, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x771c345d, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x1fdc7df2, __VMLINUX_SYMBOL_STR(_mcount) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

