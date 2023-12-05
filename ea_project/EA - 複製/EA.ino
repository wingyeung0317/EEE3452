//set up the Bluetooth function
#include "BluetoothSerial.h"  
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;
//Motor
#define MLeft     19       // Motor Left to G19
#define MRight    21       // Motor MRight to G16
#define MLeft2    16       // Motor Left2 to G21
#define MRight2    4       // Motor MRight to G4
//RGB LED
#define R   25             // Red LED pin to 25       
#define G   26             // Green LED pin to 26
#define B   27             // Blue LED pin to 27
//For slider function
#define max_char 12           
char message[max_char];    
char r_char;               
byte count = 0;            
int i;
int redValue = 255;        
int greenValue = 255;      
int blueValue = 255;       
String redTempValue;       
String greenTempValue;     
String blueTempValue;      
int flag = 0;
char currentColor;
//For slider function  
//PWM properties
const int freq = 1000;      //Frequency for Motor
const int ledfreq = 255;    //Frequency for LED
const int resolution = 10;  //  10 bits, 0 - 1024 level for Motor
const int resolution2 = 8;  //  8 bits, 0 - 255 level for LED
//PWmChannel for Motor
const int PWM0 = 0;         // Channel 0
const int PWM1 = 1;         // Channel 1
const int PWM2 = 2;         // Channel 2
const int PWM3 = 3;         // Channel 3
//PWmChannel for LED
const int PWM4 = 4;         // Channel 4 
const int PWM5 = 5;         // Channel 5 
const int PWM6 = 6;         // Channel 6 

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Tch");
  //For Motor setting: 
  //(Channel, Frequency, Bits) 
  ledcSetup(PWM0, freq, resolution); 
  ledcSetup(PWM1, freq, resolution); 
  ledcSetup(PWM2, freq, resolution); 
  ledcSetup(PWM3, freq, resolution); 
  //(Motor, Channel) 
  ledcAttachPin(MLeft, PWM0);     // Motor 1 pin 1, pin 19 => channel 0
  ledcAttachPin(MRight, PWM1);    // Motor 2 pin 1, pin 4 => channel 2
  ledcAttachPin(MLeft2, PWM2);    // Motor 1 pin 2, pin 21 => channel 1
  ledcAttachPin(MRight2, PWM3);   // Motor 2 pin 2, pin 16 => channel 3
  //For LED setting: 
  //(Channel, Frequency, Bits) 
  ledcSetup(PWM4, ledfreq, resolution2);
  ledcSetup(PWM5, ledfreq, resolution2);
  ledcSetup(PWM6, ledfreq, resolution2);
  //(LED, Channel) 
  ledcAttachPin(R, PWM4);
  ledcAttachPin(G, PWM5);
  ledcAttachPin(B, PWM6); 
}

void loop() 
{
  if (SerialBT.available()) 
  {                                         
    char rxdata = SerialBT.read();
    Serial.write(rxdata);
    //For Slider function
     flag = 0;
    if(count < (max_char-1))
    {              
        message[count] = rxdata;     
        if(rxdata=='R')
        {
           currentColor = 'R';
           redTempValue = "";
        }
        else if(rxdata=='G')
        {
           currentColor = 'G';
           greenTempValue = "";
        }
        else if(rxdata=='B')
        {
           currentColor = 'B';
           blueTempValue = "";
        }
        if(currentColor == 'R' && rxdata!='R')
        {
           redTempValue += rxdata;
        }
        else if(currentColor == 'G' && rxdata!='G')
        {
           greenTempValue += rxdata;
        }
        else if(currentColor == 'B' && rxdata!='B')
        {
           blueTempValue += rxdata;
        }
        count++;                    
        message[count] = '\0';       
    }
     if(flag == 0)
     {
       ledcWrite(PWM4, redTempValue.toInt());
       ledcWrite(PWM5, greenTempValue.toInt());
       ledcWrite(PWM6, blueTempValue.toInt());
       Serial.println(redTempValue.toInt());
       Serial.println(greenTempValue.toInt());
       Serial.println(blueTempValue.toInt());
       flag=1;
       for(i=0; i<12; i++)       
       {
        message[i] = '\0';
       } 
       count=0;  
    }
    //For Slider function
    if (rxdata == 'r')                    // LED Red
    {
      ledcWrite(PWM4, 255);
 
    }
    if (rxdata == 'g')                    // LED Green 
    {                  
      ledcWrite(PWM5, 255);
               
    }
    if (rxdata == 'b')                    // LED Blue
    {
      ledcWrite(PWM6, 255);  
    }
    if (rxdata == 'p')                    // LED Purple
    {
      ledcWrite(PWM4, ledfreq);
      ledcWrite(PWM6, ledfreq);
    }
    if (rxdata == 'c')                    // LED Cyan
    {
      ledcWrite(PWM5, ledfreq);
      ledcWrite(PWM6, ledfreq);
    }
    if (rxdata == 'y')                    // LED Yellow   
    {                    
      ledcWrite(PWM4, ledfreq);
      ledcWrite(PWM5, ledfreq);                
    }
    if (rxdata == 'w')                    // white on
    { 
      ledcWrite(PWM4, ledfreq);
      ledcWrite(PWM5, ledfreq);
      ledcWrite(PWM6, ledfreq);
    } 
    if (rxdata == 'o')                    //light off 
    {                 
      ledcWrite(PWM4, 0);
      ledcWrite(PWM5, 0);
      ledcWrite(PWM6, 0);
    } 
    if (rxdata == 'h')                    //forward    
    {  
      ledcWrite(PWM0, 0);
      ledcWrite(PWM1, freq);      //right motor
      ledcWrite(PWM2, 0);
      ledcWrite(PWM3, freq);      //left motor
      delay(500);

    }
    if (rxdata == 'j')                    //back
    {
      ledcWrite(PWM0, freq);
      ledcWrite(PWM1, 0);
      ledcWrite(PWM2, freq);
      ledcWrite(PWM3, 0);
      delay(500);
    } 
    if (rxdata == 'k')                    //left 
    { 
      ledcWrite(PWM0, 0);
      ledcWrite(PWM1, freq);
      ledcWrite(PWM2, 0);
      ledcWrite(PWM3, 0);
      delay(500);
    } 
    if (rxdata == 'x')                    //right 
    {
      ledcWrite(PWM0, 0);
      ledcWrite(PWM1, 0);
      ledcWrite(PWM2, 0);
      ledcWrite(PWM3, freq);
      delay(500);
    }
    if (rxdata == 'z')                    //stop 
    {
      ledcWrite(PWM0, 0);
      ledcWrite(PWM1, 0);
      ledcWrite(PWM2, 0);
      ledcWrite(PWM3, 0);
      delay(500);
    }   
  }   
}

void forward(){
  
}