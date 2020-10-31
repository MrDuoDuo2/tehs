# TEHS-The Easy HTTP Server
## 简单的HTTP服务器
TEHS是由C++开发的基于LINUX实现的简单HTTP服务器
***仅支持 Linux/Unix***

## 使用场景
- 当需要web服务器进行资源加载时使用
- 需要 `nohup`、`screen` 等运行
- 需要驻留后台

## 安装

```bash
pip install sppm
```

## 用法

    usage: cmake-build-debug-coverage/tehs --run|--stop|--list

    optional arguments:
    -h, --help            show this help message and exit
    --run                 启动程序
    --stop                等待程序正常退出
    --list                列出所有运行中的web服务器
