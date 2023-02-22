#!/bin/bash
#
. ${PWD}/tools/utils.sh

project="EmbedSystem"
workPath=$(dirname $(readlink -f $0))
echo "当前目录：${workPath}"
cd ${workPath}


# ##############################################################################

buildPath=${workPath}/../install/build/${project} # 编译路径
outputPath=${workPath}/../install/output/${project} # 执行文件路径
targetPath=${workPath}/../command/application # 依赖库拷贝路径
infoln "Author : wayneyao  Priject:${project} Email:417838124@qq.com"
infoln "Now PATH:${workPath}"
infoln "BuildPath  :  ${buildPath}"
infoln "OutputPath :  ${outputPath}"
infoln "TargetPath :  ${targetPath}"


stepln "clean prebuild file:"
sudo rm {workPath}/../../app
rm -r ${outputPath}

if [ ! -d ${outputPath} ]; then
  mkdir -p ${outputPath}
fi

stepln "Go in to buildPath:"
cd ${buildPath}
pwd

# echo "安装  ( install )"
make -j install || exit


# ##############################################################################

if [ -z ${targetPath} ]; then
  echo "Undefin output path!"
  exit 1
fi

stepln "Aim Path:${targetPath}"

stepln "Copy out put file:"
cp -rf ${outputPath}/bin/app ${targetPath}/system/bin/app
cp -rf ${outputPath}/bin/app ${workPath}/

# stepln "Copy Library"
# if [ ! -d ${targetPath}/system/lib ]; then
#   mkdir -p ${targetPath}/system/lib
# else
#   rm -r ${targetPath}/system/lib/*
# fi

# cp -ar ${outputPath}/lib/* ${targetPath}/system/lib/

sync

successln "Make Project:( ${project} ) Success ( $0 )"

