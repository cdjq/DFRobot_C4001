# -*- coding: utf-8 -*
'''!
  @file motion_range_velocity.py
  @brief read range velocity data
  @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license    The MIT License (MIT)
  @author     [ZhixinLiu](zhixin.liu@dfrobot.com)
  @version    V1.0
  @date       2024-2-20
  @url https://github.com/DFRobot/DFRobot_C4001
'''
from __future__ import print_function
import sys
import os
sys.path.append("../")
import time
import RPi.GPIO as GPIO
from DFRobot_C4001 import *

'''
  Select to use i2c or UART
  I2C_MODE
  UART_MODE
'''
ctype = UART_MODE

if ctype == I2C_MODE:
  I2C_1 = 0x01
  '''
    I2C_ADDR 0X2A or 0x2B
  '''
  I2C_ADDR = 0X2A
  radar = DFRobot_C4001_I2C (I2C_1, I2C_ADDR)
elif ctype == UART_MODE:
  radar = DFRobot_C4001_UART(9600)


def setup():
  while (radar.begin() == False):
    print("Sensor initialize failed!!")
    time.sleep(1)
  
  '''
    EXIST_MODE
    SPEED_MODE
  '''
  radar.set_sensor_mode(SPEED_MODE)

  # The minimum distance of detection range, the unit is cm, the range (0~2500), does not exceed max, otherwise the work is not normal
  # Detection range Maximum distance, unit cm, range (0~2500)
  # Target detection threshold, no dimensional unit 0.1, range 0~6553.5 (0~65535)
  radar.set_detect_thres(11, 1200, 11)
  
  # FRETTING_ON
  # FRETTING_OFF
  radar.set_fretting_detection(FRETTING_ON)
  
  # get config
  print("min range = " + str(radar.get_tmin_range()) )
  print("max range = " + str(radar.get_tmax_range()) )
  print("thres range = " + str(radar.get_thres_range()) )
  print("fretting detection = " + str(radar.get_fretting_detection()) )
  
  
def loop():
  print("target number = " + str(radar.get_target_number()) )
  print("target speed = "  + str(radar.get_target_speed()) + " m/s")
  print("target range = "  + str(radar.get_target_range()) + " m")
  print("target energy = " + str(radar.get_target_energy()) )
  time.sleep(0.1)

if __name__ == "__main__":
  setup()
  while True:
    loop()
