#!/bin/bash
#

. ${PWD}/tools/utils.sh
project="EmbedSystem"
workPath=$(dirname $(readlink -f $0))
buildPath=${workPath}/../install/build/${project}
outputPath=${workPath}/../install/output/${project}

infoln "Author : wayneyao  Priject:${project} Email:417838124@qq.com"
infoln "Now PATH:${workPath}"
infoln "BuildPath  :  ${buildPath}"
infoln "OutputPath :  ${outputPath}"

echo "Clear the temporary File"
rm -r "${buildPath}"
rm -r "${outputPath}"

if [ ! -d ${buildPath} ]; then
  mkdir -p ${buildPath}
fi
if [ ! -d ${outputPath} ]; then
  mkdir -p ${outputPath}
fi

echo "Go into BuildPath : ${buildPath}"
cd ${buildPath}

pwd
echo "Build Compile Env ( cmake )"
cmake ../../.. \
  -DBUILD_DEBUG=ON \
  -DGCC_DEBUG=ON \
  -DCMAKE_INSTALL_PREFIX="${outputPath}"
successln "Build Project:( ${project} ) Success"
