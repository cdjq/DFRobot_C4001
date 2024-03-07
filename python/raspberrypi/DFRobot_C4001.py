# -*- coding: utf-8 -*
'''!
  @file DFRobot_C4001.py
  @brief DFRobot_C4001 Class infrastructure, implementation of underlying methods
  @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license The MIT License (MIT)
  @author [ZhixinLiu](zhixin.liu@dfrobot.com)
  @version V1.0
  @date 2024-2-18
  @url https://github.com/DFRobot/DFRobot_C4001
'''
import serial
import time
import smbus

I2C_MODE  = 0x01
UART_MODE = 0x02

FRETTING_ON  = 1
FRETTING_OFF = 0

I2C_START_SENSOR   = 0X55
I2C_STOP_SENSOR    = 0X33
I2C_RESET_SENSOR   = 0XCC
I2C_RECOVER_SENSOR = 0XAA
I2C_SAVE_SENSOR    = 0X5C
I2C_CHANGE_MODE    = 0X3B

class struct_status:
  def __init__(self):
    self.work_status = 0
    self.work_mode = 0
    self.init_status = 0

class struct_private_data:
  def __init__(self):
    self.number = 0
    self.speed = 0
    self.range = 0
    self.energy = 0

class struct_response_data:
  def __init__(self):
    self.status = 0
    self.response1 = 0.0
    self.response2 = 0.0
    self.response3 = 0.0

class struct_pwm_data:
  def __init__(self):
    self.pwm1 = 0
    self.pwm2 = 0
    self.timer = 0

class struct_all_data:
  def __init__(self):
    self.exist = 0
    self.work_status = 0
    self.work_mode = 0
    self.init_status = 0
    self.number = 0
    self.speed = 0.0
    self.range = 0.0
    self.energy = 0

status = struct_status()

private_data = struct_private_data()
response_data = struct_response_data()


EXIST_MODE = 0
SPEED_MODE = 1


REG_STATUS              = 0x00
REG_CTRL0               = 0x01
REG_CTRL1               = 0x02
REG_SOFT_VERSION        = 0x03
REG_RESULT_STATUS       = 0x10
REG_TRIG_SENSITIVITY    = 0x20
REG_KEEP_SENSITIVITY    = 0x21
REG_TRIG_DELAY          = 0x22
REG_KEEP_TIMEOUT_L      = 0x23
REG_KEEP_TIMEOUT_H      = 0x24
REG_E_MIN_RANGE_L       = 0x25
REG_E_MIN_RANGE_H       = 0x26
REG_E_MAX_RANGE_L       = 0x27
REG_E_MAX_RANGE_H       = 0x28
REG_E_TRIG_RANGE_L      = 0x29
REG_E_TRIG_RANGE_H      = 0x2A
REG_RESULT_OBJ_MUN      = 0x10
REG_RESULT_RANGE_L      = 0x11
REG_RESULT_RANGE_H      = 0x12
REG_RESULT_SPEED_L      = 0x13
REG_RESULT_SPEED_H      = 0x14
REG_RESULT_ENERGY_L     = 0x15
REG_RESULT_ENERGY_H     = 0x16
REG_CFAR_THR_L          = 0x20
REG_CFAR_THR_H          = 0x21
REG_T_MIN_RANGE_L       = 0x22
REG_T_MIN_RANGE_H       = 0x23
REG_T_MAX_RANGE_L       = 0x24
REG_T_MAX_RANGE_H       = 0x25
REG_MICRO_MOTION        = 0x26

START_SENSOR   = "sensorStart"
STOP_SENSOR    = "sensorStop"
SAVE_CONFIG    = "saveConfig"
RECOVER_SENSOR = "resetCfg"        # factory data reset
RESET_SENSOR   = "resetSystem"     # RESET_SENSOR
SPEED_SENSOR   = "setRunApp 1"
EXIST_SENSOR   = "setRunApp 0"


class DFRobot_C4001(object):
  __uart_i2c     =  0
  __speed_null_count = 0
  __all_data = struct_all_data()
  def __init__(self, bus, Baud):
    if bus != 0:
      self.i2cbus = smbus.SMBus(bus)
      self.__uart_i2c = I2C_MODE
    else:
      self.ser = serial.Serial("/dev/ttyAMA0", baudrate=Baud,stopbits=1, timeout=0.5)
      self.__uart_i2c = UART_MODE
      if self.ser.isOpen == False:
        self.ser.open()

  def begin(self):
    '''!
      @brief begin 
    '''
    return True

  def get_status(self):
    '''!
      @brief get_status
      @return status
    '''
    data = struct_status()
    if self.__uart_i2c == I2C_MODE:
      rslt = self.read_reg(REG_STATUS, 1)
      data.work_status = rslt[0]&0x01
      data.work_mode   = (rslt[0]&0x02) >> 1
      data.init_status = (rslt[0]&0x80) >> 7
      return data
    else:
      return data

  def set_sensor(self, mode):
    '''!
      @brief set_sensor
      @param mode
    '''
    if self.__uart_i2c == I2C_MODE:
      send = [0]*1
      send[0] = mode
      time.sleep(0.1)
      if mode == I2C_START_SENSOR:
        self.write_reg(REG_CTRL0, send)
        time.sleep(0.2)
      elif mode == I2C_STOP_SENSOR:
        self.write_reg(REG_CTRL0, send)
        time.sleep(0.2)
      elif mode == I2C_RESET_SENSOR:
        self.write_reg(REG_CTRL0, send)
        time.sleep(1.5)
      elif mode == I2C_RECOVER_SENSOR:
        self.write_reg(REG_CTRL1, send)
        time.sleep(0.8)
      elif mode == I2C_SAVE_SENSOR:
        self.write_reg(REG_CTRL1, send)
        time.sleep(0.5)
      elif mode == I2C_CHANGE_MODE:
        self.write_reg(REG_CTRL1, send)
        time.sleep(1.5)
      return 0
    else:
      return 0

  def motion_detection(self):
    '''!
      @brief motion_detection
      @return status
      @retval 0 no exist
      @retval 1 exist
    '''
    if self.__uart_i2c == I2C_MODE:
      rslt = self.read_reg(REG_RESULT_STATUS, 1)
      return rslt[0]&0x01
    else:
      rslt = self.read_reg(0, 30)
      data = self.anaysis_data(rslt)
      return data.exist

  def set_sensor_mode(self, mode):
    '''!
      @brief set_sensor_mode
      @param mode 
      @n  SPEED_MODE
      @n  EXIST_MODE
    '''
    if self.__uart_i2c == I2C_MODE:
      data = self.get_status()
      if data.work_mode == mode:
        return 0
      else:
        self.set_sensor(I2C_CHANGE_MODE)
        return 0
    else:
      self.write_reg(0, STOP_SENSOR)
      time.sleep(0.1)
      if mode == EXIST_MODE:
        self.write_reg(0, EXIST_SENSOR)
        time.sleep(0.1)
      else:
        self.write_reg(0, SPEED_SENSOR)
        time.sleep(0.1)
      self.write_reg(0, SAVE_CONFIG)
      time.sleep(0.1)
      self.write_reg(0, START_SENSOR)
      time.sleep(0.1)
      return 0

  def set_trig_sensitivity(self, sensitivity):
    '''!
      @brief set_trig_sensitivity
      @param sensitivity 0-9
    '''
    if sensitivity > 9:
      return False
    if self.__uart_i2c == I2C_MODE:
      send = [0]*1
      send[0] = sensitivity
      self.write_reg(REG_TRIG_SENSITIVITY, send)
      self.set_sensor(I2C_SAVE_SENSOR)
      return 0
    else:
      data = "setSensitivity 255 "
      data += str(sensitivity)
      self.write_cmd(data, data, 1)
      return 0

  def get_trig_sensitivity(self):
    '''!
      @brief get_trig_sensitivity
      @return sensitivity 0-9
    '''
    if self.__uart_i2c == I2C_MODE:
      rslt = self.read_reg(REG_TRIG_SENSITIVITY, 1)
      return rslt[0]
    else:
      data = "getSensitivity"
      response = self.wr_cmd(data, 1)
      return response.response1

  def set_keep_sensitivity(self, sensitivity):
    '''!
      @brief set_keep_sensitivity
      @param sensitivity 0-9
    '''
    if sensitivity > 9:
      return False
    if self.__uart_i2c == I2C_MODE:
      send = [0]*1
      send[0] = sensitivity
      self.write_reg(REG_KEEP_SENSITIVITY, send)
      self.set_sensor(I2C_SAVE_SENSOR)
    else:
      data = "setSensitivity "
      data += str(sensitivity)
      data += " 255"
      self.write_cmd(data, data, 1)
      return 0

  def get_keep_sensitivity(self):
    '''!
      @brief get_keep_sensitivity
      @return sensitivity 0-9
    '''
    if self.__uart_i2c == I2C_MODE:
      rslt = self.read_reg(REG_KEEP_SENSITIVITY, 1)
      return rslt[0]
    else:
      data = "getSensitivity"
      response = self.wr_cmd(data, 1)
      return response.response2


  def set_delay(self, trig, keep):
    '''!
      @brief set_delay
      @param trig (0-200)
      @n     Trigger delay, i2c mode unit 10ms range (0 ~200) (0s-2.0s)
      @n     Trigger delay, uart mode unit 500ms range (0 ~200) (0s-100.0s)
      @param keep (4~3000) (2s-1500s)
    '''
    if trig > 200:
      return False
    if keep < 4 or keep > 3000:
      return False
    if self.__uart_i2c == I2C_MODE:
      send = [0]*3
      send[0] = trig&0xff
      send[1] = keep&0xff
      send[2] = (keep>>8)&0xff
      self.write_reg(REG_TRIG_DELAY, send)
      self.set_sensor(I2C_SAVE_SENSOR)
    else:
      data = "setLatency "
      data += str(trig*0.5)
      data += " "
      data += str(keep*0.5)
      self.write_cmd(data, data, 1)
      return 0

  def get_trig_delay(self):
    '''!
      @brief get_trig_delay
      @return trig
    '''
    if self.__uart_i2c == I2C_MODE:
      rslt = self.read_reg(REG_TRIG_DELAY, 1)
      return rslt[0]
    else:
      data = "getLatency"
      response = self.wr_cmd(data, 2)
      return response.response1*2
    
  def get_keep_timerout(self):
    '''!
      @brief get_keep_timerout
      @return keep timerout
    '''
    if self.__uart_i2c == I2C_MODE:
      rslt = self.read_reg(REG_KEEP_TIMEOUT_L, 2)
      return rslt[1]*256 + rslt[0]
    else:
      data = "getLatency"
      response = self.wr_cmd(data, 2)
      return response.response2*2

  def set_detection_range(self, min, max):
    '''!
      @brief set_detection_range
      @param min (30-2000)
      @param max (240~2000)
      @n min not more than max, otherwise the function is not normal.
    '''
    if(max < 240 or max > 2000):
      return False
    if(min < 30 or min > max):
      return False
    if self.__uart_i2c == I2C_MODE:
      send = [0]*6
      send[0] = min & 0xFF
      send[1] = (min >> 8) & 0xFF
      send[2] = max & 0xFF
      send[3] = (max >> 8) & 0xFF
      send[4] = max & 0xFF
      send[5] = (max >> 8) & 0xFF
      self.write_reg(REG_E_MIN_RANGE_L, send)
      self.set_sensor(I2C_SAVE_SENSOR)
      return True
    else:
      data1 = "setRange "
      data2 = "setTrigRange "
      data1 += str(min / 100.0)
      data1 += " "
      data1 += str(max / 100.0)
      data2 += str(max / 100.0)
      self.write_cmd(data1, data2, 2)
      return True

  def get_trig_range(self):
    '''!
      @brief get_trig_range
      @return trig range
    '''
    if self.__uart_i2c == I2C_MODE:
      rslt = self.read_reg(REG_E_TRIG_RANGE_L, 2)
      return rslt[1]*256 + rslt[0]
    else:
      data = "getTrigRange"
      response = self.wr_cmd(data, 1)
      return response.response1*100


  def get_max_range(self):
    '''!
      @brief get_max_range
      @return max range
    '''
    if self.__uart_i2c == I2C_MODE:
      rslt = self.read_reg(REG_E_MAX_RANGE_L, 2)
      return rslt[1]*256 + rslt[0]
    else:
      data = "getRange"
      response = self.wr_cmd(data, 2)
      return response.response2*100

  def get_min_range(self):
    '''!
      @brief get_max_range
      @return min range
    '''
    if self.__uart_i2c == I2C_MODE:
      rslt = self.read_reg(REG_E_MIN_RANGE_L, 2)
      return rslt[1]*256 + rslt[0]
    else:
      data = "getRange"
      response = self.wr_cmd(data, 2)
      return response.response1*100

  def get_target_number(self):
    '''!
      @brief get_target_number
      @return target number 
    '''
    if self.__uart_i2c == I2C_MODE:
      rslt = self.read_reg(REG_RESULT_OBJ_MUN, 7)
      if rslt[0] == 1:
        self.__speed_null_count = 0
        self.__all_data.number = rslt[0]
        self.__all_data.range  = (rslt[1] + rslt[2]*256) 
        self.__all_data.speed  = (rslt[3] + rslt[4]*256)
        if self.__all_data.range > 32768:
          self.__all_data.range = (int(self.__all_data.range - 65535)) / 1000.0
        else:
          self.__all_data.range /= 1000.0
        if self.__all_data.speed > 32768:
          self.__all_data.speed = (int(self.__all_data.speed - 65535)) / 1000.0
        else:
          self.__all_data.speed /= 1000.0
            
        self.__all_data.energy = rslt[5] + rslt[6]*256
      else:
        if self.__speed_null_count > 10:
          self.__all_data.number = 0
          self.__all_data.range  = 0 
          self.__all_data.speed  = 0
          self.__all_data.energy = 0  
        self.__speed_null_count += 1
      return self.__all_data.number
    else:
      rslt = self.read_reg(0, 70)
      all_data = self.anaysis_data(rslt)
      return all_data.number

  def get_target_speed(self):
    '''!
      @brief get_target_speed
      @return target speed
    '''
    return self.__all_data.speed

  def get_target_range(self):
    '''!
      @brief get_target_range
      @return target range
    '''
    return self.__all_data.range

  def get_target_energy(self):
    '''!
      @brief get_target_energy
      @return target energy
    '''
    return self.__all_data.energy

  def set_detect_thres(self, min, max, thres):
    '''!
      @brief set_detect_thres
      @param min 0-2500
      @param max 0-2500
      @param thres 0-65535
    '''
    if max > 2500:
      return False
    if min > max:
      return False
    if self.__uart_i2c == I2C_MODE:
      send = [0]*6
      send[0] = thres & 0xFF
      send[1] = (thres >> 8) & 0xFF
      send[2] = min & 0xFF
      send[3] = (min >> 8) & 0xFF
      send[4] = max & 0xFF
      send[5] = (max >> 8) & 0xFF
      self.write_reg(REG_CFAR_THR_L, send)
      self.set_sensor(I2C_SAVE_SENSOR)
    else:
      data1 = "setRange "
      data2 = "setThrFactor "
      data1 += str(min/100.0)
      data1 += " "
      data1 += str(max/100.0)
      data2 += str(thres)    
      self.write_cmd(data1, data2, 2)


  def set_io_polaity(self, value):
    '''!
      @brief set_io_polaity
      @param value 0 or 1
      @n   0: Low output when there is a target, high output when there is no target
      @n   1: Output high when there is a target, output low when there is no target (default state)
    '''
    if value > 1:
      return False
    if self.__uart_i2c == UART_MODE:
      data = "setGpioMode 1 "
      data += str(value)
      self.write_cmd(data, data, 1)
    return True

  def get_io_polaity(self):
    '''!
      @brief get_io_polaity
      @return io status
    '''
    if self.__uart_i2c == I2C_MODE:
      return 1
    else:
      data = "getGpioMode 1"
      response = self.wr_cmd(data, 2)
      return response.response2

  def set_pwm(self, pwm1, pwm2, timer):
    '''!
      @brief set_pwm
      @param pwm1 Duty cycle of the output signal of the OUT pin when the target is not detected. The value ranges from 0 to 100
      @param pwm2 Duty cycle of the output signal of the OUT pin after the target is detected. The value ranges from 0 to 100
      @param Time from pwm1 duty cycle to pwm2 duty cycle. The value ranges from 0 to 255, corresponding to the time value = timer*64ms
      @n     For example, timer=20, it takes 20*64ms=1.28s for duty cycle to change from pwm1 to pwm2.
    '''
    if pwm1 > 100 or pwm2 > 100:
      return False
    if self.__uart_i2c == UART_MODE:
      data = "setPwm "
      data += str(pwm1)
      data += " "
      data += str(pwm2)
      data += " "
      data += str(timer)
      self.write_cmd(data, data, 1)
    return True

  def get_pwm(self):
    '''!
      @brief get_pwm
      @return pwm value
    '''
    pwm_data = struct_pwm_data()
    if self.__uart_i2c == I2C_MODE:
      return pwm_data
    else:
      data = "getPwm"
      response = struct_response_data()
      response = self.wr_cmd(data, 3)
      pwm_data.pwm1 = response.response1
      pwm_data.pwm2 = response.response2
      pwm_data.timer = response.response3
      return pwm_data

  def get_tmin_range(self):
    '''!
      @brief get_tmin_range
      @return speed mode min range
    '''
    if self.__uart_i2c == I2C_MODE:
      return 1
    else:
      data = "getRange"
      response = self.wr_cmd(data, 1)
      return response.response1*100

  def get_tmax_range(self):
    '''!
      @brief get_tmax_range
      @return speed mode max range
    '''
    if self.__uart_i2c == I2C_MODE:
      return 1
    else:
      data = "getRange"
      response = self.wr_cmd(data, 2)
      return response.response2*100

  def get_thres_range(self):
    '''!
      @brief get_thres_range
      @return speed mode thres range
    '''
    if self.__uart_i2c == I2C_MODE:
      self.read_reg(REG_CFAR_THR_L,2)  
    else:
      data = "getThrFactor"
      response = self.wr_cmd(data, 1)
      return response.response1
    
  def set_fretting_detection(self, status):
    '''!
      @brief set_fretting_detection
      @param status
      @n    FRETTING_ON
      @n    FRETTING_OFF
    '''
    if self.__uart_i2c == I2C_MODE:
      send = [0]*1
      send[0] = status
      self.write_reg(REG_MICRO_MOTION, send)
      self.set_sensor(I2C_SAVE_SENSOR)
    else:
      data = "setMicroMotion "
      if status == FRETTING_ON or status == FRETTING_OFF:
        data = data + str(status)
        self.write_cmd(data, data, 1)

  def get_fretting_detection(self):
    '''!
      @brief get_fretting_detection
      @return status
    '''
    if self.__uart_i2c == I2C_MODE:
      rslt = self.read_reg(REG_MICRO_MOTION, 1)
      return rslt[0]
    else:
      data = "getMicroMotion"
      response = self.wr_cmd(data, 1)
      return response.response1
      

  def anaysis_data(self, data):
    try:
      str_data = data.decode('utf-8')
    except ValueError:
      return self.__all_data
    response_exist = str_data.find("$DFHPD")
    response_speed = str_data.find("$DFDMD")
    if response_exist != -1:
      self.__all_data.work_mode = EXIST_MODE
      self.__all_data.work_status = 1
      self.__all_data.init_status = 1
      self.__all_data.exist = int(str_data[7+response_exist])
    if response_speed != -1:
      self.__all_data.work_mode = SPEED_MODE
      self.__all_data.work_status = 1
      self.__all_data.init_status = 1
      data_split = str_data[response_speed+7:].split(',')
      if int(data_split[0]) == 1:
        self.__speed_null_count = 0
        self.__all_data.number = int(data_split[0])
        try:
          self.__all_data.speed  = float(data_split[2])
        except:
          pass
        try:
          self.__all_data.range  = float(data_split[3])
        except:
          pass
        try:
          self.__all_data.energy = int(data_split[4])
        except:
          pass
        
      else:
        if self.__speed_null_count < 10:
          self.__speed_null_count += 1
        else:
          self.__all_data.number = 0
          self.__all_data.speed  = 0
          self.__all_data.range  = 0
          self.__all_data.energy = 0
    return self.__all_data
  

  def anaysis_response(self, data):
    response = struct_response_data()
    try:
      str_data = data.decode('utf-8')
    except ValueError:
      return response
    response_index = str_data.find("Response")
    if response_index != -1:
      # Extract parameters starting after "Response"
      response.status = True
      parameters_str = str_data[response_index + 8:].strip()
      parameters = parameters_str.split()
      num_parameters = 0
      for _ in parameters:
        num_parameters += 1
      if num_parameters >= 1:
        try:
          float(parameters[0])  # Attempts to convert arguments to floating point numbers
          response.response1 = float(parameters[0])
        except ValueError:
          pass  # If the argument cannot be converted to floating-point, it is ignored
      if num_parameters >= 2:
        try:
          float(parameters[1])  # Attempts to convert arguments to floating point numbers
          response.response2 = float(parameters[1])
        except ValueError:
          pass  # If the argument cannot be converted to floating-point, it is ignored
      if num_parameters >= 3:
        try:
          float(parameters[2])  # Attempts to convert arguments to floating point numbers
          response.response3 = float(parameters[2])
        except ValueError:
          pass  # If the argument cannot be converted to floating-point, it is ignored
    else:
      response.status = False
    return response



  def wr_cmd(self, cmd1, count):
    response = struct_response_data()
    self.write_reg(0, STOP_SENSOR)
    time.sleep(0.1)
    self.write_reg(0, cmd1)
    rslt = self.read_reg(0, 50)
    len  = 1
    response = self.anaysis_response(rslt)
    time.sleep(0.05)
    self.write_reg(0, START_SENSOR)
    time.sleep(0.05)
    return response

  def write_cmd(self, cmd1, cmd2, count):
    self.write_reg(0, STOP_SENSOR)
    time.sleep(0.1)
    self.write_reg(0, cmd1)
    time.sleep(0.05)
    if count > 1:
      self.write_reg(0, cmd2)
      time.sleep(0.05)
    self.write_reg(0, SAVE_CONFIG)
    time.sleep(0.05) 
    self.write_reg(0, START_SENSOR)
    time.sleep(0.1)

class DFRobot_C4001_I2C(DFRobot_C4001): 
  def __init__(self, bus, addr):
    self.__addr = addr
    super(DFRobot_C4001_I2C, self).__init__(bus,0)

  def write_reg(self, reg, data):
    while 1:
      try:
        self.i2cbus.write_i2c_block_data(self.__addr, reg, data)
        return
      except:
        print("please check connect!")
        time.sleep(1)

  def read_reg(self, reg, len):
    try:
      rslt = self.i2cbus.read_i2c_block_data(self.__addr, reg, len)
    except:
      rslt = -1
    return rslt

class DFRobot_C4001_UART(DFRobot_C4001):

  def __init__(self, Baud):
    self.__uart_i2c = UART_MODE
    self.__Baud = Baud 
    super(DFRobot_C4001_UART, self).__init__(0, Baud)

  def write_reg(self, reg, data):
    test = bytes(data, encoding='ascii')
    self.ser.flushInput()
    self.ser.write(test)
    return

  def read_reg(self, reg, len):
    recv = [0]*len
    timenow = time.time()
    
    while(time.time() - timenow) <= 1:
      count = self.ser.inWaiting()
      if count != 0:
        recv = self.ser.read(len)
        self.ser.flushInput()
        return recv
    return recv
