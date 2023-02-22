# 交叉编译
  opencv
  git：https://github.com/opencv/opencv
  method：https://blog.csdn.net/xdonx/article/details/38871659
  cd build
  cmake -DENABLE_VFPV3=ON -DBUILD_SHARED_LIBS=OFF cmake ..
  khadas 
    ~~~
    sudo apt-get install ccache 
    sudo apt-get install libgtk-3-dev
    sudo apt-get install libgtk2.0-dev
    sudo apt-get install libavresample-dev
    sudo apt install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
    sudo apt-get install libdc1394-22-dev
    sudo apt-get install libjpeg-dev
    sudo apt-get install libtiff-dev
    sudo apt-get install libopenblas-dev
    sudo apt-get install libavcodec-dev
    sudo apt-get install libavformat-dev
    sudo apt-get install libswscale-dev
    sudo apt-get install libtbb-dev
    sudo apt install ffmpeg
    sudo apt install git gcc g++ ffmpeg cmake make python3-dev python3-numpy libavcodec-dev libavformat-dev libswscale-dev libgstreamer-plugins-base1.0-dev libgstreamer1.0-dev libgtk-3-dev libpng-dev libjpeg-dev libopenexr-dev libtiff-dev libwebp-dev
    export LD_LIBRARY_PATH=/home/khadas/nfs/work/opencv/build/lib:$LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=/home/khadas/nfs/work/opencv/av:$LD_LIBRARY_PATH
    ffmpeg :https://blog.csdn.net/xiaojun111111/article/details/72865172
    /usr/lib/aarch64-linux-gnu/pkgconfig
    export LD_LIBRARY_PATH=/home/khadas/nfs/work/opencv/build/lib:$LD_LIBRARY_PATH
    export PKG_CONFIG_PATH=/usr/lib/aarch64-linux-gnu/pkgconfig:$PKG_CONFIG_PATH
    export PKG_CONFIG_PATH=/usr/lib/pkgconfig

    
    pkg-config --flags libavcodec //查看PKG_CONFIG有没有更改
交叉编译后自动生成
├── build.sh 编译脚本
├── bin
├── include
├── lib
└── share

打开视频文件实验失败一
OpenCV | GStreamer warning: your GStreamer installation is missing a required plugin

sudo apt-get install gstreamer1.0-libav gstreamer1.0-opencv