/*!
 * @file DFRobot_C4001.h
 * @brief Define the basic structure of the DFRobot_C4001 class, the implementation of the basic methods
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [ZhixinLiu](zhixin.liu@dfrobot.com)
 * @version V1.0
 * @date 2023-02-02
 * @url https://github.com/DFRobot/DFRobot_C4001
 */
#ifndef __DFROBOT_C4001_H__
#define __DFROBOT_C4001_H__

#include <Arduino.h>
#include <Wire.h>

#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
#include "SoftwareSerial.h"
#else
#include "HardwareSerial.h"

#endif

#define MAX_BUFFER 5
typedef struct{
  uint8_t workStatus;
  uint8_t workMode;
  uint8_t initStatus;
}sSensorStatus_t;


typedef struct{
  uint8_t  number;
  float speed;
  float range;
  uint32_t energy;
}sPrivateData_t;

typedef struct{
  bool status;
  float response1;
  float response2;
  float response3;
}sResponseData_t;

typedef struct{
  uint8_t pwm1;
  uint8_t pwm2;
  uint8_t timer;
}sPwmData_t;

typedef struct{
  uint8_t exist;
  sSensorStatus_t sta;
  sPrivateData_t target;
}sAllData_t;

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

class DFRobot_C4001{
public:
#define DEVICE_ADDR_0   0x2A
#define DEVICE_ADDR_1   0x2B
#define TIME_OUT        50     ///< time out
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


  #define START_SENSOR    "sensorStart"
  #define STOP_SENSOR     "sensorStop"
  #define SAVE_CONFIG     "saveConfig"
  #define RECOVER_SENSOR  "resetCfg"        ///> factory data reset
  #define RESET_SENSOR    "resetSystem"     ///> RESET_SENSOR
  #define SPEED_MODE      "setRunApp 1"
  #define EXIST_MODE      "setRunApp 0"


  DFRobot_C4001();
  ~DFRobot_C4001();
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
  float getTargetSpeed(void);

  /**
   * @brief Get the Target Range object
   * 
   * @return int16_t 
   */
  float getTargetRange(void);

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

  sResponseData_t wRCMD(String cmd1, uint8_t count);
  void writeCMD(String cmd1 , String cmd2, uint8_t count);
  sAllData_t anaysisData(uint8_t * data, uint8_t len);
  sResponseData_t anaysisResponse(uint8_t *data, uint8_t len ,uint8_t count);
private:
  uint8_t  _addr;
  uint8_t  _M_Flag = 0;
  sPrivateData_t _buffer;
  virtual void writeReg(uint8_t reg, uint8_t *data, uint8_t len) = 0;
  virtual int16_t readReg(uint8_t reg, uint8_t *data, uint8_t len) = 0;
};

class DFRobot_C4001_I2C:public DFRobot_C4001{
public:
  DFRobot_C4001_I2C(TwoWire *pWire=&Wire, uint8_t addr = DEVICE_ADDR_0);
  bool begin(void);
protected:
  virtual void writeReg(uint8_t reg, uint8_t *data, uint8_t len);
  virtual int16_t readReg(uint8_t reg, uint8_t *data, uint8_t len);
private:
  TwoWire *_pWire;
  uint8_t _I2C_addr;

};

class DFRobot_C4001_UART:public DFRobot_C4001{
public:
#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
  DFRobot_C4001_UART(SoftwareSerial *sSerial, uint32_t Baud);
#else
  DFRobot_C4001_UART(HardwareSerial *hSerial, uint32_t Baud ,uint8_t rxpin = 0, uint8_t txpin = 0);
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
