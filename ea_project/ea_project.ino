//藍牙初始化
#include "BluetoothSerial.h"  
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;


//Motor 針腳定數
const int motorL_pin0 = 19;
const int motorL_pin1 = 21;
const int motorR_pin0 = 4;
const int motorR_pin1 = 16;

//RGB LED 針腳定數
const int R = 25;    
const int G = 26;
const int B = 27;

//PWM properties
const int freq = 1020;      //Frequency for Motor
const int resolution = 10;  //  10 bits, 0 - 1024 level for Motor
const int resolutionLED = 8;  //  8 bits, 0 - 255 level for LED

//設定Channel參數
/*Channel 取值為0 ~ 15，即可設定16個通道，
其中高速通道（0 ~ 7）由80MHz驅動，
低速通道（8 ~ 15）由 1MHz驅動*/
//摩打
const int motorL0 = 0;         // Channel 0
const int motorL1 = 1;         // Channel 1
const int motorR0 = 2;         // Channel 2
const int motorR1 = 3;         // Channel 3
//LED
const int ledR = 8;         // Channel 4 
const int ledG = 9;         // Channel 5 
const int ledB = 10;         // Channel 6 

int xAxis=140, yAxis=140;

int motorSpeedA = 0;
int motorSpeedB = 0;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_YeungWing"); // Default communication rate of the Bluetooth module

  ledcSetup(motorL0, freq, resolution); 
  ledcSetup(motorL1, freq, resolution); 
  ledcSetup(motorR0, freq, resolution); 
  ledcSetup(motorR1, freq, resolution); 
  delay(500);

  // 連接摩打至channel
  ledcAttachPin(motorL_pin0, motorL0);     //設定左摩打19號腳用Channel
  ledcAttachPin(motorR_pin1, motorR0);    //設定左摩打21號腳用Channel
  ledcAttachPin(motorL_pin1, motorL1);    //設定右摩打4號腳用Channel
  ledcAttachPin(motorR_pin0, motorR1);   //設定右摩打16號腳用Channel

  //For LED setting: 
  //(Channel, Frequency, Bits) 
  ledcSetup(ledR, 255, resolutionLED);
  ledcSetup(ledG, 255, resolutionLED);
  ledcSetup(ledB, 255, resolutionLED);
  //(LED, Channel) 
  ledcAttachPin(R, ledR);
  ledcAttachPin(G, ledG);
  ledcAttachPin(B, ledB); 
}

void loop() {
  // Default value - no movement when the Joystick stays in the center
  //xAxis = 140;
  //yAxis = 140;

  // Read the incoming data from the Smartphone Android App
  while (SerialBT.available() >= 2) {
    xAxis = SerialBT.read();
    delay(10);
    yAxis = SerialBT.read();
   Serial.print(xAxis);
   Serial.print(",");
   Serial.println(yAxis);
  }
  delay(10);
  
  // Makes sure we receive corrent values

if (xAxis > 130 && xAxis <150 && yAxis > 130 && yAxis <150){Stop();}


if (yAxis > 130 && yAxis <150){    

if (xAxis < 130){turnRight();
motorSpeedA = map(xAxis, 130, 60, 0, 255);
motorSpeedB = map(xAxis, 130, 60, 0, 255);    
}

if (xAxis > 150) {turnLeft();
motorSpeedA = map(xAxis, 150, 220, 0, 255);
motorSpeedB = map(xAxis, 150, 220, 0, 255); 
}

}else{

if (xAxis > 130 && xAxis <150){   

if (yAxis < 130){forword();}
if (yAxis > 150){backword();}

if (yAxis < 130){
motorSpeedA = map(yAxis, 130, 60, 0, 255);
motorSpeedB = map(yAxis, 130, 60, 0, 255); 
}

if (yAxis > 150){
motorSpeedA = map(yAxis, 150, 220, 0, 255);
motorSpeedB = map(yAxis, 150, 220, 0, 255);
 }
 
}else{

if (yAxis < 130){forword();}
if (yAxis > 150){backword();}

if (xAxis < 130){
motorSpeedA = map(xAxis, 130, 60, 255, 50);
motorSpeedB = 255; 
 }
 
if (xAxis > 150){
motorSpeedA = 255;
motorSpeedB = map(xAxis, 150, 220, 255, 50); 
  }
 } 
}
motorSpeedA = motorSpeedA/255*1020;
motorSpeedB = motorSpeedB/255*1020;

   //Serial.print(motorSpeedA);
   //Serial.print(",");
   //Serial.println(motorSpeedA);

}


void forword(){Serial.println("forword");
ledcWrite(motorL1, motorSpeedA);
ledcWrite(motorL0, 0); 
ledcWrite(motorR1, motorSpeedB);
ledcWrite(motorR0, 0);
}

void backword(){Serial.println("backword");
ledcWrite(motorL1, 0);
ledcWrite(motorL0, motorSpeedA); 
ledcWrite(motorR1, 0);
ledcWrite(motorR0, motorSpeedB);
}

void turnRight(){Serial.println("turnRight");
ledcWrite(motorL1, motorSpeedA);
ledcWrite(motorL0, 0); 
ledcWrite(motorR1, 0);
ledcWrite(motorR0, motorSpeedB);
}

void turnLeft(){Serial.println("turnLeft");
ledcWrite(motorL1, 0);
ledcWrite(motorL0, motorSpeedA); 
ledcWrite(motorR1, motorSpeedB);
ledcWrite(motorR0, 0);
}

void Stop(){
ledcWrite(motorL1, 0);
ledcWrite(motorL0, 0); 
ledcWrite(motorR1, 0);
ledcWrite(motorR0, 0);
Serial.println("stop");
}
