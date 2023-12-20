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

#define I2C_COMMUNICATION  //use I2C for communication, but use the serial port for communication if the line of codes were masked

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
    DFRobot_RS20XU_UART radar(&mySerial ,57600);
  #elif defined(ESP32)
    DFRobot_RS20XU_UART radar(&Serial1 ,57600 ,/*rx*/D2 ,/*tx*/D3);
  #else
    DFRobot_RS20XU_UART radar(&Serial1 ,57600);
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
   * @param min 检测范围最小距离，单位cm，范围0.3~20m（30~2000），不超过 max，否则功能不正常。
   * @param max 检测范围最大距离，单位cm，范围2.4~20m（240~2000）
   * @param trig 触发距离，单位cm，范围2.4~20m（240~2000），实际生效的配置范围不超出检测距离的最大距离和最小距离。
   */
  if(radar.setDetectionRange(50, 1000, 240)){
    Serial.println("set detection range successfully!");
  }
  // set trigger sensitivity 0 - 9
  if(radar.setTrigSensitivity(1)){
    Serial.println("set trig sensitivity successfully!");
  }

  // set keep sensitivity 0 - 9
  if(radar.setKeepSensitivity(7)){
    Serial.println("set keep sensitivity successfully!");
  }
  // 触发延时，单位0.01s，范围0~2s（0~200）
  if(radar.setTrigDelay(10)){
    Serial.println("set trigger dalay successfully!");
  }
  // 维持检测超时，单位0.5s，范围2~1500秒（4~3000）
  if(radar.setKeepTimerout(4)){
    Serial.println("set trigger dalay successfully!");
  }

  // 保存参数
  radar.setSensor(eSaveParams);

  // 获取配置
  Serial.print("trig sensitivity = ");
  Serial.println(radar.getTrigSensitivity());
  Serial.print("keep sensitivity = ");
  Serial.println(radar.getKeepSensitivity());

  Serial.print("min range = ");
  Serial.println(radar.getMinRange());
  Serial.print("max range = ");
  Serial.println(radar.getMaxRange());
  Serial.print("trigger range = ");
  Serial.println(radar.getTrigRange());

  Serial.print("keep time = ");
  Serial.println(radar.getKeepTimerout());

  Serial.print("trig delay = ");
  Serial.println(radar.getTrigDelay());

  // 开始测量
  radar.setSensor(eStartSen);

}

void loop()
{
  // 判断物体是否运动
  if(radar.motionDetection()){
    Serial.println("exit motion");
    Serial.println();
  }
  delay(1000);
}