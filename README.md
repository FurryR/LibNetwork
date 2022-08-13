# LibNetwork

### 这是什么？

一位朋友（[xiaokang00010](https://github.com/xiaokang00010)）写了[LibNetworking](https://github.com/XtherDevTeam/LibNetworking)，号称是POSIX socket的包装，但是我觉得实现不好。于是我写了一个多线程、基于事件的socket库，带HTTP支持。

### 如何使用？

请读源码。它并不难。附有example完成了几乎所有API的示例。

### 我该怎么编译？

在Linux/MacOS（UNIX系）下：

```bash
g++ [code] -lpthread
```

在Windows下：

```powershell
g++ [code] -lws2_32
```

### 版权声明

本项目是依照LibNetworking完全重写的。原项目使用MIT开源协议，故本项目也使用MIT开源协议。
