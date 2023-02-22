#!/bin/bash

workPath=$(cd $(dirname $0); pwd)
echo "当前目录：$workPath"
cd "$workPath/../install"

if [ -d "build" ]; then
  rm -rf "./build/"
  echo "删除 bulid 目录"
fi

if [ -d "output" ]; then
  rm -rf "output/"
  echo "删除 output 目录"
fi
