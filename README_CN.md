# DFRobot_RS20XU
- [English Version](./README.md)

这是一款支持多卫星系统，可多系统联合定位和单系统独立定位的GNSS定位模块。能快速获取经纬度、时间、高度等数据。多系统联合定位相比于传统单一的GPS定位，可见和可用卫星数目大幅增加，能够提高定位精度和定位速度，即使是在复杂城市环境中也能实现稳定的高精度定位。

Gravity: GNSS定位模块，具有I2C和UART两种数据输出类型，兼容Arduino、ESP32、树莓派等主流的主控设备。可用于车载导航、手持定位、物品追踪、气象站等室外定位场景。

![正反面svg效果图](/resources/images/TEL0157.jpg) 

## 产品链接(https://www.dfrobot.com.cn/goods-3652.html)

    SKU：TEL0157

## 目录

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性y)
* [历史](#历史)
* [创作者](#创作者)

## 概述

提供一个Arduino库给GNSS模块，此库具有以下功能：
  获取 gnss的数据
  获取 gnss的原始数据

## 库安装
这里提供两种使用本库的方法：<br>
1.打开Arduino IDE,在状态栏中的Tools--->Manager Libraries 搜索"DFRobot_RS20XU"并安装本库.<br>
2.首先下载库文件,将其粘贴到\Arduino\libraries目录中,然后打开examples文件夹并在该文件夹中运行演示.<br>

## 方法

```C++
/**
 * @fn getUTC
 * @brief 获取utc 标准时间
 * @return sTim_t 类型，表示返回的时分秒
 * @retval sTim_t.hour 时
 * @retval sTim_t.minute 分
 * @retval sTim_t.second 秒
 */
  sTim_t getUTC(void);

/**
 * @fn getDate
 * @brief 获取年月日等日期
 * @return sTim_t 类型，表示返回的年月日
 * @retval sTim_t.year 年
 * @retval sTim_t.month 月
 * @retval sTim_t.day 日
 */
  sTim_t getDate(void);

/**
 * @fn getLat
 * @brief 获取纬度
 * @return sLonLat_t 类型，表示返回的经纬度
 * @retval sLonLat_t.latDD   纬度 度（0-90）
 * @retval sLonLat_t.latMM   纬度 分后0-2位小数
 * @retval sLonLat_t.latMMMMM 纬度 分后2-7位小数
 * @retval sLonLat_t.latitude 包含7位小数的纬度值
 * @retval sLonLat_t.latDirection 纬度的方向
 */
  sLonLat_t getLat(void);

/**
 * @fn getLon
 * @brief 获取经度
 * @return sLonLat_t 类型，表示返回的经度
 * @retval sLonLat_t.lonDDD  经度 度（0-90）
 * @retval sLonLat_t.lonMM   经度 分后0-2位小数
 * @retval sLonLat_t.lonMMMMM 经度 分后2-7位小数
 * @retval sLonLat_t.lonitude 包含7位小数的经度值
 * @retval sLonLat_t.lonDirection 经度的方向
 */
  sLonLat_t getLon(void);

/**
 * @fn getNumSatUsed
 * @brief 获取使用的卫星数
 * @return uint8_t 类型，表示使用的卫星数
 */
  uint8_t getNumSatUsed(void);

/**
 * @fn getAlt
 * @brief 获取大地的高度
 * @return double 类型，表示大地的高度
 */
  double getAlt(void);

/**
 * @fn getSog
 * @brief 获取对地速度
 * @return speed 浮点型数据 （单位 节）
 */
  double getSog(void);

/**
 * @fn getCog
 * @brief 获取对地真航向
 * @return 浮点型数据 （单位 度）
 */
  double getCog(void);

/**
 * @fn setGnss
 * @brief 设置星系
 * @param mode
 * @n   eGPS              使用 gps
 * @n   eBeiDou           使用 beidou
 * @n   eGPS_BeiDou       使用 gps + beidou
 * @n   eGLONASS          使用 glonass
 * @n   eGPS_GLONASS      使用 gps + glonass
 * @n   eBeiDou_GLONASS   使用 beidou +glonass
 * @n   eGPS_BeiDou_GLONASS 使用 gps + beidou + glonass
 * @return NULL
 */
  void setGnss(eGnssMode_t mode);

/**
 * @fn transATMode
 * @brief 获取使用的星系模式
 * @return mode
 * @retval 1 使用 gps
 * @retval 2 使用 beidou
 * @retval 3 使用 gps + beidou
 * @retval 4 使用 glonass
 * @retval 5 使用 gps + glonass
 * @retval 6 使用 beidou +glonass
 * @retval 7 使用 gps + beidou + glonass
 */
  uint8_t transATMode(void);

/**
 * @fn getAllGnss
 * @brief 获取gnss的数据,回调接收
 * @return null
 */
  void getAllGnss(void);

/**
 * @fn enablePower
 * @brief 使能gnss的电源
 * @return null
 */
void enablePower(void);

/**
 * @fn disablePower
 * @brief 失能gnss的电源
 * @return null
 */
void disablePower(void);

/**
 * @fn setRgbOn
 * @brief 开启 rgb 灯
 * @return null
 */
void setRgbOn(void);

/**
 * @fn setRgbOn
 * @brief 关闭 rgb 灯
 * @return null
 */
void setRgbOff(void);

/**
 * @fn setCallback
 * @brief 设置回调函数类型
 * @param  * call 函数名
 * @return null
 */
  void setCallback(void (*call)(char *, uint8_t));
```

## 兼容性

MCU                | Work Well    |   Work Wrong    | Untested    | Remarks
------------------ | :----------: | :-------------: | :---------: | :----:
Arduino Uno        |      √       |                 |             |
Arduino MEGA2560   |      √       |                 |             |
Arduino Leonardo   |      √       |                 |             |
FireBeetle-ESP8266 |      √       |                 |             |
FireBeetle-ESP32   |      √       |                 |             |
FireBeetle-M0      |      √       |                 |             |
Micro:bit          |      √       | nonsupport uart |             |


## 历史
- 2022/8/15 - V0.0.1 版本
- 2022/10/26 - V1.0.0 版本

## 创作者

Written by ZhixinLiu(zhixin.liu@dfrobot.com), 2022. (Welcome to our website)