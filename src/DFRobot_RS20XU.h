/*!
 * @file DFRobot_RS20XU.h
 * @brief Define the basic structure of the DFRobot_RS20XU class, the implementation of the basic methods
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [ZhixinLiu](zhixin.liu@dfrobot.com)
 * @version V1.0
 * @date 2023-03-07
 * @url https://github.com/DFRobot/DFRobot_RS20XU
 */
#ifndef __DFROBOT_FACE_RECOGNITION_H__
#define __DFROBOT_FACE_RECOGNITION_H__

#include <Arduino.h>
#include <Wire.h>

#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
#include "SoftwareSerial.h"
#else
#include "HardwareSerial.h"
#endif

typedef struct{
  uint8_t workStatus;
  uint8_t workMode;
  uint8_t initStatus;
}sSensorStatus_t;


#define MAX_BUFFER 10

typedef struct{
  uint8_t  number;
  uint16_t speed;
  uint16_t range;
  uint16_t energy;
}sPrivateData_t;

typedef enum{
  eExitMode  = 0x00,
  eSpeedMode = 0x01,
}eMode_t;

typedef enum{
  eON  = 0x01,
  eOFF = 0x00,
}eSwitch_t;

typedef enum{
  eStartSen   = 0x55,
  eStopSen    = 0x33,
  eResetSen   = 0xCC,
  eRecoverSen = 0xAA,
  eSaveParams = 0x5C,
  eChangeMode = 0x3B,
}eSetMode_t;

class DFRobot_RS20XU{
public:
#define DEVICE_ADDR_0   0x2B
#define DEVICE_ADDR_1   0x2A
#define TIME_OUT        200     ///< time out
#define I2C_FLAG        1
#define UART_FLAG       2


#define REG_STATUS              0x00
#define REG_CTRL0               0x01
#define REG_CTRL1               0x02
#define REG_SOFT_VERSION        0x03
#define REG_RESULT_STATUS       0x10
#define REG_TRIG_SENSITIVITY    0x20
#define REG_KEEP_SENSITIVITY    0x21
#define REG_TRIG_DELAY          0x22
#define REG_KEEP_TIMEOUT_L      0x23
#define REG_KEEP_TIMEOUT_H      0x24
#define REG_E_MIN_RANGE_L       0x25
#define REG_E_MIN_RANGE_H       0x26
#define REG_E_MAX_RANGE_L       0x27
#define REG_E_MAX_RANGE_H       0x28
#define REG_E_TRIG_RANGE_L      0x29
#define REG_E_TRIG_RANGE_H      0x2A

#define REG_RESULT_OBJ_MUN      0x10
#define REG_RESULT_RANGE_L      0x11
#define REG_RESULT_RANGE_H      0x12
#define REG_RESULT_SPEED_L      0x13
#define REG_RESULT_SPEED_H      0x14
#define REG_RESULT_ENERGY_L     0x15
#define REG_RESULT_ENERGY_H     0x16
#define REG_CFAR_THR_L          0x20
#define REG_CFAR_THR_H          0x21
#define REG_T_MIN_RANGE_L       0x22
#define REG_T_MIN_RANGE_H       0x23
#define REG_T_MAX_RANGE_L       0x24
#define REG_T_MAX_RANGE_H       0x25
#define REG_MICRO_MOTION        0x26

  DFRobot_RS20XU();
  ~DFRobot_RS20XU();
  uint8_t  uartI2CFlag = 0;
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
   * @brief Set the Trig Delay object
   *        触发延时，单位0.01s，范围0~2s（0~200）
   * @param time 
   * 
   * @return true 
   * @return false 
   */
  bool setTrigDelay(uint8_t time);

  /**
   * @brief Get the Trig Delay object
   *        获取物体的触发延时
   * @return uint8_t 
   */
  uint8_t getTrigDelay(void);

  /**s
   * @brief 设置检测超时,停止多少秒算作物体不运动
   *        维持检测超时，单位0.5s，范围2~1500秒（4~3000）
   * @param time 
   * @return true 
   * @return false 
   */
  bool setKeepTimerout(uint16_t time);

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
  bool setDetectionRange(uint16_t min, uint16_t max, uint16_t trig);

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


  bool setSensorMode(eMode_t mode);

  uint8_t getTargetNumber(void);
  uint16_t getTargetSpeed(void);
  uint16_t getTargetRange(void);
  uint16_t getTargetEnergy(void);
  bool setDetectThres(uint16_t min, uint16_t max, uint16_t thres);
  uint16_t getTMinRange(void);
  uint16_t getTMaxRange(void);
  uint16_t getThresRange(void);
  void setFrettingDetection(eSwitch_t sta);
  eSwitch_t getFrettingDetection(void);

private:
  uint8_t  _addr;
  uint8_t  _M_Flag = 0;
  sPrivateData_t _buffer;
  virtual void writeReg(uint8_t reg, uint8_t *data, uint8_t len) = 0;
  virtual int16_t readReg(uint8_t reg, uint8_t *data, uint8_t len) = 0;
};

class DFRobot_RS20XU_I2C:public DFRobot_RS20XU{
public:
  DFRobot_RS20XU_I2C(TwoWire *pWire=&Wire, uint8_t addr = DEVICE_ADDR_0);
  bool begin(void);
protected:
  virtual void writeReg(uint8_t reg, uint8_t *data, uint8_t len);
  virtual int16_t readReg(uint8_t reg, uint8_t *data, uint8_t len);
private:
  TwoWire *_pWire;
  uint8_t _I2C_addr;

};

class DFRobot_RS20XU_UART:public DFRobot_RS20XU{
public:
#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
  DFRobot_RS20XU_UART(SoftwareSerial *sSerial, uint16_t Baud);
#else
  DFRobot_RS20XU_UART(HardwareSerial *hSerial, uint16_t Baud ,uint8_t rxpin = 0, uint8_t txpin = 0);
#endif

  bool begin(void);
protected:
  virtual void writeReg(uint8_t reg, uint8_t *data, uint8_t len);
  virtual int16_t readReg(uint8_t reg, uint8_t *data, uint8_t len);
private:

#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
  SoftwareSerial *_serial;
#else
  HardwareSerial *_serial;
#endif
  uint32_t _baud;
  uint8_t _rxpin;
  uint8_t _txpin;
};
#endif
