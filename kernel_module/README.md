# 生成静态库

gcc -c -o yyjson.o -fPIC yyjson.c
gcc -shared -fPIC -o libyyjson.so yyjson.c
sudo ln -s /home/lucy/sambaFile/tcpclient/third/cjson/lib/libcjson.so /usr/lib


# khadas 内核
ARCH=arm64
uname -a
linux-headers-4.9.241  下载这个内核
linux-headers-4.9.241.tar.gz

# khadas vim3 论坛
https://docs.khadas.com/linux/vim3/Gpio.html

# 反编译dtb.img
dtc-I dtb -O dts dtb.img -o dtsi.txt

# dts->dtbo
https://copyfuture.com/blogs-details/20200523123334822lw0bqdwn90nfs7p
https://blog.51cto.com/u_15127599/3559373
dtc -I dts -O dtb -o example.dtb example.dts
make dtbs
cp kvim3_linux.dtb /boot/dtb-linux*/
su 
reboot

# vim3的基础半载驱动都在
/home/khadas/nfs/work/sdk/drivers/amlogic/watchdog/meson_wdt_v3.c

# lib头文件路径
  khadas /home/khadas/nfs/work/bs_/kernel_module/khadas/lib
  export LD_LIBRARY_PATH=/home/khadas/nfs/work/bs_/kernel_module/khadas/lib:$LD_LIBRARY_PATH
# 交叉编译
  opencv  --- third/opencv2/README.md opencv4


