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
   * @brief motionDetection
   *         运动检测
   * @return true
   * @return false
   */
  bool motionDetection(void);

  /**
   * @brief Set the Sensor object
   * 
   * @param mode
   *   eStartSen        开始采集
   *   eStopSen         停止采集
   *   eResetSen        复位传感器
   *   eRecoverSen      恢复默认设置
   *   eSaveParams      保存配置
   *   eChangeMode      切换模式
   */
  void setSensor(eSetMode_t mode);
  
  /**
   * @brief Set the Delay object
   * 
   * @param trig 触发延时，单位0.01s，范围0~2s（0~200）
   * @param keep 维持检测超时，单位0.5s，范围2~1500秒（4~3000）
   * @return true 
   * @return false 
   */
  bool setDelay(uint8_t trig , uint16_t keep);

  /**
   * @brief Get the Trig Delay object
   *        获取物体的触发延时
   * @return uint8_t 
   */
  uint8_t getTrigDelay(void);

  /**
   * @brief 获取物体持续超时时间
   * 
   * @return  uint16_t 
   */
  uint16_t getKeepTimerout(void);

  /**
   * @brief Get the Trig Range object
   *        获取触发距离，单位cm，范围2.4~20m（240~2000），实际生效的配置范围不超出检测距离的最大距离和最小距离。
   * @return uint16_t 
   */
  uint16_t getTrigRange(void);

  /**
   * @brief Get the Max Range object
   *        获取检测范围最大距离，单位cm，范围2.4~20m（240~2000）
   * @return  uint16_t 
   */
  uint16_t getMaxRange(void);

  /**
   * @brief Get the Min Range object
   *        获取检测范围最小距离，单位cm，范围0.3~20m（30~2000），不超过MAX_RANGE，否则功能不正常。
   * @return uint16_t 
   */
  uint16_t getMinRange(void);

  /**
   * @brief Set the Detection Range object
   *        
   * @param min 检测范围最小距离，单位cm，范围0.3~20m（30~2000），不超过MAX_RANGE，否则功能不正常。
   * @param max 检测范围最大距离，单位cm，范围2.4~20m（240~2000）
   * @param trig 触发距离，单位cm，范围2.4~20m（240~2000），实际生效的配置范围不超出检测距离的最大距离和最小距离。
   * @return true 
   * @return false 
   */
  bool setDetectionRange(uint16_t min, uint16_t max);

  /**
   * @brief Set the Trig Sensitivity object
   *        设置触发灵敏度，0~9
   * @param sensitivity 
   * @return true 
   * @return false 
   */
  bool setTrigSensitivity(uint8_t sensitivity);

  /**
   * @brief Set the Keep Sensitivity object
   *        设置维持灵敏度，0~9
   * @param sensitivity 
   * @return true 
   * @return false 
   */
  bool setKeepSensitivity(uint8_t sensitivity);

  /**
   * @brief Get the Trig Sensitivity object
   *        获取触发灵敏度
   * @return uint8_t 
   */
  uint8_t getTrigSensitivity(void);

  /**
   * @brief Get the Keep Sensitivity object
   *        获取维持灵敏度
   * @return uint8_t 
   */
  uint8_t getKeepSensitivity(void);

  /**
   * @brief Get the Status object
   * 
   * @return sSensorStatus_t 
   *        workStatus
   *          0 stop
   *          1 start
   *        workMode
   *          0 为存在检测
   *          1 为测速测距
   *        initStatus
   *          0 未初始化
   *          1 初始化完成
   */
  sSensorStatus_t getStatus(void);


  /**
   * @brief Set the Io Polaity object
   * 
   * @param value
   *        0：有目标时输出低电平，无目标时输出高电平
            1：有目标时输出高电平，无目标时输出低电平（默认状态）
   * @return true 
   * @return false 
   */
  bool setIoPolaity(uint8_t value);

  /**
   * @brief Get the Io Polaity object
   *
   * @return uint8_t 配置的I/O 口检测到目标后，引脚输出的信号电平
   */
  uint8_t getIoPolaity(void);


  /**
   * @brief Set the Pwm object
   * 
   * @param pwm1 
   *        未检测到目标时，OUT引脚输出信号的占空比，取值范围：0～100
   * @param pwm2 
   *        检测到目标后，OUT引脚输出信号的占空比，取值范围：0～100
   * @param timer 
   *        从pwm1 占空比渐变为pwm2 占空比的时间，取值范围：0～255，对应时间值 = timer*64ms
   *        如timer=20，占空比从pwm1渐变为pwm2需要 20*64ms=1.28s。
   * @return true 
   * @return false 
   */
  bool setPwm(uint8_t pwm1 , uint8_t pwm2, uint8_t timer);


  /**
   * @brief Get the Pwm object
   * 
   * @return sPwmData_t 
   * @retval pwm1  未检测到目标时，OUT引脚输出信号的占空比，取值范围：0～100
   * @retval pwm2  检测到目标后，OUT引脚输出信号的占空比，取值范围：0～100
   * @retval timer  从pwm1 占空比渐变为pwm2 占空比的时间，取值范围：0～255，对应时间值 = timer*64ms
   *         如timer=20，占空比从pwm1渐变为pwm2需要 20*64ms=1.28s。
   */
  sPwmData_t getPwm(void);

  /**
   * @brief Set the Sensor Mode object
   * 
   * @param mode 
   * @return true 
   * @return false 
   */
  bool setSensorMode(eMode_t mode);

  /**
   * @brief Get the Target Number object
   * 
   * @return uint8_t 
   */
  uint8_t getTargetNumber(void);
  /**
   * @brief Get the Target Speed object
   * 
   * @return int16_t 
   */
  int16_t getTargetSpeed(void);
  /**
   * @brief Get the Target Range object
   * 
   * @return int16_t 
   */
  int16_t getTargetRange(void);
  /**
   * @brief Get the Target Energy object
   * 
   * @return int16_t 
   */
  uint32_t getTargetEnergy(void);
  /**
   * @brief Set the Detect Thres object
   * 
   * @param min 
   * @param max 
   * @param thres 
   * @return true 
   * @return false 
   */
  bool setDetectThres(uint16_t min, uint16_t max, uint16_t thres);

  /**
   * @brief 
   * 
   * @return uint16_t 
   */
  uint16_t getTMinRange(void);
  /**
   * @brief 
   * 
   * @return uint16_t 
   */
  uint16_t getTMaxRange(void);
  /**
   * @brief Get the Thres Range object
   * 
   * @return uint16_t 
   */
  uint16_t getThresRange(void);
  /**
   * @brief Set the Fretting Detection object
   * 
   * @param sta 
   */
  void setFrettingDetection(eSwitch_t sta);
  /**
   * @brief Get the Fretting Detection object
   * 
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