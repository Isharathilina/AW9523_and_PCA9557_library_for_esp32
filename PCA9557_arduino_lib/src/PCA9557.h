#ifndef PCA9557_h
#define PCA9557_h
#include "Arduino.h"
#include <Wire.h>

// pca9557 registers from data sheet
#define configReg 0x03
#define inputReg 0x00
#define outputReg 0x01

class PCA9557{
  
  private:
    int modifyBit(int currentByte, int position, int bit) // bit field, change position, change value
      { 
        int mask = 1 << position; 
        return (currentByte & ~mask) | ((bit << position) & mask); 
      } 


    uint8_t i2cAddress; // 0011 a2 a1 a0  
    uint8_t CpinModeP0 = 0xff; // all as input (1= input)
    uint8_t pinDataP0 = 0x00; // all as 0 default
    
  public:
     PCA9557()
      {
        delay(6);
      }

      // deive addr 0011 a2 a1 a0  
      // a2 =0
    void init(bool AD1, bool AD0)
    {  // 1 for input , 0 for output
      if(AD0==1 and AD1==1){
          i2cAddress = 0x1B; //0011011  27
          
        }else if(AD0==1 and AD1==0){
          i2cAddress = 0x1A; //0011010	26
          
        }else if(AD0==0 and AD1==1){
          i2cAddress = 0x19; //0011001	25
          
        }else if(AD0==0 and AD1==0){
          i2cAddress = 0x18; //0011000	24
          
        }  
       

        // config config_port registers for all pins as input in initilize mode
        /*
        04h config p1
        05 config p2
        */
        Wire.beginTransmission(i2cAddress); // transmit to device 
        Wire.write(configReg);        // config register p0
        Wire.write(CpinModeP0);        // data , all pin as input
        Wire.endTransmission();    // stop transmitting

      }

    /*
    po_0 = 0      
    po_1   1      
    po_2   2      
    po_3   3      
    po_4   4      
    po_5   5      
    po_6   6      
    po_7   7      

    */

  // pin , CpinMode  INPUT == 0
  void pinMode(uint8_t pin, uint8_t mode)
  {
    
      pinModeP0 = modifyBit(pinModeP0, pin, !mode); // change relavent bit in pin mode (!mode use for match with arduino mode)

      Wire.beginTransmission(i2cAddress); // transmit to device 
      Wire.write(configReg);        // config register p0
      Wire.write(pinModeP0);        // data
      Wire.endTransmission();    // stop transmitting
    
  }



     // HIGH ==1 
  void digitalWrite(uint8_t pin, uint8_t mode)
  {

  
      pinDataP0 = modifyBit(pinDataP0, pin, mode); // change relavent bit in pin data

      Wire.beginTransmission(i2cAddress); // transmit to device 
      Wire.write(outputReg);        // config register p0
      Wire.write(pinDataP0);        // data
      Wire.endTransmission();    // stop transmitting

    
  }


  bool digitalRead(uint8_t pin)
  {
     
        Wire.beginTransmission(i2cAddress); // transmit to device 
        Wire.write(inputReg);        // config input register 
        Wire.endTransmission();    // stop transmitting

        Wire.beginTransmission(i2cAddress); // transmit to device for read
		Wire.requestFrom(i2cAddress, 1);   // read 1 byte
        uint8_t ReadDataP0 = Wire.read(); // receive byte 

        return  (ReadDataP0 >> pin) & 1;


  }
};


#endif