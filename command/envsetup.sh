#!/bin/bash

# 选择系统内核 khadas 或者 rpi

workPath=$(cd $(dirname $0); pwd)

kernel_version="linux-headers-4.9.241"
TOP_DIR=$workPath/../../
# cd $(TOP_DIR)

if [ ! -d $kernel_version ]; then
tar -zxvf $workPath/../kernel_module/kernel/khadas/$kernel_version.tar.gz -C $workPath/../../ 
fi

#生成
if [ ! -d $kernel_version ]; then
mkdir -p $workPath/bash_func
fi



# 集成脚本函数  

