#include <ArduinoJson.h>
#include <SoftwareSerial.h>
SoftwareSerial s(8,9);


#define idle 0
#define start 3
#define charging 1
#define discharging 2

#define holding 4

int  startMillis,currentMillis;
float R1=31.94,R2=5.49;
float scale=R2/(R1+R2);
bool flag=false;



int CT=0;
float V=0;
float I=0;
int DT=0;
int TM=0;
float C=0;
int testno=0,NOT=0;

int max_voltage=20,Hold_time=60;
int select_Rbank=0;
String CAP = "NA";
int status= idle;
 DynamicJsonDocument doc(400); 
void setup() {
   Serial.begin(4800);
   s.begin(4800);


    Serial.println("hi in setup");
    delay(500);
    
    pinMode(2,OUTPUT);     //discharge RB2
    pinMode(3,OUTPUT);     // timer reset
    pinMode(4,OUTPUT);     //timer hold
    pinMode(5,OUTPUT);     //charge
    pinMode(6,OUTPUT);     //discharge
    pinMode(7,OUTPUT);     // 
    pinMode(11,OUTPUT);     // feedback1
    pinMode(12,OUTPUT);     // feedback2
    pinMode(13,OUTPUT);     // feedback3
    
    pinMode(A0,INPUT);     //charging current
    pinMode(A1,INPUT);     //discharging current
    pinMode(A2,INPUT);     //VOLATAGE input for cell
    pinMode(A3,INPUT);      //VOLATAGE input for cell
    pinMode(A4,INPUT);     //VOLATAGE READ for shunt charging current 
    pinMode(A5,INPUT);     //VOLATAGE for bamk
    
    
    
    pinMode(8,0);
    pinMode(9,1);  
  
    
   

 
    digitalWrite(2,0); 
    digitalWrite(3,0); 
    digitalWrite(4,0); 
    digitalWrite(6,0); 
    digitalWrite(5,0); 
  
    
    digitalWrite(3,0); // reset timer
    delay(100);
    digitalWrite(3,1);
    delay(100);
    Serial.println("TIMER RESET");   


}
void charge()
{
  
 

  Serial.print("PERFORMING TEST NO;");
  Serial.print(i);
 
 
  Serial.println("start charging....");
  digitalWrite(4,1); //............................start timer............................................................................
  
  digitalWrite(5,1); //start charging 

  while(command==charge)//charge ==true
  {
  V=(((analogRead(A5))/(1024.0))*(5.0/scale));
  I=(analogRead(A0)/(4096.0 * 37.5))*1000.0;
  status= charging;
  }
  
 }

void timer_reset()
{
  digitalWrite(3,1);
  delay(100);
  digitalWrite(3,0); // reset timer
 
  delay(100);
  Serial.println("TIMER RESET");
  
  
}

void dis_charge()
{
        digitalWrite(3,1);
        delay(500);
        digitalWrite(3,0);
         delay(500);
        digitalWrite(4,1);     ////timer start
        digitalWrite(5,0); //start charging 


}
void stop()
{
  while(command==stop)
  {
  
  }
}

void testRB1()
{

  switch(test_type){
      case ucap_test or life_cycle :
                     {
                      for(int i=1;i<=NOT;i++)
                        {
                                timer_reset();
                                charge();
          
                                while(1)
                                        {
                                          
                                           
                                            V=(((analogRead(A5))/(1024.0))*(5.0/scale));
                                            I=(analogRead(A0)/(4096.0 * 37.5))*1000.0;
                                            Serial.print("bank 1 Charged upto :");
                                            status= charging;
                                            Serial.println(V);
                                            currentMillis = millis()/1000;
                                            Serial.println((currentMillis-startMillis));  
                                            TM=currentMillis-startMillis;   
                                            send_data();
                                            if(V>=max_voltage)
                                            {
                                                digitalWrite(4,0); //...............................stop timer......................................... 
                                                currentMillis = millis()/1000;
                                                Serial.print("charging time :   ");
                                                Serial.print((currentMillis-startMillis));   
                                                CT=(currentMillis-startMillis)+5;
                                                Serial.println("  Seconds  ");    
                                                Serial.println("Timer hold.............................");  
                                                break;  
                                            }
                                        }
          
                                  startMillis =millis()/1000;
                                  currentMillis =millis()/1000;
                                  while(currentMillis-startMillis< Hold_time)
                                        {
                                                I=(analogRead(A0)/(4096 * 37.5))*1000.0;
                                                Serial.println("holding time"); 
                                                
                                                currentMillis= millis()/1000;
                                                TM=currentMillis-startMillis; 
                                                send_data();
                                                status=holding;
                                                if(currentMillis-startMillis>10)
                                                {
                                                    digitalWrite(5,0); //supply cuttoff 
                                                }
                                        }        
                                  digitalWrite(5,0); //supply cuttoff 
                                  timer_reset();
                                  dis_charge();
                                  while(1)
                                        {       
                                                I=(analogRead(A1)/(4096 * 37.5))*1000.0;
                                                V=(((analogRead(A5))/(1024.0))*(5.0/scale));
                                                Serial.print("bank 1 DisCharged upto :");
                                                status=discharging;
                                                Serial.print(V);
                                                Serial.println("Volt");
                                                currentMillis = millis()/1000;
                                                TM=currentMillis-startMillis; 
                                                send_data(); 
                                                if(V<(max_voltage/2) and flag==true)
                                                {
                                                     digitalWrite(4,0); //.................................................hold timer.................................................
                                                     Serial.println("discharged");
                                                     Serial.print("discharging time :     ");
                                                     Serial.print((currentMillis-startMillis)); 
                                                     DT=(currentMillis-startMillis)+4;
                                                     Serial.println(" Seconds");
                                                     Serial.println(" Capacitance  :");
                                                     C=((currentMillis-startMillis))/(bank_res);
                                                     CAP=String(C);
                                                     Serial.print(C);
                                                     Serial.print("  C");
                                                     delay(200);
                                                     flag=false;
                                                     digitalWrite(4,1);
                                                 }
                                                  if(V<1.0)//
                                                  {
                                                    
                                                      Serial.println("return to loop again....................................................................");
                                                      status=idle;
                                                      digitalWrite(6,0); //discharge bank 1 
                                                      digitalWrite(2,0); //discharge bank 2
                                                      digitalWrite(4,1);
                                                       testno=i; 
                                                       break;
                                                   }
                                          }
                        }
                      break;  
                     }   
                      
       

     case esr_test :
                    {
                    //do nothing  
                       
                    }   
     case (stop or charge or discharge):
                    {
                      
                      
                      
                    }              
                  
                  
  
  
  
  
  
  
  
  }
  float bank_res=5.0;
  if(NOT==0)
  {
  NOT=1;  
  }
 
  CT=0;
  DT=0;
  CAP="0";
  I=0;
 
  digitalWrite(3,1);
  delay(100);
  digitalWrite(3,0); // reset timer
 
  delay(100);
  Serial.println("TIMER RESET");

  Serial.print("PERFORMING TEST NO;");
 // Serial.print(i);
 
 
  Serial.println("start charging....");
  digitalWrite(4,1); //............................start timer............................................................................
  
  digitalWrite(5,1); //start charging 
  startMillis =millis()/1000;
  
      

  



   
    
                        
                           digitalWrite(5,0); //supply cuttoff 

                          if(select_Rbank== 1)
                          {
                            digitalWrite(6,1); //discharge bank 1 
                            Serial.println(" disscharge through bank 1");
                            bank_res=6.93;
                            
                          }
                          
                          else 
                           if(select_Rbank== 2)
                          {
                            digitalWrite(2,1); //discharge bank 1 
                            digitalWrite(6,1); //discharge bank 2
                            Serial.println(" disscharge through bank 1 and 2");
                            bank_res=3.465;
                          }

                          
                                          
                      

                         
                         digitalWrite(3,1);
                          delay(500);
                         digitalWrite(3,0);
                         delay(500);
                         //************************************************* reset timer*************************************************************************
                         digitalWrite(4,1);     ////timer start
                   // Serial.println("hold time 15 seconds");   
                    /*for(int i=0;i<15;i++)
                    {
                    delay(1000);
                    Serial.println("Wait holding");   
                    }*/
                    
                     /// delay(1000);

                          
               startMillis = millis()/1000;
               Serial.println("discharing started"); 
    bool flag=true;                  
    
        
            Serial.println("returning ....................................................................");
                                //  return;
        
      }
return;
}
      
     
  
 int i =0;
 
void if_idle()
{
     
      boolean messageReady = false;
       boolean messageReady1 = false;
      String message = "";
      while(messageReady == false) { // blocking but that's ok
        Serial.println("system in idle state");
        if(s.available()) {
          message =s.readString();
          messageReady = true;
          messageReady1 = true;
        }
      }
      Serial.print(message);
      Serial.println(" after messege loop");
      // Attempt to deserialize the JSON-formatted message
      DeserializationError error = deserializeJson(doc,message);
      Serial.println(" deserialzation  ");
      if(error) {
        Serial.println(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        /*s.print(F("deserializeJson() failed: "));
        s.println(error.c_str());*/
       // return;
      }
      if((doc["type"] == "request")) {
      doc["type"] = "response";
      // Get data from analog sensors
      doc["ST"] = status;//status;
      doc["V"] = 0;
      doc["I"] = I;
      doc["CT"] = 0;
      doc["DT"] = 0;
      doc["TM"] = 0;
      doc["C"] = CAP;
      doc["testno"] =0;
      
      serializeJson(doc,s);
      i+=100;
      messageReady1 = false;
      messageReady = false;
    }
    

       
     max_voltage= doc["max_voltage"]; //status;
      
      Hold_time= doc["Hold_time"];
      select_Rbank= doc["select_Rbank"] ;
      NOT= doc["NOT"] ;
      Serial.print("bank selected=======  ");
      Serial.println(select_Rbank);

      if(select_Rbank== 1 or select_Rbank == 2)
      {
        status=start;

        Serial.println("starting system");
        
     
      }
      
      
    s.flush();
    message="";
    messageReady = false;
    messageReady1 = false;
 
  
}





void send_data()
{
    String message = "";
     boolean messageReady = false;
     boolean messageReady1 = false;
    Serial.println("in side send data");
    while( s.available()) {
      
    message +=  char(s.read());
    messageReady = true;
    messageReady1 = true;
   
   
  }
  // Only process message if there's one
  if(messageReady) {
    Serial.print(message);
    // The only messages we'll parse will be formatted in JSON
   // DynamicJsonDocument doc(2000); // ArduinoJson version 6+
    // Attempt to deserialize the message
    DeserializationError error = deserializeJson(doc,message);
    if(error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      messageReady = false;
      //return;
    }
    if((doc["type"] == "request") or  messageReady1 ) {
      doc["type"] = "response";
      // Get data from analog sensors
      doc["ST"] = status;//status;
      doc["V"] = V;
      doc["I"] = I;
      doc["CT"] = CT;
      doc["DT"] = DT;
      doc["TM"] = TM;
      doc["C"] = CAP;
      doc["testno"] = testno;
       doc["test_type"] = testno;
      
      serializeJson(doc,s);
      i+=100;
       messageReady1 = false;
      
    }
    s.flush();
    message="";
    messageReady = false;
    messageReady1 = false;
  }
  
  
  
  
  
}
void loop() {
 if(status==idle)
 {
  Serial.println("if idle ");
  if_idle();
 }
 else if(status==start)
 {
     while(1)
    { 
    
    testRB1();
    if(status==idle)
    {
      break;
      
    }
    
    }
 
 }
}
 

  
