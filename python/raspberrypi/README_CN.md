# DFRobot_C4001
- [English Version](./README.md)

这是一款人体存在传感器，如PIR传感器、红外传感器和超声波传感器，具有动静都能检测的特点，并且其抗干扰能力相对较强，不易受到温度变化、环境光变化和环境噪声等因素的影响。不论人体是静坐、睡觉还是运动，传感器都能够快速、灵敏地检测到其存在。
例如，在公共智能厕所项目中，由于公共厕所环境相对复杂，容易受到环境噪声和温湿度变化的影响。然而，该款毫米波人体存在传感器可以在这种复杂的环境下实时监测公共厕所蹲位的使用情况，优化公共厕所的布局和资源分配，提高使用效率和用户体验。

![正反面svg效果图](../../resources/images/TEL0157.jpg)


## 产品链接(https://www.dfrobot.com.cn/goods-3115.html)

    SKU：SEN0609   SEN0610

## 目录

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性)
* [历史](#历史)
* [创作者](#创作者)

## 概述

提供一个python库给模块，此库具有以下功能：
  获取 物体是否存在
  获取 物体的速度，范围，物体能量强度

## 库安装
1. 下载库至树莓派，要使用这个库，首先要将库下载到Raspberry Pi，命令下载方法如下:

```
sudo git clone https://github.com/DFRobot/DFRobot_C4001
```

1. 打开并运行例程，要执行一个例程demo_x.py，请在命令行中输入python demo_x.py。例如，要执行 get_mm wavewave.py例程，你需要输入:

```
python  motion_range_velocity.py
或 
python2 motion_range_velocity.py
或 
python3 motion_range_velocity.py
```

## 方法

```python
  '''!
    @brief 初始化传感器
  '''
  def begin(self):

  '''!
    @brief 获取年月日等日期
    @return struct_utc_tim 类型，表示返回的年月日
  '''
  def get_date(self):

  '''!
    @brief 获取年月日等日期
    @return struct_utc_tim 类型，表示返回的时分秒
  '''
  def get_utc(self):

  '''!
    @brief 获取纬度
    @return struct_lat_lon 类型，表示返回的纬度
  '''
  def get_lat(self):

  '''!
    @brief 获取经度
    @return struct_lat_lon 类型，表示返回的经度
  '''
  def get_lon(self):

  '''!
    @brief 获取使用的卫星数
    @return 表示使用的卫星数
  '''
  def get_num_sta_used(self):

  '''!
    @brief 获取大地的高度
    @return double 类型，表示大地的高度
  '''
  def get_alt(self):

  '''!
    @brief 获取对地真航向
    @return 浮点型数据 （单位 度）
  '''
  def get_cog(self):

  '''!
    @brief 获取对地速度
    @return speed 浮点型数据 （单位 节）
  '''
  def get_sog(self):

  '''!
    @brief 获取使用的星系模式
    @return mode
    @retval 1 使用 gps
    @retval 2 使用 beidou
    @retval 3 使用 gps + beidou
    @retval 4 使用 glonass
    @retval 5 使用 gps + glonass
    @retval 6 使用 beidou +glonass
    @retval 7 使用 gps + beidou + glonass
  '''
  def get_mm wavewave_mode(self):

  '''!
    @brief 设置星系
    @param mode
    @n   GPS              使用 gps
    @n   BeiDou           使用 beidou
    @n   GPS_BeiDou       使用 gps + beidou
    @n   GLONASS          使用 glonass
    @n   GPS_GLONASS      使用 gps + glonass
    @n   BeiDou_GLONASS   使用 beidou +glonass
    @n   GPS_BeiDou_GLONASS 使用 gps + beidou + glonass
  '''
  def set_mm wavewave(self, mode):

  '''!
    @brief 获取mm wavewave的数据
  '''
  def get_all_mm wavewave(self):

  '''!
    @brief 使能mm wavewave 电源
  '''
  def enable_power(self):

  '''!
    @brief 失能mm wavewave 电源,此时mm wavewave不更新数据
  '''
  def disable_power(self):
  
  '''!
    @brief 开启rgb
  '''
  def rgb_on(self):
  
  '''!
    @brief 关闭rgb
  '''
  def rgb_off(self):
```

## 兼容性

* RaspberryPi Version

| Board        | 正常运行  | 运行失败   | 未测试    | 备注
| ------------ | :-------: | :--------: | :------: | :-----: |
| RaspberryPi2 |           |            |    √     |         |
| RaspberryPi3 |           |            |    √     |         |
| RaspberryPi4 |     √     |            |          |         |

* Python版本

| Python  | 正常运行  | 运行失败   | 未测试    | 备注
| ------- | :-------: | :--------: | :------: | :-----: |
| Python2 |     √     |            |          |         |
| Python3 |     √     |            |          |         |


## 历史

- 2024/02/26 - V1.0.0 版本

## 创作者

Written by ZhixinLiu(zhixin.liu@dfrobot.com), 2022. (Welcome to our website)