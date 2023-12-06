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
const int freq = 1024;      //Frequency for Motor
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

int r, g, b;
int xAxis=140, yAxis=140;

int motorSpeedA = 0;
int motorSpeedB = 0;
double degree = -1;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_YeungWing"); // Default communication rate of the Bluetooth module

  ledcSetup(motorL0, freq, resolution); 
  ledcSetup(motorL1, freq, resolution); 
  ledcSetup(motorR0, freq, resolution); 
  ledcSetup(motorR1, freq, resolution); 

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
  while (SerialBT.available() >= 5) {
    xAxis = SerialBT.read();
    yAxis = SerialBT.read();
    r = SerialBT.read();
    g = SerialBT.read();
    b  = SerialBT.read();
  }
  
  // Makes sure we receive corrent values

ledcWrite(ledR, r);
ledcWrite(ledG, g);
ledcWrite(ledB, b);

degree=-1;

if (xAxis > 139 && xAxis <141 && yAxis > 139 && yAxis <141){Stop();}


if (yAxis > 139 && yAxis <141){    

  if (xAxis < 139){;
    motorSpeedA = map(xAxis, 139, 60, 0, 1024);
    motorSpeedB = map(xAxis, 139, 60, 0, 1024); 
    turnLeft();   
  }

  if (xAxis > 141) {
    motorSpeedA = map(xAxis, 141, 220, 0, 1024);
    motorSpeedB = map(xAxis, 141, 220, 0, 1024); 
    turnRight();
  }

}else{

  if (xAxis > 139 && xAxis <141){   

    if (yAxis < 139){
      motorSpeedA = map(yAxis, 139, 60, 0, 1024);
      motorSpeedB = map(yAxis, 139, 60, 0, 1024); 
      forward();
    }

    if (yAxis > 141){
      motorSpeedA = map(yAxis, 141, 220, 0, 1024);
      motorSpeedB = map(yAxis, 141, 220, 0, 1024);
      backward();
    }
 
  }else{

    if (xAxis < 139){
      // View calc.png
      if (yAxis<139){
        degree = atan((140-xAxis)/(140-yAxis));
        Serial.print(degree);
        if (degree==-1){
        }else if (degree<=0.785){
          motorSpeedA = map(xAxis, 139, 60, map(yAxis, 139, 60, 0, 1024), 0);
          motorSpeedB = map(yAxis, 139, 60, 0, 1024);
          forward();
          Serial.println("ForwardL");
        }else{
          motorSpeedA = map(yAxis, 141, 60, map(xAxis, 139, 60, 0, 1024), 0);
          motorSpeedB = map(xAxis, 139, 60, 0, 1024); 
          turnLeft();
          Serial.println("LeftF");
        }
      }

      if (yAxis>141){
        degree = atan((140-xAxis)/(yAxis-140));
        Serial.print(degree);
        if (degree==-1){
        }else if (degree<=0.785){
          motorSpeedA = map(yAxis, 141, 220, 0, 1024);
          motorSpeedB = map(xAxis, 139, 60, map(yAxis, 141, 220, 0, 1024), 0);
          backward();
          Serial.println("backwardL");
        }else{
          motorSpeedA = map(xAxis, 139, 60, 0, 1024); 
          motorSpeedB = map(yAxis, 141, 220, map(xAxis, 139, 60, 0, 1024), 0);
          turnLeft();
          Serial.println("LeftB");
        }
      }
    }else if (xAxis > 141){
      if (yAxis<139){
        degree = atan((xAxis-140)/(140-yAxis));
        Serial.print(degree);
        if (degree==-1){
        }else if (degree<=0.785){
          motorSpeedA = map(yAxis, 139, 60, 0, 1024);
          motorSpeedB = map(xAxis, 141, 220, map(yAxis, 139, 60, 0, 1024), 0);
          forward();
          Serial.println("ForwardR");
        }else{
          motorSpeedA = map(xAxis, 141, 220, 0, 1024); 
          motorSpeedB = map(yAxis, 139, 60, map(xAxis, 141, 220, 0, 1024), 0);
          turnRight();
          Serial.println("RightF");
        }
      }
      
      if (yAxis>141){
        degree = atan((xAxis-140)/(yAxis-140));
        Serial.print(degree);
        if (degree==-1){
        }else if (degree<=0.785){
          motorSpeedA = map(yAxis, 141, 220, 0, 1024);
          motorSpeedB = map(xAxis, 141, 220, map(yAxis, 141, 220, 0, 1024), 0);
          backward();
          Serial.println("backwardR");
        }else{
          motorSpeedA = map(yAxis, 141, 220, map(xAxis, 141, 220, 0, 1024), 0);
          motorSpeedB = map(xAxis, 141, 220, 0, 1024); 
          turnRight();
          Serial.println("RightB");
        }
      }
    }


  } 
}

   //Serial.print(motorSpeedA);
   //Serial.print(",");
   //Serial.println(motorSpeedA);

}

void forward(){Serial.println("forward");
  ledcWrite(motorL1, motorSpeedA);
  ledcWrite(motorL0, 0); 
  ledcWrite(motorR1, motorSpeedB);
  ledcWrite(motorR0, 0);
}

void backward(){Serial.println("backward");
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
}
