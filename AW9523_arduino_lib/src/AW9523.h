#ifndef AW9523_h
#define AW9523_h
#include "Arduino.h"
#include <Wire.h>

// aw9523 data register from datasheet
#define inputPort0 0x00
#define inputPort1 0x01

#define outputPort0 0x02
#define outputPort1 0x03

#define configPort0 0x04
#define configPort1 0x05



 

class AW9523{
  
  private:
    int modifyBit(int currentByte, int position, int bit) // bit field, change position, change value
      { 
        int mask = 1 << position; 
        return (currentByte & ~mask) | ((bit << position) & mask); 
      } 


    uint8_t i2cAddress;
    
    uint8_t pinModeP0 = 0xff; // all as input (1= input)
    uint8_t pinModeP1 = 0xff; // all as input (1= input)
    uint8_t pinDataP0 = 0x00; // all as 0 default
    uint8_t pinDataP1 = 0x00; // all as 0 default
    
  public:
     AW9523()
      {
        delay(6);
      }
	  
	  // device address 10110 AD1 AD0
    void init(bool AD1, bool AD0)
    {  // 1 for input , 0 for output
      if(AD0==1 and AD1==1){
          i2cAddress = 0x5B; // 11	91
          
        }else if(AD0==1 and AD1==0){
          i2cAddress = 0x5A; // 10	90
          
        }else if(AD0==0 and AD1==1){
          i2cAddress = 0x59; // 01	89
          
        }else if(AD0==0 and AD1==0){
          i2cAddress = 0x58; // 00	88
          
        }  
        

        // config config_port registers for all pins as input in initilize mode
        /*
        04h config p1
        05 config p2
        */
        Wire.beginTransmission(i2cAddress); // transmit to device 
        Wire.write(configPort0);        // config register p0
        Wire.write(pinModeP0);        // data , all pin as input
        Wire.endTransmission();    // stop transmitting

        Wire.beginTransmission(i2cAddress); // transmit to device 
        Wire.write(configPort1);        // config register p1
        Wire.write(pinModeP1);        // data , all pin as input
        Wire.endTransmission();    // stop transmitting

      }

    /*
    po_0 = 0      p1_0 = 8
    po_1   1      p1_1   9
    po_2   2      p1_2   10
    po_3   3      p1_3   11
    po_4   4      p1_4   12
    po_5   5      p1_5   13
    po_6   6      p1_6   14
    po_7   7      p1_7   15

    */

  // pin , CpinMode  INPUT == 0
  void pinMode(uint8_t pin, uint8_t mode)
  {
    if(pin<8){
      pinModeP0 = modifyBit(pinModeP0, pin, !mode); // change relavent bit in pin mode

      Wire.beginTransmission(i2cAddress); // transmit to device 
      Wire.write(configPort0);        // config register p0
      Wire.write(pinModeP0);        // data
      Wire.endTransmission();    // stop transmitting

    }else
    {
      pinModeP1 = modifyBit(pinModeP1, pin-8, !mode); // change relavent bit in pin mode

      Wire.beginTransmission(i2cAddress); // transmit to device 
      Wire.write(configPort1);        // config register p1
      Wire.write(pinModeP1);        // data 
      Wire.endTransmission();    // stop transmitting
    }
  }



     // HIGH ==1 
  void digitalWrite(uint8_t pin, uint8_t mode)
  {

    if(pin<8){  // for 02H register
      pinDataP0 = modifyBit(pinDataP0, pin, mode); // change relavent bit in pin mode

      Wire.beginTransmission(i2cAddress); // transmit to device 
      Wire.write(outputPort0);        // config register p0
      Wire.write(pinDataP0);        // data
      Wire.endTransmission();    // stop transmitting

    }else
    {  // for 03H register
      pinModeP1 = modifyBit(pinModeP1, pin-8, mode); // change relavent bit in pin mode

      Wire.beginTransmission(i2cAddress); // transmit to device 
      Wire.write(outputPort1);        // config register p1
      Wire.write(pinModeP1);        // data 
      Wire.endTransmission();    // stop transmitting
    }
  }


  void analogWrite(uint8_t pin, uint8_t value)
  {
    uint8_t analogRegister= pin+20;   // dim channel start form 20 t0 35

      Wire.beginTransmission(i2cAddress); // transmit to device 
      Wire.write(analogRegister);        // config register 
      Wire.write(value);          // data 
      Wire.endTransmission();    // stop transmitting

  }

  bool digitalRead(uint8_t pin)
  {
      if(pin<8)
      {  //00h reg

        Wire.beginTransmission(i2cAddress); // transmit to device 
        Wire.write(inputPort0);        // config input register 
        Wire.endTransmission();    // stop transmitting

        Wire.beginTransmission(i2cAddress); // transmit to device for read
		Wire.requestFrom(i2cAddress, 1);   // read 1 byte
        uint8_t ReadDataP0 = Wire.read(); // receive byte 

        return  (ReadDataP0 >> pin) & 1;


      }else
      { // 01h reg

        Wire.beginTransmission(i2cAddress); // transmit to device 
        Wire.write(inputPort1);        // config input register 
        Wire.endTransmission();    // stop transmitting

        Wire.beginTransmission(i2cAddress); // transmit to device for read
		Wire.requestFrom(i2cAddress, 1);   // read 1 byte
        uint8_t ReadDataP1 = Wire.read(); // receive byte 

        return  (ReadDataP1 >> pin-8) & 1;

      }

  }
};


#endif