 /* pin ports
    po_0 = 0      p1_0 = 8
    po_1   1      p1_1   9
    po_2   2      p1_2   10
    po_3   3      p1_3   11
    po_4   4      p1_4   12
    po_5   5      p1_5   13
    po_6   6      p1_6   14
    po_7   7      p1_7   15

    include wire libbary and connect aw9523 scl, sda

    */
#include<aw9523.h>
AW9523 extend;

void setup() {
  // put your setup code here, to run once:
  
  extend.init(1,1);
  extend.pinMode(1,OUTPUT);
  extend.pinMode(2,OUTPUT);
  extend.pinMode(3,INPUT);
  

}

void loop() {
  
  extend.digitalWrite(1,HIGH);
  extend.analogWrite(2,100);
  bool val = digitalRead(3);
  
}
