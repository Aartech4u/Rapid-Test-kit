#include <SoftwareSerial.h>
SoftwareSerial s(8,9);
 char a[20];
 int incomingByte = 0; // for incoming serial data
void setup() {
s.begin(9600);
Serial.begin(9600);
pinMode(4,0);
}
 
void loop() {
int data=90;

  if (s.available() > 0) {
    // read the incoming byte:
    incomingByte = s.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  }
}
