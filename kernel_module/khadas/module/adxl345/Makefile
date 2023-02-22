ifneq ($(KERNELRELEASE),)

obj-m := adxl345.o
# obj-m := i2c_adxl345.o
else
PWD :=$(shell pwd)
KDIR :=$(PWD)/../../../../../../env/khadas_env/usr/src/linux-headers-4.9.241
KO_DIR :=$(PWD)/../../ko
all:
	make -C $(KDIR) M=$(PWD) modules ARCH=arm64 CROSS_COMPILE= 
	cp ./*.ko $(KO_DIR)
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers  modul*

endif
