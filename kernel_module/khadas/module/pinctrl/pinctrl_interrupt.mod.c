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
	{ 0xf5d8ce5c, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0x66015c77, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0xb8eec98b, __VMLINUX_SYMBOL_STR(del_timer) },
	{ 0x526c3a6c, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0x4f6cafaa, __VMLINUX_SYMBOL_STR(irq_of_parse_and_map) },
	{ 0xd4fbf73f, __VMLINUX_SYMBOL_STR(of_find_node_opts_by_path) },
	{ 0x3655f448, __VMLINUX_SYMBOL_STR(gpiod_to_irq) },
	{ 0x85825898, __VMLINUX_SYMBOL_STR(add_timer) },
	{ 0x8fdf772a, __VMLINUX_SYMBOL_STR(init_timer_key) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x94842f91, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x3d7702cc, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x12b02d8, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0xf721cc3, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x5af2a104, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xbcc15353, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0xd164af7b, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0xdcb764ad, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x84bc974b, __VMLINUX_SYMBOL_STR(__arch_copy_from_user) },
	{ 0x88db9f48, __VMLINUX_SYMBOL_STR(__check_object_size) },
	{ 0x5990bd16, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xfe990052, __VMLINUX_SYMBOL_STR(gpio_free) },
	{ 0x8f0e4589, __VMLINUX_SYMBOL_STR(gpiod_set_raw_value) },
	{ 0xfda470a1, __VMLINUX_SYMBOL_STR(gpiod_direction_output_raw) },
	{ 0x47229b5c, __VMLINUX_SYMBOL_STR(gpio_request) },
	{ 0xf434ca7d, __VMLINUX_SYMBOL_STR(gpiod_get_raw_value) },
	{ 0xbec87752, __VMLINUX_SYMBOL_STR(gpio_to_desc) },
	{ 0xc5517246, __VMLINUX_SYMBOL_STR(of_get_named_gpio_flags) },
	{ 0x9df232d3, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x1fdc7df2, __VMLINUX_SYMBOL_STR(_mcount) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

