#!/bin/bash
set -e
source /opt/petalinux/settings.sh
source /opt/Xilinx/SDK/2019.1/settings64.sh

BSP_NAME=i3ds-bsp-v3
BSP_ROOT=/opt/bsp/$BSP_NAME/build/tmp/sysroots/plnx_aarch64

mkdir -p build_petalinux
cd build_petalinux
cmake .. -DCMAKE_C_COMPILER="aarch64-linux-gnu-gcc" \
      -DCMAKE_CXX_COMPILER="aarch64-linux-gnu-c++" \
      -DCMAKE_FIND_ROOT_PATH="$BSP_ROOT" \
      -DBUILD_EMBEDDED=ON \
      -DBUILD_TOOLS=ON \
      -DBUILD_TESTS=OFF \
      -DBUILD_BINDINGS=OFF \
      -DGENERATE_ASN=OFF \
      -DNO_OPENCV=ON

make -j4
