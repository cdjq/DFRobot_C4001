/*!
 * @file DFRobot_RS20XU.cpp
 * @brief Define the basic structure of the DFRobot_RS20XU class, the implementation of the basic methods
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [ZhixinLiu](zhixin.liu@dfrobot.com)
 * @version V1.0
 * @date 2023-03-07
 * @url https://github.com/DFRobot/DFRobot_RS20XU
 */
#include "DFRobot_RS20XU.h"

DFRobot_RS20XU::DFRobot_RS20XU(){}
DFRobot_RS20XU::~DFRobot_RS20XU(){}

sSensorStatus_t DFRobot_RS20XU::getStatus(void)
{
  sSensorStatus_t data;
  uint8_t temp = 0;
  readReg(REG_STATUS, &temp, (uint8_t)1);
  data.workStatus = (temp&0x01);
  data.workMode   = (temp&0x02) >> 1;
  data.initStatus = (temp&0x80) >> 7;
  return data;
}

bool DFRobot_RS20XU::motionDetection(void)
{
  uint8_t temp = 0;
  readReg(REG_RESULT_STATUS, &temp, (uint8_t)1);
  if(temp&0x01){
    return true;
  }
  return false;
}

void DFRobot_RS20XU::setSensor(eSetMode_t mode)
{
  uint8_t temp = mode;
  if(mode == eStartSen || mode == eStopSen || mode == eResetSen){
    writeReg(REG_CTRL0, &temp, (uint8_t)1);
    delay(2000);  // must timer
  }else if(mode == eSaveParams){
    writeReg(REG_CTRL1, &temp, (uint8_t)1);
  }else{
    writeReg(REG_CTRL1, &temp, (uint8_t)1);
    delay(2000);  // must timer
  }
}

bool DFRobot_RS20XU::setSensorMode(eMode_t mode)
{
  sSensorStatus_t data;
  data = getStatus();
  if(data.workMode == mode){
    return true;
  }else{
    setSensor(eChangeMode);
    data = getStatus();
    if(data.workMode == mode){
      Serial.println("mode successful !");
      return true;
    }else{
      Serial.println("mode change faild !");
      return false;
    }
  }
}

bool DFRobot_RS20XU::setTrigSensitivity(uint8_t sensitivity)
{
  uint8_t temp = sensitivity;
  if(sensitivity > 9){
    return false;
  }
  writeReg(REG_TRIG_SENSITIVITY, &temp, (uint8_t)1);
  return true;

}
uint8_t DFRobot_RS20XU::getTrigSensitivity(void)
{
  uint8_t temp = 0;
  readReg(REG_TRIG_SENSITIVITY, &temp, (uint8_t)1);
  return temp;
}

bool DFRobot_RS20XU::setKeepSensitivity(uint8_t sensitivity)
{
  uint8_t temp = sensitivity;
  if(sensitivity > 9){
    return false;
  }
  writeReg(REG_KEEP_SENSITIVITY, &temp, (uint8_t)1);
  return true;
}

uint8_t DFRobot_RS20XU::getKeepSensitivity(void)
{
  uint8_t temp = 0;
  readReg(REG_KEEP_SENSITIVITY, &temp, (uint8_t)1);
  return temp;
}


bool DFRobot_RS20XU::setTrigDelay(uint8_t time)
{
  if(time > 200){
    return false;
  }
  writeReg(REG_TRIG_DELAY, &time, (uint8_t)1);
  return true; 
}

uint8_t DFRobot_RS20XU::getTrigDelay(void)
{
  uint8_t temp = 0;
  readReg(REG_TRIG_DELAY, &temp, (uint8_t)1);
  return temp;
}

bool DFRobot_RS20XU::setKeepTimerout(uint16_t time)
{
  uint8_t temp[2] = {0};
  if(time > 200){
    return false;
  }
  temp[0] = time;
  temp[1] = time>>8;
  writeReg(REG_KEEP_TIMEOUT_L, temp, (uint8_t)2);
  return true; 
}

uint16_t DFRobot_RS20XU::getKeepTimerout(void)
{
  uint8_t temp[2] = {0};
  readReg(REG_KEEP_TIMEOUT_L, temp, (uint8_t)2);
  return (((uint16_t)temp[1]) << 8) | temp[0];
}


bool DFRobot_RS20XU::setDetectionRange(uint16_t min, uint16_t max, uint16_t trig)
{
  uint8_t temp[10] = {0};
  if(max < 240 || max > 2000){
    return false;
  }
  if(min < 30 || min > max){
    return false;
  }
  if(trig > max || trig < min){
    return false;
  }
  temp[0] = (uint8_t)(min);
  temp[1] = (uint8_t)(min >> 8);
  temp[2] = (uint8_t)(max);
  temp[3] = (uint8_t)(max >> 8);
  temp[4] = (uint8_t)(trig);
  temp[5] = (uint8_t)(trig >> 8);
  writeReg(REG_E_MIN_RANGE_L, temp, (uint8_t)6);  
  return true;
}


uint16_t DFRobot_RS20XU::getTrigRange(void)
{
  uint8_t temp[4] = {0};
  readReg(REG_E_TRIG_RANGE_L, temp, (uint8_t)2);
  return (uint16_t)(temp[0] | ((uint16_t)temp[1]) << 8);
}

uint16_t DFRobot_RS20XU::getMaxRange(void)
{
  uint8_t temp[4] = {0};
  readReg(REG_E_MAX_RANGE_L, temp, (uint8_t)2);
  return (uint16_t)(temp[0] | ((uint16_t)temp[1]) << 8);
}

uint16_t DFRobot_RS20XU::getMinRange(void)
{
  uint8_t temp[4] = {0};
  readReg(REG_E_MIN_RANGE_L, temp, (uint8_t)2);
  return (uint16_t)(temp[0] | ((uint16_t)temp[1]) << 8);
}



uint8_t DFRobot_RS20XU::getTargetNumber(void)
{
  uint8_t count = 0;
  uint8_t temp[10] = {0};
  for(uint8_t i = 0; i < MAX_BUFFER; i++){
    readReg(REG_RESULT_OBJ_MUN, temp, (uint8_t)7);
    count += temp[0];
    delay(10);
  }

  if(count < 2){
    _buffer.number = 0;
    _buffer.range  = 0;
    _buffer.speed  = 0;
    _buffer.energy = 0;
  }else{
    _buffer.number = 1;
    _buffer.range  = (uint16_t)(temp[1] | ((uint16_t)temp[2]) << 8);
    _buffer.speed  = (uint16_t)(temp[3] | ((uint16_t)temp[4]) << 8);
    _buffer.energy = (uint16_t)(temp[5] | ((uint16_t)temp[6]) << 8);
  }
  return _buffer.number;
}


uint16_t DFRobot_RS20XU::getTargetSpeed(void)
{
  return _buffer.speed;
}

uint16_t DFRobot_RS20XU::getTargetRange(void)
{
  return _buffer.range;
}

uint16_t DFRobot_RS20XU::getTargetEnergy(void)
{
  return _buffer.energy;
}


bool DFRobot_RS20XU::setDetectThres(uint16_t min, uint16_t max, uint16_t thres)
{
  uint8_t temp[10] = {0};
  if(max > 2500){
    return false;
  }
  if(min > max){
    return false;
  }
  temp[0] = (uint8_t)(thres);
  temp[1] = (uint8_t)(thres >> 8);
  temp[2] = (uint8_t)(min);
  temp[3] = (uint8_t)(min >> 8);
  temp[4] = (uint8_t)(max);
  temp[5] = (uint8_t)(max >> 8);
  writeReg(REG_CFAR_THR_L, temp, (uint8_t)6);
  return true;
}

uint16_t DFRobot_RS20XU::getTMinRange(void)
{
  uint8_t temp[4] = {0};
  readReg(REG_T_MIN_RANGE_L, temp, (uint8_t)2);
  return (uint16_t)(temp[0] | ((uint16_t)temp[1]) << 8);
}

uint16_t DFRobot_RS20XU::getTMaxRange(void)
{
  uint8_t temp[4] = {0};
  readReg(REG_T_MAX_RANGE_L, temp, (uint8_t)2);
  return (uint16_t)(temp[0] | ((uint16_t)temp[1]) << 8);
}

uint16_t DFRobot_RS20XU::getThresRange(void)
{
  uint8_t temp[4] = {0};
  readReg(REG_CFAR_THR_L, temp, (uint8_t)2);
  return (uint16_t)(temp[0] | ((uint16_t)temp[1]) << 8);
}


void DFRobot_RS20XU::setFrettingDetection(eSwitch_t sta)
{
  uint8_t temp = sta;
  writeReg(REG_MICRO_MOTION, &temp, (uint8_t)1);
}

eSwitch_t DFRobot_RS20XU::getFrettingDetection(void)
{
  uint8_t temp = 0;
  readReg(REG_MICRO_MOTION, &temp, (uint8_t)1);
  return (eSwitch_t)temp;
}


DFRobot_RS20XU_I2C::DFRobot_RS20XU_I2C(TwoWire *pWire, uint8_t addr)
{
  _pWire = pWire;
  this->_I2C_addr = addr;
  uartI2CFlag = I2C_FLAG;
}

bool DFRobot_RS20XU_I2C::begin()
{
  _pWire->begin();
  _pWire->beginTransmission(_I2C_addr);
  if(_pWire->endTransmission() == 0){
    return true;
  }else{
    return false;
  }
}

void DFRobot_RS20XU_I2C::writeReg(uint8_t reg, uint8_t *data, uint8_t len)
{
  _pWire->beginTransmission(this->_I2C_addr);
  _pWire->write(reg);
  _pWire->write(data, len);
  _pWire->endTransmission();
}

int16_t DFRobot_RS20XU_I2C::readReg(uint8_t reg, uint8_t *data, uint8_t len)
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
  DFRobot_RS20XU_UART::DFRobot_RS20XU_UART(SoftwareSerial *sSerial, uint16_t Baud)
  {
    this->_serial = sSerial;
    this->_baud = Baud;
    uartI2CFlag = UART_FLAG;
    _serial->begin(this->_baud);
  }
#else
  DFRobot_RS20XU_UART::DFRobot_RS20XU_UART(HardwareSerial *hSerial, uint16_t Baud ,uint8_t txpin, uint8_t rxpin)
  {
    this->_serial = hSerial;
    this->_baud = Baud;
    uartI2CFlag = UART_FLAG;
    this->_txpin = txpin;
    this->_rxpin = rxpin;
  }
#endif

bool DFRobot_RS20XU_UART::begin()
{
  uint8_t rx_temp[40] = {0x00};
  int len = 0;
  #ifdef ESP32
    _serial->begin(this->_baud, SERIAL_8N1, _txpin, _rxpin);
  #elif defined(ARDUINO_AVR_UNO) || defined(ESP8266)
    // nothing use software
  #else
    _serial->begin(this->_baud);  // M0 cannot create a begin in a construct
  #endif
  len = readReg(0, rx_temp, 0);
  return true;
}

void DFRobot_RS20XU_UART::writeReg(uint8_t reg, uint8_t *data, uint8_t len)
{
  for(uint8_t i = 0; i < len; i++){
    _serial->write(data[i]);
  }
}

int16_t DFRobot_RS20XU_UART::readReg(uint8_t reg, uint8_t *data, uint8_t len)
{
  uint16_t i = 0;
  uint32_t nowtime = millis();
  while(millis() - nowtime < TIME_OUT){
    while(_serial->available() > 0){
      data[i++] = _serial->read();
    }
  }
  return i;
}
