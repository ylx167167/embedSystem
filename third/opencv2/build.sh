#!/bin/bash
set -e # Exit immediately if a command exits with a non-zero status.
# BuildDir=/home/7hisi/opencv-4.1.0/build
installDir=/home/khadas/nfs/work/bs_/third/opencv2
# BuildDir=/home/khadas/nfs/work/opencv
ToolChain=/usr/bin
# if [ ! -d "$BuildDir" ]; then
#   # Take action if $BuildDir doesn‘t exists. 
#   echo "create ${BuildDir}..."
#   mkdir -p ${BuildDir}
# fi
# cd ${BuildDir}

echo "building OpenCV4"
#   -D CMAKE_TOOLCHAIN_FILE=../platforms/linux/aarch64-gnu.toolchain.cmake
#   -D CMAKE_TOOLCHAIN_FILE=../platforms/linux/arm-gnueabi.toolchain.cmake
# cmake ../ -D CMAKE_BUILD_TYPE=RELEASE 
#   -D BUILD_SHARED_LIBS=ON 
#   -D CMAKE_FIND_ROOT_PATH=${BuildDir}/
#   -D CMAKE_TOOLCHAIN_FILE=../platforms/linux/aarch64-gnu.toolchain.cmake
#   -D CMAKE_C_COMPILER=${ToolChain}/aarch64-linux-gnu-gcc
#   -D CMAKE_CXX_COMPILER=${ToolChain}/aarch64-linux-gnu-g++
#   -D CMAKE_CXX_FLAGS="-mfloat-abi=softfp -mfpu=neon"
#   -D CMAKE_C_FLAGS="-mfloat-abi=softfp -mfpu=neon"
#   -D CMAKE_INSTALL_PREFIX=${installDir}/lib 
#   -D WITH_TBB=ON 
#   -D WITH_EIGEN=ON 
#   -D BUILD_ZLIB=ON 
#   -D BUILD_TIFF=ON 
#   -D BUILD_JASPER=ON 
#   -D BUILD_JPEG=ON 
#   -D BUILD_PNG=ON 
#   -D ENABLE_NEON=ON 
#   -D ENABLE_VFPV3=ON 
#   -D WITH_LIBV4L=ON 
#   -D BUILD_opencv_python=OFF
#   -D ENABLE_PRECOMPILED_HEADERS=OFF 
#   -D BUILD_EXAMPLES=OFF 
#   -D BUILD_TESTS=OFF 
#   -D BUILD_PERF_TESTS=OFF 
#   -D BUILD_WITH_DEBUG_INFO=OFF 
#   -D BUILD_DOCS=OFF 
#   -D WITH_OPENCL=OFF 
#   -D WITH_1394=OFF 
  
#   cmake ../
# cmake -D CMAKE_BUILD_TYPE=RELEASE -D BUILD_SHARED_LIBS=ON -D CMAKE_FIND_ROOT_PATH=${BuildDir}/ -D CMAKE_TOOLCHAIN_FILE=../platforms/linux/aarch64-gnu.toolchain.cmake -D CMAKE_C_COMPILER=${ToolChain}/aarch64-linux-gnu-gcc -D CMAKE_CXX_COMPILER=${ToolChain}/aarch64-linux-gnu-g++ -D CMAKE_INSTALL_PREFIX=${installDir}/lib -D WITH_TBB=ON -D WITH_EIGEN=ON -D BUILD_ZLIB=ON -D BUILD_TIFF=ON -D BUILD_JASPER=ON -D BUILD_JPEG=ON -D BUILD_PNG=ON -D ENABLE_NEON=ON -D ENABLE_VFPV3=ON -D WITH_LIBV4L=ON -D BUILD_opencv_python=OFF -D ENABLE_PRECOMPILED_HEADERS=OFF -D BUILD_EXAMPLES=OFF -D BUILD_TESTS=OFF -D BUILD_PERF_TESTS=OFF -D BUILD_WITH_DEBUG_INFO=OFF -D BUILD_DOCS=OFF -D WITH_OPENCL=OFF -D WITH_1394=OFF -D OPENCV_EXTRA_MODULES_PATH=/home/khadas/nfs/work/opencv_contrib/modules ..
# cmake -D CMAKE_BUILD_TYPE=RELEASE -D BUILD_SHARED_LIBS=ON -D CMAKE_TOOLCHAIN_FILE=../platforms/linux/aarch64-gnu.toolchain.cmake -D WITH_1394=ON -D WITH_TBB=ON -D WITH_EIGEN=ON -D BUILD_ZLIB=ON -D BUILD_TIFF=ON -D BUILD_JASPER=ON -D BUILD_JPEG=ON -D BUILD_PNG=ON -D ENABLE_PRECOMPILED_HEADERS=OFF -D OPENCV_EXTRA_MODULES_PATH=/home/khadas/nfs/work/opencv_contrib/modules ..
# make -j$(nproc)
cmake -D CMAKE_BUILD_TYPE=Release -D OPENCV_GENERATE_PKGCONFIG=YES -D WITH_1394=ON -D WITH_TBB=ON -D WITH_EIGEN=ON -D BUILD_ZLIB=ON -D BUILD_TIFF=ON -D BUILD_JASPER=ON -D BUILD_JPEG=ON -D BUILD_PNG=ON -D CMAKE_INSTALL_PREFIX=${installDir}/lib -D WITH_FFMPEG=ON -D OPENCV_EXTRA_MODULES_PATH=/home/khadas/nfs/work/opencv_contrib/modules ..
# cmake -DCMAKE_BUILD_TYPE=Release -DOPENCV_GENERATE_PKGCONFIG=ON -DCMAKE_INSTALL_PREFIX=/${installDir}/lib  ..