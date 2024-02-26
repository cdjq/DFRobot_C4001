# DFRobot_C4001
- [中文版](./README_CN.md)

Determine whether the human body exists, and its anti-interference ability is relatively strong, and it is not easy to be affected by factors such as temperature changes, environmental light changes and environmental noise. Whether the human body is sitting, sleeping or moving, the sensor can quickly and sensitively detect its presence.

![效果图](resources/images/mmWaveLadar.jpg)

## Product Link(https://www.dfrobot.com/product-2282.html)
    SKU：SEN0609 SEN0610

## Table of Contents

* [Summary](#Summary)
* [Installation](#Installation)
* [Methods](#Methods)
* [Compatibility](#Compatibility)
* [History](#History)
* [Credits](#Credits)

## Summary

Provides an Arduino library that has the following features:
  Get whether the human body exists
  Get some state of motion of the object

## Installation
There are two methods for using this library:<br>
1. Open Arduino IDE, search for "DFRobot_C4001" on the status bar in Tools ->Manager Libraries and install the library.<br>
2. Download the library file before use, paste it into \Arduino\libraries directory, then open the examples folder and run the demo in the folder.<br>

## Methods

```C++
  /**
   * @fn motionDetection
   * @brief motion Detection
   * @return true
   * @return false
   */
  bool motionDetection(void);

  /**
   * @fn setSensor
   * @brief Set the Sensor object
   * 
   * @param mode
   *   eStartSen        start collect
   *   eStopSen         stop collect
   *   eResetSen        reset sensor
   *   eRecoverSen      recover params
   *   eSaveParams      save config
   *   eChangeMode      chagne mode
   */
  void setSensor(eSetMode_t mode);
  
  /**
   * @fn setDelay
   * @brief Set the Delay object
   * @param trig Trigger delay, unit 0.01s, range 0~2s (0~200)
   * @param keep Maintain the detection timeout, unit 0.5s, range 2~1500 seconds (4~3000)
   * @return true 
   * @return false 
   */
  bool setDelay(uint8_t trig , uint16_t keep);

  /**
   * @fn getTrigDelay
   * @brief Get the Trig Delay object
   * @return uint8_t 
   */
  uint8_t getTrigDelay(void);

  /**
   * @fn getKeepTimerout
   * @brief get keep timer out
   * 
   * @return  uint16_t 
   */
  uint16_t getKeepTimerout(void);

  /**
   * @fn getTrigRange
   * @brief Get the Trig Range object
   * @n     The triggering distance, in cm, ranges from 2.4 to 20m (240 to 2000). 
   * @n     The actual configuration range does not exceed the maximum and minimum detection distance.
   * @return uint16_t 
   */
  uint16_t getTrigRange(void);

  /**
   * @fn getMaxRange
   * @brief Get the Max Range object
   * @return  uint16_t 
   */
  uint16_t getMaxRange(void);

  /**
   * @fn getMinRange
   * @brief Get the Min Range object
   * @return uint16_t 
   */
  uint16_t getMinRange(void);

  /**
   * @fn setDetectionRange
   * @brief Set the Detection Range object
   * @param min Detection range Minimum distance, unit cm, range 0.3~20m (30~2000), not exceeding max, otherwise the function is abnormal.
   * @param max Detection range Maximum distance, unit cm, range 2.4~20m (240~2000)
   * @param trig The trigger distance (unit: cm) ranges from 2.4 to 20m (240 to 2000). The actual configuration range does not exceed the maximum and minimum detection distance.
   * @return true 
   * @return false 
   */
  bool setDetectionRange(uint16_t min, uint16_t max);

  /**
   * @fn setTrigSensitivity
   * @brief Set trigger sensitivity, 0~9
   * @param sensitivity 
   * @return true 
   * @return false 
   */
  bool setTrigSensitivity(uint8_t sensitivity);

  /**
   * @fn setKeepSensitivity
   * @brief Set the Keep Sensitivity object，0~9
   * @param sensitivity 
   * @return true 
   * @return false 
   */
  bool setKeepSensitivity(uint8_t sensitivity);

  /**
   * @fn getTrigSensitivity
   * @brief Get the Trig Sensitivity object
   * @return uint8_t 
   */
  uint8_t getTrigSensitivity(void);

  /**
   * @fn getKeepSensitivity
   * @brief Get the Keep Sensitivity object
   * @return uint8_t 
   */
  uint8_t getKeepSensitivity(void);

  /**
   * @fn getStatus
   * @brief Get the Status object
   * 
   * @return sSensorStatus_t 
   *        workStatus
   *          0 stop
   *          1 start
   *        workMode
   *          0 indicates presence detection
   *          1 is speed measurement and ranging
   *        initStatus
   *          0 not init
   *          1 init success
   */
  sSensorStatus_t getStatus(void);


  /**
   * @fn setIoPolaity
   * @brief Set the Io Polaity object
   * @param value
   *        0：Output low level when there is a target, output high level when there is no target
   *        1: Output high level when there is a target, output low level when there is no target (default)
   * @return true 
   * @return false 
   */
  bool setIoPolaity(uint8_t value);

  /**
   * @fn getIoPolaity
   * @brief Get the Io Polaity object
   * @return uint8_t The level of the signal output by the pin after the configured I/O port detects the target
   */
  uint8_t getIoPolaity(void);


  /**
   * @fn setPwm
   * @brief Set the Pwm object
   * @param pwm1 
   *        pwm1  When no target is detected, the duty cycle of the output signal of the OUT pin ranges from 0 to 100
   * @param pwm2 
   *        After the target is detected, the duty cycle of the output signal of the OUT pin ranges from 0 to 100
   * @param timer 
   *        timer The value ranges from 0 to 255, corresponding to timer x 64ms
   *        For example, timer=20, it takes 20*64ms=1.28s for the duty cycle to change from pwm1 to pwm2.
   * @return true 
   * @return false 
   */
  bool setPwm(uint8_t pwm1 , uint8_t pwm2, uint8_t timer);


  /**
   * @fn getPwm
   * @brief Get the Pwm object
   * @return sPwmData_t 
   * @retval pwm1  When no target is detected, the duty cycle of the output signal of the OUT pin ranges from 0 to 100
   * @retval pwm2  After the target is detected, the duty cycle of the output signal of the OUT pin ranges from 0 to 100
   * @retval timer The value ranges from 0 to 255, corresponding to timer x 64ms
   *         For example, timer=20, it takes 20*64ms=1.28s for the duty cycle to change from pwm1 to pwm2.
   */
  sPwmData_t getPwm(void);

  /**
   * @fn setSensorMode
   * @brief Set the Sensor Mode object
   * @param mode 
   * @return true 
   * @return false 
   */
  bool setSensorMode(eMode_t mode);

  /**
   * @fn getTargetNumber
   * @brief Get the Target Number object
   * @return uint8_t 
   */
  uint8_t getTargetNumber(void);

  /**
   * @fn getTargetSpeed
   * @brief Get the Target Speed object
   * @return int16_t 
   */
  int16_t getTargetSpeed(void);

  /**
   * @fn getTargetRange
   * @brief Get the Target Range object
   * @return int16_t 
   */
  int16_t getTargetRange(void);

  /**
   * @fn getTargetEnergy
   * @brief Get the Target Energy object
   * @return uint32_t 
   */
  uint32_t getTargetEnergy(void);

  /**
   * @fn setDetectThres
   * @brief Set the Detect Thres object
   * @param min 
   * @param max 
   * @param thres 
   * @return true 
   * @return false 
   */ 
  bool setDetectThres(uint16_t min, uint16_t max, uint16_t thres);

  /**
   * @fn getTMinRange
   * @brief get speed Min Range
   * @return uint16_t 
   */
  uint16_t getTMinRange(void);

  /**
   * @fn getTMaxRange
   * @brief get speed Max Range
   * @return uint16_t 
   */
  uint16_t getTMaxRange(void);

  /**
   * @fn getThresRange
   * @brief Get the Thres Range object
   * @return uint16_t 
   */
  uint16_t getThresRange(void);

  /**
   * @fn setFrettingDetection
   * @brief Set the Fretting Detection object
   * @param sta 
   */
  void setFrettingDetection(eSwitch_t sta);

  /**
   * @fn getFrettingDetection
   * @brief Get the Fretting Detection object
   * @return eSwitch_t 
   */
  eSwitch_t getFrettingDetection(void);

```

## Compatibility

MCU                | Work Well    |   Work Wrong    | Untested    | Remarks
------------------ | :----------: | :-------------: | :---------: | :----:
Arduino Uno        |      √       |                 |             |
Arduino MEGA2560   |      √       |                 |             |
Arduino Leonardo   |      √       |                 |             |
FireBeetle-ESP8266 |      √       |                 |             |
FireBeetle-ESP32   |      √       |                 |             |
FireBeetle-M0      |      √       |                 |             |
Micro:bit          |      √       | nonsupport uart |             |


## History

- 2024/02/26 - Version 1.0.0 released.

## Credits

Written by ZhixinLiu(zhixin.liu@dfrobot.com), 2024. (Welcome to our website)
