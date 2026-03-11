# openGauss for sw_64（申威）架构

本仓库是 [openGauss](https://opengauss.org) 数据库在 **sw_64（申威/Sunway）** 架构上的适配版本，基于 openGauss 7.0.0-RC3 源码，在 **麒麟 Kylin V10** 操作系统上完成编译和功能验证。

## 项目简介

openGauss 官方支持 x86_64 和 aarch64 架构，尚未原生支持申威 sw_64 平台。本项目完成了 openGauss 在 sw_64 架构上的全量源码编译适配，解决了 52 个编译错误，涉及平台识别、原子操作、SIMD 指令、CRC 校验、LLVM 适配、第三方库移植等方面，并通过 46 项功能测试验证了数据库核心功能的完整性。

### 适配环境

| 项目 | 信息 |
|------|------|
| 数据库版本 | openGauss 7.0.0-RC3 |
| CPU 架构 | sw_64（申威/Sunway） |
| 操作系统 | 麒麟 Kylin V10 |
| 部署方式 | 一主两备（含 CM） |
| 编译器 | GCC 10.3.1 |

## 编译指南

### 1. 准备第三方依赖

第三方依赖库已适配 sw_64 架构，请从以下仓库获取：

> **三方包仓库：** [openGauss-for-sw64-third_party](https://github.com/NoNamesJavaDog/openGauss-for-sw64-third_party)

```bash
# 克隆三方包
git clone git@github.com:NoNamesJavaDog/openGauss-for-sw64-third_party.git /root/openGauss-third_party

# 编译三方包（如需重新编译）
cd /root/openGauss-third_party
python3 build/build_all.py -m all
```

### 2. 编译 openGauss Server

```bash
# 克隆源码
git clone git@github.com:NoNamesJavaDog/openGauss-for-sw64.git /root/openGauss-server
cd /root/openGauss-server

# 设置环境变量
export GAUSSHOME=/root/mppdb_temp_install
export GCC_PATH=/root/openGauss-third_party/output/buildtools/gcc10.3
export CC=$GCC_PATH/gcc/bin/gcc
export CXX=$GCC_PATH/gcc/bin/g++
export LD_LIBRARY_PATH=$GCC_PATH/gcc/lib64:$GCC_PATH/gcc/lib64/libstdc++.lv1:$LD_LIBRARY_PATH
export JAVA_HOME=$GCC_PATH/../../platform/jdk/jdk

# 执行编译
./configure --gcc-version=10.3.0 --prefix=$GAUSSHOME --3rd=/root/openGauss-third_party/output \
    --enable-thread-safety --without-readline --without-zlib

make -j$(nproc)
make install -j$(nproc)
```

### 3. 编译安装包

```bash
# 生成 sw_64 安装包
./build.sh -m release -3rd /root/openGauss-third_party/output -pkg

# 安装包输出目录
ls output/
# openGauss-Server-7.0.0-RC3-KylinV10-sw_64.tar.bz2
# openGauss-Libpq-7.0.0-RC3-KylinV10-sw_64.tar.gz
# openGauss-Tools-7.0.0-RC3-KylinV10-sw_64.tar.gz
# openGauss-Symbol-7.0.0-RC3-KylinV10-sw_64.tar.gz
```

## 安装部署

### 单机安装

```bash
# 解压安装包
mkdir -p /opt/openGauss && cd /opt/openGauss
tar -xjf openGauss-Server-7.0.0-RC3-KylinV10-sw_64.tar.bz2

# 使用 simpleInstall 快速安装
cd simpleInstall
sh install.sh -w "YourPassword@123" -p 15000
```

### 集群部署（一主两备）

集群部署需要使用 OM 工具配置 XML 文件后执行预安装和安装，详见安装文档。

```bash
# 预安装
./gs_preinstall -U omm -G dbgroup -X cluster_config.xml

# 安装
gs_install -X cluster_config.xml

# 验证集群状态
gs_om -t status --detail
```

## 预编译安装包

本仓库 `output/` 目录已包含编译好的 sw_64 安装包，可直接使用，无需重新编译。

## 受影响的功能说明

由于 sw_64 架构的特殊性，以下 9 项能力采用了替代方案，可能对部分功能有影响：

| 组件 | 替代方案 | 影响说明 |
|------|----------|----------|
| **SIMD 向量化** | 标量运算回退 | 列存储查询、向量化执行器性能下降，功能正常 |
| **CRC32C 校验** | 软件实现替代硬件指令 | WAL 日志校验、数据页校验性能下降，功能正常 |
| **LLVM JIT** | 使用系统 LLVM 13 | 表达式 JIT 编译可用，版本差异可能影响部分优化 |
| **onnxruntime** | C 桩函数替代 | AI 模型推理功能（DB4AI）不可用 |
| **XGBoost** | 缺失动态库 | 机器学习预测功能不可用 |
| **text-splitter** | Rust 交叉编译 | 文本分词功能可能受限 |
| **tokenizers** | Rust 交叉编译 | NLP tokenizer 功能可能受限 |
| **cryptography** | 系统 3.3.1 版本 | Python 加密功能可用，部分新特性缺失 |
| **nghttp2** | 禁用相关工具 | HTTP/2 相关管理工具不可用，数据库核心不受影响 |

> **总结：** 数据库核心功能（SQL 执行、事务、存储、主备复制等）均正常可用。受影响的主要是 AI/ML 扩展功能和部分性能优化，不影响日常使用。已通过 46 项功能测试全部 PASS 验证。

## 原始文档

openGauss 官方原始 README 文档请参阅：

- [README_ORIGINAL.md](README_ORIGINAL.md)（中文）
- [README_ORIGINAL.en.md](README_ORIGINAL.en.md)（English）

## 相关仓库

| 仓库 | 说明 |
|------|------|
| [openGauss-for-sw64](https://github.com/NoNamesJavaDog/openGauss-for-sw64) | 数据库源码（本仓库） |
| [openGauss-for-sw64-third_party](https://github.com/NoNamesJavaDog/openGauss-for-sw64-third_party) | 第三方依赖库 |

## 许可证

与 openGauss 相同，采用 [MulanPSL-2.0](http://license.coscl.org.cn/MulanPSL2) 许可证。
