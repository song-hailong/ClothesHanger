# 基于AIOT的智能晾衣架

> 程序源码是好多好多年前写的，写的有点low，请大家多担待。

B站展示视频链接：[基于AIOT的智能晾衣架](https://www.bilibili.com/video/BV1ET4y1R7vF/?spm_id_from=333.999.0.0&vd_source=5d0dd24722835df97c44f4058436cf53) 。

Blinker开发环境搭建可根据文档 **[点灯科技开发环境搭建](https://www.songhailong.cn/2022/05/%E7%82%B9%E7%81%AF%E7%A7%91%E6%8A%80%E5%BC%80%E5%8F%91%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/)** 进行操作。

---

## 一：项目文件介绍

#### 1. **Hardware**

Hardware文件夹为项目电路的原理图和PCB文件，提供Altium Designer格式的源文件。

#### 2. **Software**

Software文件夹为项目的程序源码。

**STM32-MainProgram** 文件夹为STM32F103主控程序，项目除联网部分，均通过此主控进行控制。

**Blinker_Airer** 为ESP8266 ESP-12F的程序，通过Blinker进行联网，实现APP和语音控制。两个主控之间通过串口通信。

#### 3. AppInterface

AppInterface文件夹中提供了APP界面配置文件。

#### 4. Docs

相关的参考文件，包括环境搭建和芯片的Datasheet等。