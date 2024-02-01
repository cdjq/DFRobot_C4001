 /*!
  * @file  motionDetection.ino
  * @brief  radar recognition demo
  * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  * @license The MIT License (MIT)
  * @author ZhixinLiu(zhixin.liu@dfrobot.com)
  * @version V1.0
  * @date 2023-12-07
  * @url https://github.com/dfrobot/DFRobot_RS20XU
  */

#include "DFRobot_RS20XU.h"

//#define I2C_COMMUNICATION  //use I2C for communication, but use the serial port for communication if the line of codes were masked

#ifdef  I2C_COMMUNICATION
  DFRobot_RS20XU_I2C radar(&Wire ,DEVICE_ADDR_0);
#else
/* ---------------------------------------------------------------------------------------------------------------------
 *    board   |             MCU                | Leonardo/Mega2560/M0 |    UNO    | ESP8266 | ESP32 |  microbit  |   m0  |
 *     VCC    |            3.3V/5V             |        VCC           |    VCC    |   VCC   |  VCC  |     X      |  vcc  |
 *     GND    |              GND               |        GND           |    GND    |   GND   |  GND  |     X      |  gnd  |
 *     RX     |              TX                |     Serial1 TX1      |     5     |   5/D6  |  D2   |     X      |  tx1  |
 *     TX     |              RX                |     Serial1 RX1      |     4     |   4/D7  |  D3   |     X      |  rx1  |
 * ----------------------------------------------------------------------------------------------------------------------*/
/* Baud rate cannot be changed */
  #if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
    SoftwareSerial mySerial(4, 5);
    DFRobot_RS20XU_UART radar(&mySerial ,115200);
  #elif defined(ESP32)
    DFRobot_RS20XU_UART radar(&Serial1 ,115200 ,/*rx*/D2 ,/*tx*/D3);
  #else
    DFRobot_RS20XU_UART radar(&Serial1 ,115200);
  #endif
#endif

void setup()
{
  Serial.begin(115200);
  while(!radar.begin()){
    Serial.println("NO Deivces !");
    delay(1000);
  }
  Serial.println("Device connected!");

  // exit Mode
  radar.setSensorMode(eExitMode);


  sSensorStatus_t data;
  data = radar.getStatus();
  //  0 stop  1 start
  Serial.print("work status  = ");
  Serial.println(data.workStatus);

  //  0 为存在检测   1 为测速测距
  Serial.print("work mode  = ");
  Serial.println(data.workMode);

  //  0 未初始化   1 初始化完成
  Serial.print("init status = ");
  Serial.println(data.initStatus);
  Serial.println();

  /*
   * @param min 检测范围最小距离，单位cm，范围（30~2000），不超过 max，否则功能不正常。
   * @param max 检测范围最大距离，单位cm，范围（240~2000）
   */
  if(radar.setDetectionRange(50, 1000)){
    Serial.println("set detection range successfully!");
  }
  // set trigger sensitivity 0 - 9
  if(radar.setTrigSensitivity(1)){
    Serial.println("set trig sensitivity successfully!");
  }

  // set keep sensitivity 0 - 9
  if(radar.setKeepSensitivity(2)){
    Serial.println("set keep sensitivity successfully!");
  }
  /*
   * 触发延时，单位10ms，范围（0~200）
   * 维持检测超时，单位500ms，范围（4~3000）
   */
  if(radar.setDelay(/*trig*/24, /*keep*/44)){
    Serial.println("set delay successfully!");
  }
  
  /*
   *        未检测到目标时，OUT引脚输出信号的占空比，取值范围：0～100
   * @param pwm2 
   *        检测到目标后，OUT引脚输出信号的占空比，取值范围：0～100
   * @param timer 
   *        从pwm1 占空比渐变为pwm2 占空比的时间，取值范围：0～255，对应时间值 = timer*64ms
   *        如timer=20，占空比从pwm1渐变为pwm2需要 20*64ms=1.28s。
   */
  if(radar.setPwm(10, 10, 10)){
    Serial.println("set pwm period successfully!");
  }

  /*
   * 设置pwm 极性 
   *  0：有目标时输出低电平，无目标时输出高电平
   *  1：有目标时输出高电平，无目标时输出低电平（默认状态）
   */
  if(radar.setIoPolaity(1)){
    Serial.println("set Io Polaity successfully!");
  }


  // 获取配置
  Serial.print("trig sensitivity = ");
  Serial.println(radar.getTrigSensitivity());
  Serial.print("keep sensitivity = ");
  Serial.println(radar.getKeepSensitivity());

  Serial.print("min range = ");
  Serial.println(radar.getMinRange());
  Serial.print("max range = ");
  Serial.println(radar.getMaxRange());

  Serial.print("keep time = ");
  Serial.println(radar.getKeepTimerout());

  Serial.print("trig delay = ");
  Serial.println(radar.getTrigDelay());

  Serial.print("polaity = ");
  Serial.println(radar.getIoPolaity());

  sPwmData_t pwmData;
  pwmData = radar.getPwm();
  Serial.print("pwm1 = ");
  Serial.println(pwmData.pwm1);
  Serial.print("pwm2 = ");
  Serial.println(pwmData.pwm2);
  Serial.print("pwm timer = ");
  Serial.println(pwmData.timer);
}

void loop()
{
  // 判断物体是否运动
  if(radar.motionDetection()){
    Serial.println("exit motion");
    Serial.println();
  }
  delay(100);
}