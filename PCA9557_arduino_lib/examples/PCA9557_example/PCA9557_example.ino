 /* pin ports

   IO0 =0
   IO1 = 1
   IO2 = 2
   IO3 = 3
   IO4 = 4
   IO5 = 5
   IO6 = 6
   IO7 = 7
   
    */
#include<PCA9557.h>
PCA9557 extend;

void setup() {
  // put your setup code here, to run once:
  
  extend.init(1,1);
  extend.CpinMode(1,OUTPUT);
  extend.CpinMode(2,OUTPUT);
  extend.CpinMode(3,INPUT);
  

}

void loop() {
  
  extend.CdigitalWrite(1,HIGH);
  extend.CdigitalWrite(2,HIGH);
  bool val = CdigitalRead(3);
  
}
