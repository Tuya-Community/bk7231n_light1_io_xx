# bk7231n_light1_io_xx

[English](./README.md) | [中文](./README_zh.md)

## 简介

这个工程使用在涂鸦模组（WIFI）二次开发的教程中的，通过这个简单的 demo 带领大家通过使用涂鸦提供的 SDK 在模组上开发一款智能产品，能够通过涂鸦智能 app 远程控制板子上的 led 灯。

[点我查看相关文档](https://developer.tuya.com/cn/docs/iot/SoC-redevelopment_Windows?id=Kauqrxxka0wv6)

### 硬件简介

实验中用到的引脚介绍如下，点击查看[CBU Nano板的详细介绍](https://developer.tuya.com/cn/docs/iot/tuya-sandwich-wifi-and-ble-soc-board-BK7231N?id=Kao72e6net3bs)。


| 丝印名称 | 芯片引脚 |                            备注                            |
| :------: | :------: | :--------------------------------------------------------: |
|    S2    |  GPIO_9  | 按键引脚，按下为低电平。涂鸦封装的引脚名称为`TY_GPIOA_9`。 |
|    D2    | GPIO_16  |  LED引脚，低电平点亮。涂鸦封装的引脚名称为`TY_GPIOA_16`。  |

在bk7231N SDK的apps目录下clone该项目，然后回到 SDK 的根目录即可编译该demo

<br>

## 如何使用这个 demo

使用这个 demo 前，请确保你已经搭建好开发环境。如果你还不知道如何搭建开发环境，可以[点击这里](https://developer.tuya.com/cn/docs/iot/SoC-redevelopment_Windows?id=Kauqrxxka0wv6)查看如何搭建开发环境。

![download&build](https://images.tuyacn.com/smart/shiliu_zone/SOC_Develop/download&build.gif)

<br>

## 相关文档

涂鸦Demo中心：https://developer.tuya.com/demo


<br>


## 技术支持

您可以通过以下方法获得涂鸦的支持:

- 开发者中心：https://developer.tuya.com
- 帮助中心: https://support.tuya.com/help
- 技术支持工单中心: [https://service.console.tuya.com](https://service.console.tuya.com/) 

<br>
