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

## 使用

### 代码

```python
import sppm

sppm.sppm_start(foo)
```

更多细节，请查看 `examples/example.py` 以及 `examples/example_working_lock.py`

### 管理

更多使用方法，请执行 `python3 examples/example.py -h` 查看帮助信息。

#### 启动
```bash
python3 examples/example.py --start
```

    2019-12-01 17:45:07 15048 [INFO] **** 按Ctrl+C可以终止运行 ****
    2019-12-01 17:45:07 15049 [INFO] Run 0 time(s) task->foo.
    2019-12-01 17:45:17 15049 [INFO] Run 1 time(s) task->foo.
    2019-12-01 17:45:27 15049 [INFO] Run 2 time(s) task->foo.
    2019-12-01 17:45:37 15049 [INFO] Run 3 time(s) task->foo.
    ^C2019-12-01 17:45:47 15049 [INFO] Run 4 time(s) task->foo.
    执行exit_callback函数


指定日志等级：

```bash
python3 examples/example.py --start -l 5
```

#### 查看状态
```bash
python3 examples/example.py --status
```

    pid                  : 16728
    ppid                 : 16727
    alive                : true
    uptime               : 152 second(s)
    human readable uptime: 2 minute(s), 32 second(s)
    create time          : 2019-12-01 18:32:30.300000
    active               : false
    last active time     : 2019-12-01 18:32:30.696024


#### 停止
```bash
python3 examples/example.py --stop
```

### 运行多个程序

```bash
python3 examples/example.py --start

SPPM_ENV=examples/.sppm_env_working_lock python examples/example_working_lock.py --start
```

    $ python3 examples/example.py --status
    pid                  : 17404
    ppid                 : 17403
    alive                : true
    uptime               : 48 second(s)
    human readable uptime: 48 second(s)
    create time          : 2019-12-01 18:49:47.880000
    active               : false
    last active time     : 2019-12-01 18:49:48.273476


    $ SPPM_ENV=examples/.sppm_env_working_lock python examples/example_working_lock.py --status
    pid                  : 17397
    ppid                 : 17396
    alive                : true
    uptime               : 40 second(s)
    human readable uptime: 40 second(s)
    create time          : 2019-12-01 18:49:25.690000
    active               : true
    last active time     : 2019-12-01 18:50:06.127305


### 配置文件

默认情况下，程序自动从环境变量 `SPPM_ENV` 加载 `Python` 文件目录下的 `.sppm_env`。

    $ cat examples/.sppm_env
    pid=/tmp/example.pid
    child_pid=/tmp/example_child.pid
    lock=/tmp/example.lock
    log=/tmp/example.log

运行多个程序时，每个程序必须单独配置环境变量 `SPPM_ENV` 指向不同的配置文件。