#!/bin/bash
# 还原 Symbol 包
cd "$(dirname "$0")"
cat part_* > ../openGauss-Symbol-7.0.0-RC3-KylinV10-sw_64.tar.gz
echo "已还原: openGauss-Symbol-7.0.0-RC3-KylinV10-sw_64.tar.gz"
