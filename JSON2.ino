#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial s(8,9);
int i=0;
 
void setup() {
s.begin(9600);
}


 
void loop() {
 StaticJsonBuffer<1000> jsonBuffer;
 JsonObject& root = jsonBuffer.createObject();
  root["data3"] = i;
  root["data4"] = 200;
if(s.available()>0)
{
 root.printTo(s);
}
root.printTo(s);
i++;
}
