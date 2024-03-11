# DFRobot_C4001
- [English Version](./README.md)

这是一款人体存在传感器，如PIR传感器、红外传感器和超声波传感器，具有动静都能检测的特点，并且其抗干扰能力相对较强，不易受到温度变化、环境光变化和环境噪声等因素的影响。不论人体是静坐、睡觉还是运动，传感器都能够快速、灵敏地检测到其存在。
例如，在公共智能厕所项目中，由于公共厕所环境相对复杂，容易受到环境噪声和温湿度变化的影响。然而，该款毫米波人体存在传感器可以在这种复杂的环境下实时监测公共厕所蹲位的使用情况，优化公共厕所的布局和资源分配，提高使用效率和用户体验。

![正反面svg效果图](/resources/images/mmWaveLadar.jpg) 

## 产品链接(https://www.dfrobot.com.cn/goods-3115.html)

    SKU：SEN0609 SEN0610

## 目录

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性y)
* [历史](#历史)
* [创作者](#创作者)

## 概述

提供一个Arduino库，此库具有以下功能：
  获取 人体是否存在
  获取 物体的速度，范围，物体能量强度

## 库安装
这里提供两种使用本库的方法：<br>
1.打开Arduino IDE,在状态栏中的Tools--->Manager Libraries 搜索"DFRobot_C4001"并安装本库.<br>
2.首先下载库文件,将其粘贴到\Arduino\libraries目录中,然后打开examples文件夹并在该文件夹中运行演示.<br>

## 方法

```C++
  /**
   * @fn motionDetection
   * @brief 运动检测
   * @return true
   * @return false
   */
  bool motionDetection(void);

  /**
   * @fn setSensor
   * @brief 设置传感器模式
   * @param mode
   * @n  eStartSen        开始采集
   * @n  eStopSen         停止采集
   * @n  eResetSen        复位传感器
   * @n  eRecoverSen      恢复默认设置
   * @n  eSaveParams      保存配置
   * @n  eChangeMode      切换模式
   */
  void setSensor(eSetMode_t mode);
  
  /**
   * @fn setDelay
   * @brief 设置延时时间
   * @param trig 触发延时，单位0.01s，范围0~2s（0~200）
   * @param keep 维持检测超时，单位0.5s，范围2~1500秒（4~3000）
   * @return true 
   * @return false 
   */
  bool setDelay(uint8_t trig , uint16_t keep);

  /**
   * @fn getTrigDelay
   * @brief 获取物体的触发延时
   * @return uint8_t 
   */
  uint8_t getTrigDelay(void);

  /**
   * @fn getKeepTimerout
   * @brief 获取物体持续超时时间
   * @return  uint16_t 
   */
  uint16_t getKeepTimerout(void);

  /**
   * @fn getTrigRange
   * @brief 存在模式，获取触发距离，单位cm，范围2.4~20m（240~2000），实际生效的配置范围不超出检测距离的最大距离和最小距离。
   * @return uint16_t 
   */
  uint16_t getTrigRange(void);

  /**
   * @fn getMaxRange
   * @brief 存在模式，获取检测范围最大距离，单位cm，范围2.4~20m（240~2000）
   * @return  uint16_t 
   */
  uint16_t getMaxRange(void);

  /**
   * @fn getMinRange
   * @brief 存在模式，获取检测范围最小距离，单位cm，范围0.3~20m（30~2000），不超过MAX_RANGE，否则功能不正常。
   * @return uint16_t 
   */
  uint16_t getMinRange(void);

  /**
   * @fn setDetectionRange
   * @brief 设置检测范围
   * @param min 检测范围最小距离，单位cm，范围0.3~20m（30~2000），不超过MAX_RANGE，否则功能不正常。
   * @param max 检测范围最大距离，单位cm，范围2.4~20m（240~2000）
   * @param trig 触发距离，单位cm，范围2.4~20m（240~2000），实际生效的配置范围不超出检测距离的最大距离和最小距离。
   * @n     触发距离就是能触发无人到有人的距离，
   * @n     例如配置最远检测距离10米，触发距离的值是6米，无人后需要到6米内才显示有人，只处于6-10米间不会出发为有人
   * @return true or false
   */
  bool setDetectionRange(uint16_t min, uint16_t max);

  /**
   * @fn setTrigSensitivity
   * @brief 设置触发灵敏度，0~9
   * @param sensitivity 
   * @return true or false 
   */
  bool setTrigSensitivity(uint8_t sensitivity);

  /**
   * @fn setKeepSensitivity
   * @brief 设置维持灵敏度，0~9
   * @param sensitivity 
   * @return true or false 
   */
  bool setKeepSensitivity(uint8_t sensitivity);

  /**
   * @fn getTrigSensitivity
   * @brief 获取触发灵敏度
   * @return uint8_t 
   */
  uint8_t getTrigSensitivity(void);

  /**
   * @fn getKeepSensitivity
   * @brief 获取维持灵敏度
   * @return uint8_t 
   */
  uint8_t getKeepSensitivity(void);

  /**
   * @fn getStatus
   * @brief 获取模块当前状态
   * @return sSensorStatus_t 
   * @n     workStatus
   * @n       0 stop
   * @n       1 start
   * @n     workMode
   * @n       0 为存在检测
   * @n       1 为测速测距
   * @n     initStatus
   * @n       0 未初始化
   * @n       1 初始化完成
   */
  sSensorStatus_t getStatus(void);

  /**
   * @fn setIoPolaity
   * @brief 设置io口极性
   * @param value
   * @n     0：有目标时输出低电平，无目标时输出高电平
   * @n     1：有目标时输出高电平，无目标时输出低电平（默认状态）
   * @return true or false 
   */
  bool setIoPolaity(uint8_t value);

  /**
   * @fn getIoPolaity
   * @brief 获取io口极性状态
   * @return uint8_t 配置的I/O 口检测到目标后，引脚输出的信号电平
   */
  uint8_t getIoPolaity(void);

  /**
   * @fn setPwm
   * @brief 设置 pwm 周期
   * @param pwm1  未检测到目标时，OUT引脚输出信号的占空比，取值范围：0～100
   * @param pwm2  检测到目标后，OUT引脚输出信号的占空比，取值范围：0～100
   * @param timer  从pwm1 占空比渐变为pwm2 占空比的时间，取值范围：0～255，对应时间值 = timer*64ms
   * @n            如timer=20，占空比从pwm1渐变为pwm2需要 20*64ms=1.28s。
   * @return true or false 
   */
  bool setPwm(uint8_t pwm1 , uint8_t pwm2, uint8_t timer);

  /**
   * @fn getPwm
   * @brief 获取pwm 周期
   * @return sPwmData_t 
   * @retval pwm1  未检测到目标时，OUT引脚输出信号的占空比，取值范围：0～100
   * @retval pwm2  检测到目标后，OUT引脚输出信号的占空比，取值范围：0～100
   * @retval timer  从pwm1 占空比渐变为pwm2 占空比的时间，取值范围：0～255，对应时间值 = timer*64ms
   * @n      如timer=20，占空比从pwm1渐变为pwm2需要 20*64ms=1.28s。
   */
  sPwmData_t getPwm(void);

  /**
   * @fn setSensorMode
   * @brief 设置sensor 模式
   * @param mode 存在模式或者运动模式
   * @return true or false 
   */
  bool setSensorMode(eMode_t mode);

  /**
   * @fn getTargetNumber
   * @brief 获取速度模式下的目标数量
   * @return uint8_t 
   */
  uint8_t getTargetNumber(void);

  /**
   * @fn getTargetSpeed
   * @brief 获取速度模式下的目标速度
   * @return float 
   */
  float getTargetSpeed(void);

  /**
   * @fn getTargetRange
   * @brief 获取速度模式下目标范围
   * @return float
   */
  float getTargetRange(void);

  /**
   * @fn getTargetEnergy
   * @brief 获取速度模式下目标能量
   * @return uint32_t 
   */
  uint32_t getTargetEnergy(void);

  /**
   * @fn setDetectThres
   * @brief 设置检测的阈值
   * @param min 检测距离最小距离，单位厘米，范围0.3~20m(30~2000)，不能超过max，否则功能异常。
   * @param max 最大探测距离，单位厘米，范围2.4~20m (240~2000)
   * @param thres 目标检测阈值，无量纲单位0.1，范围0~6553.5 (0~65535)
   * @return true or false 
   */
  bool setDetectThres(uint16_t min, uint16_t max, uint16_t thres);

  /**
   * @fn getTMinRange
   * @brief 运动模式，获取检测的最小范围
   * @return uint16_t 
   */
  uint16_t getTMinRange(void);

  /**
   * @fn getTMaxRange
   * @brief 运动模式，获取检测的最大范围
   * @return uint16_t 
   */
  uint16_t getTMaxRange(void);

  /**
   * @fn getThresRange
   * @brief 运动模式，获取阈值配置的范围
   * @return uint16_t 
   */
  uint16_t getThresRange(void);

  /**
   * @fn setFrettingDetection
   * @brief 设置微动检测
   * @param sta 枚举类型
   */
  void setFrettingDetection(eSwitch_t sta);

  /**
   * @fn getFrettingDetection
   * @brief 获取微动检测的配置状态
   * @return eSwitch_t 
   */
  eSwitch_t getFrettingDetection(void);
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
- 2024/02/26 - V1.0.0 版本

## 创作者

Written by ZhixinLiu(zhixin.liu@dfrobot.com), 2024. (Welcome to our website)