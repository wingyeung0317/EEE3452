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

// input buffer
char buf[10] = {0};
int i = 0;
int reverseSpeed = 0;

// init
bool stringComplete = false;

void setup() {
  Serial.begin(115200);
  while (!Serial)
  {
    // Wait for serial port to connect, needed for native USB port only
  }
  // SerialBT.begin("ESP32_YeungWing");
  
  //初始化Motor設置
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

void loop() 
{
  while (Serial.available())
  {                                         
    char btInput = (char)Serial.read();

    if (btInput == '\n'){
      buf[i++] = btInput;
      buf[i] = 0;
      stringComplete = true;
    }else{
      buf[i++] = btInput;
    }

    
  if (stringComplete){
    Serial.print(buf);    // the printing of string will be stopped when zero is reached
    stringComplete = false;
    i = 0;
  }

  switch (buf[0]){
    case 'f': //f11 f12 ... f69   [0]=forward [1]=檔 [2]=speed
      forward(1133/9*(3+(buf[1])-'0')*(buf[2]-'0')/10);
      Serial.println(1133/9*(3+(buf[1])-'0')*(buf[2]-'0')/10);
      break;
    case 'l': //f11 f12 ... f69   [0]=forward [1]=檔 [2]=speed
      if (buf[1]=='0'){
        left(1133*(buf[2]-'0')/10);
      }
      leftS(1133/9*(3+(buf[1])-'0')*(buf[2]-'0')/10, ((buf[2]-'0')%10)*10);
      break;
    case 'r': //f11 f12 ... f69   [0]=forward [1]=檔 [2]=speed
      if (buf[1]=='0'){
        left(1133*(buf[2]-'0')/10);
      }
      rightS(1133/9*(3+(buf[1])-'0')*(buf[2]-'0')/10, ((buf[2]-'0')%10)*10);
      break;
    
    default:
      stop();
      break;
  }
  
  }

}

void forward(int speed){
  ledcWrite(motorL0, 0);
  ledcWrite(motorL1, speed);
  ledcWrite(motorR0, 0);
  ledcWrite(motorR1, speed);
}

void back(int speed){
  ledcWrite(motorL0, speed);
  ledcWrite(motorL1, 0);
  ledcWrite(motorR0, speed);
  ledcWrite(motorR1, 0);
}

void left(int speed){
  ledcWrite(motorL0, 0);
  ledcWrite(motorL1, speed);
  ledcWrite(motorR0, 0);
  ledcWrite(motorR1, 0);
}

void leftS(int speed, int swift){
  ledcWrite(motorL0, 0);
  ledcWrite(motorL1, speed);
  ledcWrite(motorR0, 0);
  ledcWrite(motorR1, speed/swift);
}

void right(int speed){
  ledcWrite(motorL0, 0);
  ledcWrite(motorL1, 0);
  ledcWrite(motorR0, 0);
  ledcWrite(motorR1, speed);
}

void rightS(int speed, int swift){
  ledcWrite(motorL0, 0);
  ledcWrite(motorL1, speed/swift);
  ledcWrite(motorR0, 0);
  ledcWrite(motorR1, speed);
}

void stop(){
  ledcWrite(motorL0, 0);
  ledcWrite(motorL1, 0);
  ledcWrite(motorR0, 0);
  ledcWrite(motorR1, 0);
}