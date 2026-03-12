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

---

## 快速安装（推荐）

本仓库 `output/` 目录已提供编译好的 sw_64 安装包，**无需自行编译**，直接下载安装包即可使用。

### 第一步：下载安装包

从本仓库 `output/` 目录下载以下文件：

| 文件 | 说明 | 大小 |
|------|------|------|
| `openGauss-Server-7.0.0-RC3-KylinV10-sw_64.tar.bz2` | 数据库主程序 | ~50MB |
| `openGauss-Libpq-7.0.0-RC3-KylinV10-sw_64.tar.gz` | 客户端库 | ~6.5MB |
| `openGauss-Tools-7.0.0-RC3-KylinV10-sw_64.tar.gz` | 管理工具 | ~9.3MB |
| `openGauss-Symbol-7.0.0-RC3-KylinV10-sw_64.tar.gz.split/` | 调试符号包（拆分） | ~492MB |

> **注意：** Symbol 包因超过 GitHub 100MB 限制，已拆分存放。如需还原，进入 `split` 目录执行 `./restore.sh`。

```bash
# 克隆仓库获取安装包
git clone https://github.com/NoNamesJavaDog/openGauss-for-sw64.git
cd openGauss-for-sw64/output

# 还原 Symbol 包（可选，调试时才需要）
cd openGauss-Symbol-7.0.0-RC3-KylinV10-sw_64.tar.gz.split
./restore.sh
cd ..
```

### 第二步：准备系统环境

```bash
# 创建数据库用户和用户组
groupadd dbgroup
useradd -g dbgroup omm
echo "omm:YourPassword@123" | chpasswd

# 创建安装目录
mkdir -p /opt/openGauss
chown omm:dbgroup /opt/openGauss

# 配置系统参数（root 执行）
cat >> /etc/sysctl.conf << 'EOF'
kernel.shmmax = 21474836480
kernel.shmall = 5242880
kernel.shmmni = 4096
kernel.sem = 250 6400000 1000 25600
net.ipv4.tcp_keepalive_time = 30
net.ipv4.tcp_keepalive_intvl = 5
net.ipv4.tcp_keepalive_probes = 5
net.ipv4.tcp_fin_timeout = 60
net.ipv4.tcp_tw_reuse = 1
net.core.somaxconn = 65535
vm.overcommit_memory = 0
vm.overcommit_ratio = 90
EOF
sysctl -p

# 设置资源限制
cat >> /etc/security/limits.conf << 'EOF'
omm   soft   nofile   1000000
omm   hard   nofile   1000000
omm   soft   nproc    60000
omm   hard   nproc    60000
omm   soft   stack    32768
omm   hard   stack    32768
EOF

# 关闭透明大页
echo never > /sys/kernel/mm/transparent_hugepage/enabled
echo never > /sys/kernel/mm/transparent_hugepage/defrag
```

### 第三步：单机安装（simpleInstall）

适用于快速体验或开发测试环境。

```bash
# 解压安装包
cd /opt/openGauss
tar -xjf /path/to/openGauss-Server-7.0.0-RC3-KylinV10-sw_64.tar.bz2

# 执行一键安装（以 root 执行）
cd simpleInstall
sh install.sh -w "YourPassword@123" -p 15000
# -w  数据库密码（需包含大小写字母、数字、特殊字符）
# -p  数据库端口（默认 5432）

# 验证安装
su - omm
gsql -d postgres -p 15000 -c "SELECT version();"
```

安装成功后的目录结构：

```
/opt/openGauss/
├── install/          # 数据库程序文件
│   ├── bin/          # 可执行文件（gsql、gs_ctl 等）
│   └── lib/          # 动态库
└── data/             # 数据目录
    ├── postgresql.conf
    └── pg_hba.conf
```

### 第四步：集群安装（一主两备，生产环境推荐）

适用于生产环境，需要至少 3 台 sw_64 服务器。

#### 4.1 所有节点执行

```bash
# 配置主机名（每台节点对应修改）
hostnamectl set-hostname gauss1   # 主节点
hostnamectl set-hostname gauss2   # 备节点1
hostnamectl set-hostname gauss3   # 备节点2

# 配置 /etc/hosts
cat >> /etc/hosts << 'EOF'
10.x.x.1 gauss1
10.x.x.2 gauss2
10.x.x.3 gauss3
EOF

# 解压安装包到所有节点
mkdir -p /opt/openGauss
tar -xjf openGauss-Server-7.0.0-RC3-KylinV10-sw_64.tar.bz2 -C /opt/openGauss
```

#### 4.2 编写集群配置文件

在主节点创建 `cluster_config.xml`：

```xml
<?xml version="1.0" encoding="UTF-8"?>
<ROOT>
  <CLUSTER>
    <PARAM name="clusterName" value="openGauss_sw64"/>
    <PARAM name="nodeNames" value="gauss1,gauss2,gauss3"/>
    <PARAM name="backIp1s" value="10.x.x.1,10.x.x.2,10.x.x.3"/>
    <PARAM name="gaussdbAppPath" value="/opt/openGauss/install"/>
    <PARAM name="gaussdbLogPath" value="/opt/openGauss/log"/>
    <PARAM name="gaussdbToolPath" value="/opt/openGauss/tools"/>
    <PARAM name="corePath" value="/opt/openGauss/corefile"/>
    <PARAM name="clusterType" value="single-inst"/>
  </CLUSTER>

  <DEVICELIST>
    <DEVICE sn="gauss1">
      <PARAM name="name" value="gauss1"/>
      <PARAM name="azName" value="AZ1"/>
      <PARAM name="azPriority" value="1"/>
      <PARAM name="backIp1" value="10.x.x.1"/>
      <PARAM name="sshIp1" value="10.x.x.1"/>
      <PARAM name="dataNum" value="1"/>
      <PARAM name="dataPortBase" value="15000"/>
      <PARAM name="dataPath1" value="/opt/openGauss/data/dn"/>
      <PARAM name="instanceType" value="Primary"/>
    </DEVICE>

    <DEVICE sn="gauss2">
      <PARAM name="name" value="gauss2"/>
      <PARAM name="azName" value="AZ1"/>
      <PARAM name="azPriority" value="1"/>
      <PARAM name="backIp1" value="10.x.x.2"/>
      <PARAM name="sshIp1" value="10.x.x.2"/>
      <PARAM name="dataNum" value="1"/>
      <PARAM name="dataPortBase" value="15000"/>
      <PARAM name="dataPath1" value="/opt/openGauss/data/dn"/>
      <PARAM name="instanceType" value="Standby"/>
    </DEVICE>

    <DEVICE sn="gauss3">
      <PARAM name="name" value="gauss3"/>
      <PARAM name="azName" value="AZ1"/>
      <PARAM name="azPriority" value="1"/>
      <PARAM name="backIp1" value="10.x.x.3"/>
      <PARAM name="sshIp1" value="10.x.x.3"/>
      <PARAM name="dataNum" value="1"/>
      <PARAM name="dataPortBase" value="15000"/>
      <PARAM name="dataPath1" value="/opt/openGauss/data/dn"/>
      <PARAM name="instanceType" value="Standby"/>
    </DEVICE>
  </DEVICELIST>
</ROOT>
```

#### 4.3 执行预安装和安装

```bash
# 在主节点执行预安装（root）
cd /opt/openGauss/install/script
./gs_preinstall -U omm -G dbgroup -X /path/to/cluster_config.xml

# 切换到 omm 用户执行安装
su - omm
gs_install -X /path/to/cluster_config.xml

# 验证集群状态
gs_om -t status --detail
```

### 第五步：验证安装

```bash
su - omm

# 连接数据库
gsql -d postgres -p 15000

# 查看版本
SELECT version();

# 查看集群状态（集群部署）
\q
gs_om -t status --detail

# 查看主备同步状态
gsql -d postgres -p 15000 -c "SELECT * FROM pg_stat_replication;"
```

### 常见问题

**Q: 启动失败，提示 `could not open shared memory segment`**
```bash
# 检查共享内存配置
sysctl kernel.shmmax kernel.shmall
# 调高 shmmax 值
sysctl -w kernel.shmmax=21474836480
```

**Q: 连接失败，提示 `pg_hba.conf` 拒绝连接**
```bash
# 编辑认证配置
vi /opt/openGauss/data/postgresql.conf   # 确认 listen_addresses = '*'
vi /opt/openGauss/data/pg_hba.conf       # 添加允许的客户端 IP
# 重载配置
gs_ctl reload -D /opt/openGauss/data
```

**Q: 忘记密码**
```bash
# 临时允许无密码登录
echo "local all omm trust" >> /opt/openGauss/data/pg_hba.conf
gs_ctl reload -D /opt/openGauss/data
gsql -d postgres -p 15000 -c "ALTER USER omm PASSWORD 'NewPassword@123';"
# 恢复 pg_hba.conf
```

---

## 从源码编译

如果需要自行编译，请参考以下步骤。

### 1. 准备第三方依赖

第三方依赖库已适配 sw_64 架构，请从以下仓库获取：

> **三方包仓库：** [openGauss-for-sw64-third_party](https://github.com/NoNamesJavaDog/openGauss-for-sw64-third_party)

```bash
# 克隆三方包（注意：大文件已拆分，需执行 restore.sh 还原）
git clone https://github.com/NoNamesJavaDog/openGauss-for-sw64-third_party.git /root/openGauss-third_party

# 还原拆分的大文件
cd /root/openGauss-third_party
find . -name "restore.sh" -exec sh {} \;
```

### 2. 编译 openGauss Server

```bash
git clone https://github.com/NoNamesJavaDog/openGauss-for-sw64.git /root/openGauss-server
cd /root/openGauss-server

export GAUSSHOME=/root/mppdb_temp_install
export GCC_PATH=/root/openGauss-third_party/output/buildtools/gcc10.3
export CC=$GCC_PATH/gcc/bin/gcc
export CXX=$GCC_PATH/gcc/bin/g++
export LD_LIBRARY_PATH=$GCC_PATH/gcc/lib64:$GCC_PATH/gcc/lib64/libstdc++.lv1:$LD_LIBRARY_PATH
export JAVA_HOME=$GCC_PATH/../../platform/jdk/jdk

./configure --gcc-version=10.3.0 --prefix=$GAUSSHOME --3rd=/root/openGauss-third_party/output \
    --enable-thread-safety --without-readline --without-zlib

make -j$(nproc)
make install -j$(nproc)
```

### 3. 打包

```bash
./build.sh -m release -3rd /root/openGauss-third_party/output -pkg
# 安装包输出至 output/ 目录
```

---

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

- [README_ORIGINAL.md](README_ORIGINAL.md)（中文）
- [README_ORIGINAL.en.md](README_ORIGINAL.en.md)（English）

## 相关仓库

| 仓库 | 说明 |
|------|------|
| [openGauss-for-sw64](https://github.com/NoNamesJavaDog/openGauss-for-sw64) | 数据库源码（本仓库） |
| [openGauss-for-sw64-third_party](https://github.com/NoNamesJavaDog/openGauss-for-sw64-third_party) | 第三方依赖库 |

## 许可证

与 openGauss 相同，采用 [MulanPSL-2.0](http://license.coscl.org.cn/MulanPSL2) 许可证。
