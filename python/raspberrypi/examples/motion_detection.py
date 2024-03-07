# -*- coding: utf-8 -*
'''!
  @file motion_detection.py
  @brief motion detection data
  @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license    The MIT License (MIT)
  @author     [ZhixinLiu](zhixin.liu@dfrobot.com)
  @version    V1.0
  @date       2024-02-20
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
ctype = I2C_MODE

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
  radar.set_sensor_mode(EXIST_MODE)

  # The minimum distance of detection range, the unit is cm, the range (0~2500), does not exceed max, otherwise the work is not normal
  # Detection range Maximum distance, unit cm, range (0~2500)
  # Target detection threshold, no dimensional unit 0.1, range 0~6553.5 (0~65535)
  radar.set_detect_thres(11, 1200, 11)
  
  # min Detection range Minimum distance, unit cm, range (30~2000), not more than max, otherwise the function is not normal.
  # max Detection range Maximum distance, unit cm, range (240~2000)
  radar.set_detection_range(50, 1000)
  
  # set trigger sensitivity 0 - 9
  radar.set_trig_sensitivity(1)
  
  # set keep sensitivity 0 - 9
  radar.set_keep_sensitivity(1)
  
  # Trigger delay, i2c mode unit 10ms range (0 ~200) (0s-2.0s)
  # Trigger delay, uart mode unit 500ms range (0 ~200) (0s-100.0s)
  # Maintain detection timeout, unit 500ms, range (4~3000) (2s-1500s)
  radar.set_delay(24, 4)
  
  # pwm1 Duty cycle of the output signal of the OUT pin when the target is not detected. The value ranges from 0 to 100
  # pwm2 Duty cycle of the output signal of the OUT pin after the target is detected. The value ranges from 0 to 100
  # param timer
  #     Time from pwm1 duty cycle to pwm2 duty cycle. The value ranges from 0 to 255, corresponding to the time value = timer*64ms
  #     For example, timer=20, it takes 20*64ms=1.28s for duty cycle to change from pwm1 to pwm2.
  radar.set_pwm(10, 10, 10)
  
  # Set pwm polarity
  # 0: Low output when there is a target, high output when there is no target
  # 1: Output high when there is a target, output low when there is no target (default state)
  radar.set_io_polaity(1)
  
  # get config parameters
  print("trig sensitivity = " + str(radar.get_trig_sensitivity()) )
  print("keep sensitivity = " + str(radar.get_keep_sensitivity()) )
  print("min range = " + str(radar.get_min_range()) )
  print("max range = " + str(radar.get_max_range()) )
  print("keep time = " + str(radar.get_keep_timerout()) )
  print("trig delay = " + str(radar.get_trig_delay()) )
  print("polaity = " + str(radar.get_io_polaity()) )
  pwm = struct_pwm_data()
  pwm = radar.get_pwm()
  print("pwm1  = " + str(pwm.pwm1) )
  print("pwm2  = " + str(pwm.pwm2) )
  print("timer = " + str(pwm.timer) )
  
  
def loop():
  if 1 == radar.motion_detection():
    print("montion exist")
  time.sleep(0.1)

if __name__ == "__main__":
  setup()
  while True:
    loop()
