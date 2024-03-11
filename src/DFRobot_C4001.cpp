/*!
 * @file DFRobot_C4001.cpp
 * @brief Define the basic structure of the DFRobot_C4001 class, the implementation of the basic methods
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [ZhixinLiu](zhixin.liu@dfrobot.com)
 * @version V1.0
 * @date 2024-02-02
 * @url https://github.com/DFRobot/DFRobot_C4001
 */
#include "DFRobot_C4001.h"

DFRobot_C4001::DFRobot_C4001(){}
DFRobot_C4001::~DFRobot_C4001(){}

sSensorStatus_t DFRobot_C4001::getStatus(void)
{
  sSensorStatus_t data;
  uint8_t temp[100] = {0};
  uint8_t len = 0;
  if(uartI2CFlag == I2C_FLAG){
    uint8_t temp = 0;
    readReg(REG_STATUS, &temp, (uint8_t)1);
    data.workStatus = (temp&0x01);
    data.workMode   = (temp&0x02) >> 1;
    data.initStatus = (temp&0x80) >> 7;
  }else{  
    sAllData_t allData;
    readReg(0, temp, 100);
    writeReg(0, (uint8_t *)START_SENSOR, strlen(START_SENSOR));
    while(len == 0){
      delay(1000);
      len = readReg(0, temp, 100);
      allData = anaysisData(temp ,len);
    }
    data.workStatus = allData.sta.workStatus;
    data.workMode   = allData.sta.workMode;
    data.initStatus = allData.sta.initStatus;
  }
  return data;
}

bool DFRobot_C4001::motionDetection(void)
{
  if(uartI2CFlag == I2C_FLAG){
    uint8_t temp = 0;
    readReg(REG_RESULT_STATUS, &temp, (uint8_t)1);
    if(temp&0x01){
      return true;
    }
    return false;
  }else{
    static bool old = false;
    uint8_t status = 0;
    uint8_t len = 0;
    uint8_t temp[100] = {0};
    sAllData_t data;
    len = readReg(0, temp, 100);
    data = anaysisData(temp ,len);
    if(data.exist){
      old = (bool)status;
      return (bool)data.exist;
    }else{
      return (bool)old;
    }
  }
}

void DFRobot_C4001::setSensor(eSetMode_t mode)
{
  uint8_t temp = mode;
  if(uartI2CFlag == I2C_FLAG){
    if(mode == eStartSen){
      writeReg(REG_CTRL0, &temp, (uint8_t)1);
      delay(200);  // must timer
    }else if(mode == eStopSen){
      writeReg(REG_CTRL0, &temp, (uint8_t)1);
      delay(200);  // must timer
    }else if(mode == eResetSen){
      writeReg(REG_CTRL0, &temp, (uint8_t)1);
      delay(1500);  // must timer
    }else if(mode == eSaveParams){
      writeReg(REG_CTRL1, &temp, (uint8_t)1);
      delay(500);  // must timer
    }else if(mode == eRecoverSen){
      writeReg(REG_CTRL1, &temp, (uint8_t)1);
      delay(800);  // must timer
    }else if(mode == eChangeMode){
      writeReg(REG_CTRL1, &temp, (uint8_t)1);
      delay(1500);  // must timer
    }
  }else{
    if(mode == eStartSen){
      writeReg(0, (uint8_t *)START_SENSOR, strlen(START_SENSOR));
      delay(200);  // must timer
    }else if(mode == eStopSen){
      writeReg(0, (uint8_t *)STOP_SENSOR, strlen(STOP_SENSOR));
      delay(200);  // must timer
    }else if(mode == eResetSen){
      writeReg(0, (uint8_t *)RESET_SENSOR, strlen(RESET_SENSOR));
      delay(1500);  // must timer
    }else if(mode == eSaveParams){
      writeReg(0, (uint8_t *)STOP_SENSOR, strlen(STOP_SENSOR));
      delay(200);  // must timer
      writeReg(0, (uint8_t *)SAVE_CONFIG, strlen(SAVE_CONFIG));
      delay(800);  // must timer
      writeReg(0, (uint8_t *)START_SENSOR, strlen(START_SENSOR));
    }else if(mode == eRecoverSen){
      writeReg(0, (uint8_t *)STOP_SENSOR, strlen(STOP_SENSOR));
      delay(200);
      writeReg(0, (uint8_t *)RECOVER_SENSOR, strlen(RECOVER_SENSOR));
      delay(800);  // must timer
      writeReg(0, (uint8_t *)START_SENSOR, strlen(START_SENSOR));
      delay(500);
    }
  }
}

bool DFRobot_C4001::setSensorMode(eMode_t mode)
{
  if(uartI2CFlag == I2C_FLAG){
    sSensorStatus_t data;
    data = getStatus();
    if(data.workMode == mode){
      return true;
    }else{
      setSensor(eChangeMode);
      data = getStatus();
      if(data.workMode == mode){
        return true;
      }else{
        return false;
      }
    }
  }else{
    sensorStop();
    if(mode == eExitMode){
      writeReg(0, (uint8_t *)EXIST_MODE, strlen(EXIST_MODE));
      delay(50);  
    }else{
      writeReg(0, (uint8_t *)SPEED_MODE, strlen(SPEED_MODE));
      delay(50);
    }
    delay(50);
    writeReg(0, (uint8_t *)SAVE_CONFIG, strlen(SAVE_CONFIG));
    delay(500);
    writeReg(0, (uint8_t *)START_SENSOR, strlen(START_SENSOR));
    delay(100);
    return true;
  }
}

bool DFRobot_C4001::setTrigSensitivity(uint8_t sensitivity)
{
  uint8_t temp = sensitivity;
  if(sensitivity > 9){
    return false;
  }
  if(uartI2CFlag == I2C_FLAG){
    writeReg(REG_TRIG_SENSITIVITY, &temp, (uint8_t)1);
    setSensor(eSaveParams);
    return true;
  }else{
    String data = "setSensitivity 255 1";
    data[19] = sensitivity + 0x30;
    writeCMD(data, data, (uint8_t)1);
    return true;
  }
}

uint8_t DFRobot_C4001::getTrigSensitivity(void)
{
  if(uartI2CFlag == I2C_FLAG){
    uint8_t temp = 0;
    readReg(REG_TRIG_SENSITIVITY, &temp, (uint8_t)1);
    return temp;
  }else{
    sResponseData_t responseData;
    uint8_t temp[100] = {0};
    readReg(0, temp, 100);
    String data = "getSensitivity";
    responseData = wRCMD(data, (uint8_t)1);
    if(responseData.status){ 
      return responseData.response1;
    }
    return 0;
  }
}

bool DFRobot_C4001::setKeepSensitivity(uint8_t sensitivity)
{
  uint8_t temp = sensitivity;
  if(sensitivity > 9){
    return false;
  }
  if(uartI2CFlag == I2C_FLAG){
    writeReg(REG_KEEP_SENSITIVITY, &temp, (uint8_t)1);
    setSensor(eSaveParams);
    return true;
  }else{
    String data = "setSensitivity 1 255";
    data[15] = sensitivity + 0x30;
    writeCMD(data, data, (uint8_t)1);
    return true;
  }
}

uint8_t DFRobot_C4001::getKeepSensitivity(void)
{
  if(uartI2CFlag == I2C_FLAG){
    uint8_t temp = 0;
    readReg(REG_KEEP_SENSITIVITY, &temp, (uint8_t)1);
    return temp;
  }else{
    sResponseData_t responseData;
    uint8_t temp[100] = {0};
    readReg(0, temp, 100);
    String data = "getSensitivity";
    responseData = wRCMD(data, (uint8_t)1);
    if(responseData.status){
      return responseData.response2;
    }
    return 0;
  }
}

bool DFRobot_C4001::setDelay(uint8_t trig , uint16_t keep)
{
  if(trig > 200){
    return false;
  }
  if(keep < 4 || keep > 3000){
    return false;
  }
  if(uartI2CFlag == I2C_FLAG){
    uint8_t temp[3] = {0};
    temp[0] = trig;
    temp[1] = keep;
    temp[2] = keep>>8;
    writeReg(REG_TRIG_DELAY, temp, (uint8_t)3);
    setSensor(eSaveParams);
    return true;
  }else{
    String data = "setLatency ";
    data += String((float)trig*0.01, 1);
    data += " ";
    data += String((float)keep*0.5, 1);
    writeCMD(data, data, (uint8_t)1);
    return true;
  }
}

uint8_t DFRobot_C4001::getTrigDelay(void)
{
  if(uartI2CFlag == I2C_FLAG){
    uint8_t temp = 0;
    readReg(REG_TRIG_DELAY, &temp, (uint8_t)1);
    return temp;
  }else{
    sResponseData_t responseData;
    String data = "getLatency";
    responseData = wRCMD(data, (uint8_t)1);
    if(responseData.status){
      return responseData.response1*100;
    }
    return 0;
  }
}

uint16_t DFRobot_C4001::getKeepTimerout(void)
{
  if(uartI2CFlag == I2C_FLAG){
    uint8_t temp[2] = {0};
    readReg(REG_KEEP_TIMEOUT_L, temp, (uint8_t)2);
    return (((uint16_t)temp[1]) << 8) | temp[0];
  }else{
    sResponseData_t responseData;
    String data = "getLatency";
    responseData = wRCMD(data, (uint8_t)2);
    if(responseData.status){
      return responseData.response2*2;
    }
    return 0;
  }
}

bool DFRobot_C4001::setDetectionRange(uint16_t min, uint16_t max, uint16_t trig)
{
  if(max < 240 || max > 2000){
    return false;
  }
  if(min < 30 || min > max){
    return false;
  }
  if(uartI2CFlag == I2C_FLAG){
    uint8_t temp[10] = {0};
    temp[0] = (uint8_t)(min);
    temp[1] = (uint8_t)(min >> 8);
    temp[2] = (uint8_t)(max);
    temp[3] = (uint8_t)(max >> 8);
    temp[4] = (uint8_t)(trig);
    temp[5] = (uint8_t)(trig >> 8);
    writeReg(REG_E_MIN_RANGE_L, temp, (uint8_t)6);
    setSensor(eSaveParams);
    return true;
  }else{
    String data1 = "setRange ";
    String data2 = "setTrigRange ";
    data1 += String(((float)min)/100.0, 1);
    data1 += " ";
    data1 += String(((float)max)/100.0, 1);
    data2 += String(((float)trig)/100.0, 1);
    writeCMD(data1, data2, (uint8_t)2);
    return true;
  }
}

uint16_t DFRobot_C4001::getTrigRange(void)
{
  if(uartI2CFlag == I2C_FLAG){
    uint8_t temp[4] = {0};
    readReg(REG_E_TRIG_RANGE_L, temp, (uint8_t)2);
    return (uint16_t)(temp[0] | ((uint16_t)temp[1]) << 8);
  }else{
    sResponseData_t responseData;
    String data = "getTrigRange";
    responseData = wRCMD(data, (uint8_t)1);
    if(responseData.status){
      return responseData.response1*100;
    }
    return 0;
  }
}

uint16_t DFRobot_C4001::getMaxRange(void)
{
  if(uartI2CFlag == I2C_FLAG){
    uint8_t temp[4] = {0};
    readReg(REG_E_MAX_RANGE_L, temp, (uint8_t)2);
    return (uint16_t)(temp[0] | ((uint16_t)temp[1]) << 8);
  }else{
    sResponseData_t responseData;
    String data = "getRange";
    responseData = wRCMD(data, (uint8_t)2);
    if(responseData.status){
      return responseData.response2*100;
    }
    return 0;
  }
}

uint16_t DFRobot_C4001::getMinRange(void)
{
  if(uartI2CFlag == I2C_FLAG){
    uint8_t temp[4] = {0};
    readReg(REG_E_MIN_RANGE_L, temp, (uint8_t)2);
    return (uint16_t)(temp[0] | ((uint16_t)temp[1]) << 8);
  }else{
    sResponseData_t responseData;
    String data = "getRange";
    responseData = wRCMD(data, (uint8_t)2);
    if(responseData.status){
      return responseData.response1*100;
    }
    return 0;
  }
}

uint8_t DFRobot_C4001::getTargetNumber(void)
{
  static uint8_t flash_number = 0;
  uint8_t temp[10] = {0};
  if(uartI2CFlag == I2C_FLAG){
    readReg(REG_RESULT_OBJ_MUN, temp, (uint8_t)7);
    if(temp[0] == 1){
      flash_number = 0;
      _buffer.number = 1;
      _buffer.range  = (float)( int16_t((uint16_t)(temp[1] | ((uint16_t)temp[2]) << 8))) / 100.0;
      _buffer.speed  = (float)( int16_t((uint16_t)(temp[3] | ((uint16_t)temp[4]) << 8))) / 100.0;
      _buffer.energy = (uint16_t)(temp[5] | ((uint16_t)temp[6]) << 8);
    }else{
      if(flash_number++ > 10){
        _buffer.number = 0;
        _buffer.range  = 0;
        _buffer.speed  = 0;
        _buffer.energy = 0;
      }
    }
    return _buffer.number;
  }else{
    uint8_t len = 0;
    uint8_t temp[100] = {0};
    sAllData_t data;
    len = readReg(0, temp, 100);
    data = anaysisData(temp ,len);
    if(data.target.number != 0){
      flash_number = 0;
      _buffer.number = data.target.number;
      _buffer.range  = data.target.range/100.0;
      _buffer.speed  = data.target.speed/100.0;
      _buffer.energy = data.target.energy;
    }else{
      _buffer.number = 1;
      if(flash_number++ > 10){
        _buffer.number = 0;
        _buffer.range  = 0;
        _buffer.speed  = 0;
        _buffer.energy = 0;
      }
    }
    return data.target.number;
  }
}

float DFRobot_C4001::getTargetSpeed(void)
{
  return _buffer.speed;
}

float DFRobot_C4001::getTargetRange(void)
{
  return _buffer.range;
}

uint32_t DFRobot_C4001::getTargetEnergy(void)
{
  return _buffer.energy;
}

bool DFRobot_C4001::setDetectThres(uint16_t min, uint16_t max, uint16_t thres)
{
  if(max > 2500){
    return false;
  }
  if(min > max){
    return false;
  }
  if(uartI2CFlag == I2C_FLAG){
    uint8_t temp[10] = {0};
    temp[0] = (uint8_t)(thres);
    temp[1] = (uint8_t)(thres >> 8);
    temp[2] = (uint8_t)(min);
    temp[3] = (uint8_t)(min >> 8);
    temp[4] = (uint8_t)(max);
    temp[5] = (uint8_t)(max >> 8);
    writeReg(REG_CFAR_THR_L, temp, (uint8_t)6);
    setSensor(eSaveParams);
    return true;
  }else{
    String data1 = "setRange ";
    String data2 = "setThrFactor ";
    data1 += String(((float)min)/100.0, 1);
    data1 += " ";
    data1 += String(((float)max)/100.0, 1);
    data2 += thres;
    writeCMD(data1, data2, (uint8_t)2);
    return true;
  }
}

bool DFRobot_C4001::setIoPolaity(uint8_t value)
{
  if(value > 1){
    return false;
  }
  if(uartI2CFlag == I2C_FLAG){
    return true;
  }else{
    String data = "setGpioMode 1 ";
    data += value;
    writeCMD(data, data, (uint8_t)1);
    return true;
  }
}

uint8_t DFRobot_C4001::getIoPolaity(void)
{
  if(uartI2CFlag == I2C_FLAG){
    return 0;
  }else{
    sResponseData_t responseData;
    String data = "getGpioMode 1";
    responseData = wRCMD(data, (uint8_t)2);
    if(responseData.status){
      return responseData.response2;
    }
    return 0;
  }
}

bool DFRobot_C4001::setPwm(uint8_t pwm1 , uint8_t pwm2, uint8_t timer)
{
  if(pwm1 > 100 || pwm2 > 100){
    return false;
  }
  if(uartI2CFlag == I2C_FLAG){
    return true;
  }else{
    String data = "setPwm ";
    data += pwm1;
    data += " ";
    data += pwm2;
    data += " ";
    data += timer;
    writeCMD(data, data, (uint8_t)1);
    return true;
  }
}

sPwmData_t DFRobot_C4001::getPwm(void)
{
  sPwmData_t pwmData;
  memset(&pwmData, 0, sizeof(sPwmData_t));
  if(uartI2CFlag == I2C_FLAG){
    return pwmData;
  }else{
    sResponseData_t responseData;
    String data = "getPwm";
    responseData = wRCMD(data, (uint8_t)3);
    if(responseData.status){
      pwmData.pwm1 = responseData.response1;
      pwmData.pwm2 = responseData.response2;
      pwmData.timer = responseData.response3;
    }
    return pwmData;
  }
}

uint16_t DFRobot_C4001::getTMinRange(void)
{
  if(uartI2CFlag == I2C_FLAG){
    uint8_t temp[4] = {0};
    readReg(REG_T_MIN_RANGE_L, temp, (uint8_t)2);
    return (uint16_t)(temp[0] | ((uint16_t)temp[1]) << 8);
  }else{
    sResponseData_t responseData;
    String data = "getRange";
    responseData = wRCMD(data, (uint8_t)1);
    if(responseData.status){
      return responseData.response1*100;
    }
    return 0;
  }
}

uint16_t DFRobot_C4001::getTMaxRange(void)
{
  if(uartI2CFlag == I2C_FLAG){
    uint8_t temp[4] = {0};
    readReg(REG_T_MAX_RANGE_L, temp, (uint8_t)2);
    return (uint16_t)(temp[0] | ((uint16_t)temp[1]) << 8);
  }else{
    sResponseData_t responseData;
    String data = "getRange";
    responseData = wRCMD(data, (uint8_t)2);
    if(responseData.status){
      return responseData.response2*100;
    }
    return 0;
  }
}

uint16_t DFRobot_C4001::getThresRange(void)
{
  if(uartI2CFlag == I2C_FLAG){
    uint8_t temp[4] = {0};
    readReg(REG_CFAR_THR_L, temp, (uint8_t)2);
    return (uint16_t)(temp[0] | ((uint16_t)temp[1]) << 8);
  }else{
    sResponseData_t responseData;
    String data = "getThrFactor";
    responseData = wRCMD(data, (uint8_t)1);
    if(responseData.status){
      return responseData.response1;
    }
    return 0;
  }
}

void DFRobot_C4001::setFrettingDetection(eSwitch_t sta)
{
  if(uartI2CFlag == I2C_FLAG){
    uint8_t temp = sta;
    writeReg(REG_MICRO_MOTION, &temp, (uint8_t)1);
    setSensor(eSaveParams);
  }else{
    String data = "setMicroMotion ";
    data += sta;
    writeCMD(data, data, (uint8_t)1);
  }
}

eSwitch_t DFRobot_C4001::getFrettingDetection(void)
{
  if(uartI2CFlag == I2C_FLAG){
    uint8_t temp = 0;
    readReg(REG_MICRO_MOTION, &temp, (uint8_t)1);
    return (eSwitch_t)temp;
  }else{
    sResponseData_t responseData;
    String data = "getMicroMotion";
    responseData = wRCMD(data, (uint8_t)1);
    if(responseData.status){
      return (eSwitch_t)responseData.response1;
    }
    return (eSwitch_t)0;
  }
}

sResponseData_t DFRobot_C4001::anaysisResponse(uint8_t *data, uint8_t len ,uint8_t count)
{
  sResponseData_t responseData;
  uint8_t space[5] = {0};
  uint8_t i = 0;
  uint8_t j = 0;
  for(i = 0; i < len; i++){
    if(data[i] == 'R' && data[i+1] == 'e' && data[i+2] == 's'){
      break;
    }
  }
  if(i == len || i == 0){
    responseData.status = false;
  }else{
    responseData.status = true;
    for(j = 0; i < len; i++){
      if(data[i] == ' '){
        space[j++] = i + 1;
      }
    }
    if(j != 0){
      responseData.response1 = atof((const char*)(data+space[0]));
      if(j >= 2){
        responseData.response2 = atof((const char*)(data+space[1]));
      }
      if(count == 3){
        responseData.response3 = atof((const char*)(data+space[2]));
      }
    }else{
      responseData.response1 = 0.0;
      responseData.response2 = 0.0;
    }
  }
  return responseData;
}

sAllData_t DFRobot_C4001::anaysisData(uint8_t * data, uint8_t len)
{
  sAllData_t allData;
  uint8_t location = 0;
  memset(&allData, 0, sizeof(sAllData_t));
  for(uint8_t i = 0; i < len; i++){
    if(data[i] == '$'){
      location = i;
      break;
    }
  }
  if(location == len){
    return allData;
  }
  if(0 == strncmp((const char *)(data+location), "$DFHPD", strlen("$DFHPD"))){
    allData.sta.workMode = eExitMode;
    allData.sta.workStatus = 1;
    allData.sta.initStatus = 1;
    if(data[location+7] == '1'){
      allData.exist = 1;
    }else{
      allData.exist = 0;
    }
  }else if(0 == strncmp((const char *)(data+location), "$DFDMD", strlen("$DFDMD"))){
    // $DFDMD,par1,par2,par3,par4,par5,par6,par7*
    allData.sta.workMode = eSpeedMode;
    allData.sta.workStatus = 1;
    allData.sta.initStatus = 1;
    char *token;
    char *parts[10]; // Let's say there are at most 10 parts
    int index = 0;   // Used to track the number of parts stored
    token = strtok((char*)(data+location), ",");
    while (token != NULL) {
      parts[index] = token; // Stores partial Pointers in an array
      if(index++ > 8){
        break;
      }
      token = strtok(NULL, ","); // Continue to extract the next section
    }
    allData.target.number = atoi(parts[1]);
    allData.target.range = atof(parts[3]) * 100;
    allData.target.speed = atof(parts[4]) * 100;
    allData.target.energy = atof(parts[5]);
  }else{
  }
  return allData;
}

sResponseData_t DFRobot_C4001::wRCMD(String cmd1, uint8_t count)
{
  uint8_t len = 0;
  uint8_t temp[200] = {0};
  sResponseData_t responseData;
  sensorStop();
  writeReg(0, (uint8_t *)cmd1.c_str(), cmd1.length());
  delay(100);
  len = readReg(0, temp, 200);
  responseData = anaysisResponse(temp, len, count);
  delay(100);
  writeReg(0, (uint8_t *)START_SENSOR, strlen(START_SENSOR));
  delay(100);
  return responseData;
}

void DFRobot_C4001::writeCMD(String cmd1 , String cmd2, uint8_t count)
{
  sensorStop();
  writeReg(0, (uint8_t *)cmd1.c_str(), cmd1.length());
  delay(100);
  if(count > 1){
    delay(100);
    writeReg(0, (uint8_t *)cmd2.c_str(), cmd2.length());
    delay(100);
  }
  writeReg(0, (uint8_t *)SAVE_CONFIG, strlen(SAVE_CONFIG));
  delay(100);
  writeReg(0, (uint8_t *)START_SENSOR, strlen(START_SENSOR));
  delay(100);
}

bool DFRobot_C4001::sensorStop(void)
{
  uint8_t len = 0;
  uint8_t temp[200] = {0};
  writeReg(0, (uint8_t *)STOP_SENSOR, strlen(STOP_SENSOR));
  delay(1000);
  len = readReg(0, temp, 200);
  while(1){
    if(len != 0){
      if (strstr((const char *)temp, "sensorStop") != NULL) {
        return true;
      }
    }
    memset(temp, 0, 200);
    delay(400);
    writeReg(0, (uint8_t *)STOP_SENSOR, strlen(STOP_SENSOR));
    len = readReg(0, temp, 200);
    
  }
}

DFRobot_C4001_I2C::DFRobot_C4001_I2C(TwoWire *pWire, uint8_t addr)
{
  _pWire = pWire;
  this->_I2C_addr = addr;
  uartI2CFlag = I2C_FLAG;
}

bool DFRobot_C4001_I2C::begin()
{
  _pWire->begin();
  _pWire->beginTransmission(_I2C_addr);
  if(_pWire->endTransmission() == 0){
    return true;
  }else{
    return false;
  }
}

void DFRobot_C4001_I2C::writeReg(uint8_t reg, uint8_t *data, uint8_t len)
{
  _pWire->beginTransmission(this->_I2C_addr);
  _pWire->write(reg);
  _pWire->write(data, len);
  _pWire->endTransmission();
}

int16_t DFRobot_C4001_I2C::readReg(uint8_t reg, uint8_t *data, uint8_t len)
{
  /* i2c get data */
  uint8_t i = 0;
  _pWire->beginTransmission(this->_I2C_addr);
  _pWire->write(reg);
  if(_pWire->endTransmission(false) != 0){
    return -1;
  }
  _pWire->requestFrom((uint8_t)this->_I2C_addr,(uint8_t)len);
  while (_pWire->available()){
    data[i++]=_pWire->read();
  }
  return i;
}

#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
  DFRobot_C4001_UART::DFRobot_C4001_UART(SoftwareSerial *sSerial, uint32_t Baud)
  {
    this->_serial = sSerial;
    this->_baud = Baud;
    uartI2CFlag = UART_FLAG;
    //_serial->begin(this->_baud);
  }
#else
  DFRobot_C4001_UART::DFRobot_C4001_UART(HardwareSerial *hSerial, uint32_t Baud ,uint8_t txpin, uint8_t rxpin)
  {
    this->_serial = hSerial;
    this->_baud = Baud;
    uartI2CFlag = UART_FLAG;
    this->_txpin = txpin;
    this->_rxpin = rxpin;
  }
#endif

bool DFRobot_C4001_UART::begin()
{
  #ifdef ESP32
    _serial->begin(this->_baud, SERIAL_8N1, _txpin, _rxpin);
  #elif defined(ARDUINO_AVR_UNO) || defined(ESP8266)
    _serial->begin(this->_baud);
    delay(1000);
  #else
    _serial->begin(this->_baud);  // M0 cannot create a begin in a construct
  #endif
  return true;
}

void DFRobot_C4001_UART::writeReg(uint8_t reg, uint8_t *data, uint8_t len)
{
  _serial->write(data, len);
  len = reg;
}

int16_t DFRobot_C4001_UART::readReg(uint8_t reg, uint8_t *data, uint8_t len)
{
  uint16_t i = 0;
  uint32_t nowtime = millis();
  while(millis() - nowtime < TIME_OUT){
    while(_serial->available() > 0){
      if(i == len) return len;
      data[i++] = _serial->read();
    }
  }
  len = reg;
  reg = len;
  return i;
}
