

#include <ArduinoJson.h>
#include <SoftwareSerial.h>
SoftwareSerial s(8,9);




#define idle 0
#define start 3
#define charging 1
#define discharging 2
#define kit_off 6

#define UCAP 1
#define ESR 2
#define LIFE_CYCLE 3

#define CHARGE 4
#define DISCHARGE 5
int test_type2;

int test_type;

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
int action;
 DynamicJsonDocument doc(400); 
void setup() {
   Serial.begin(115200);
   s.begin(9600);

   


    Serial.println("hi in setup");
    delay(500);
    
    pinMode(2,OUTPUT);     //discharge RB2
    pinMode(3,OUTPUT);     // timer reset
    pinMode(4,OUTPUT);     //timer hold
    pinMode(5,OUTPUT);     //charge
    pinMode(6,OUTPUT);     //discharge
    pinMode(7,INPUT);     //TO CHECK IF KIT ON (5V)
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
  
 


 
  Serial.println("start charging....");
  digitalWrite(4,1); //............................start timer............................................................................
  
  digitalWrite(5,1); //start charging 


  
 }

void timer_reset()
{
  digitalWrite(3,1);
  delay(100);
  digitalWrite(3,0); // reset timer
 
  delay(100);
  Serial.println("TIMER RESET");

  startMillis = millis()/1000;
  
  
}

void dis_charge()
{
     
        digitalWrite(4,1);     ////timer start
        digitalWrite(5,0); //cutt of  supply
        digitalWrite(3,0);
        digitalWrite(6,1);


}
void stop()
{
  Serial.println("inside stop function");
      digitalWrite(4,0); 
      digitalWrite(5,0);
      digitalWrite(3,0);
      digitalWrite(6,0);
      if(action==CHARGE)
      {
        CT=TM;
      }
      else  if(action==DISCHARGE)
      {
        DT=TM;
      }
      status=idle;
      return; 
}

void testRB1()
{
  
  switch(test_type){
    
      case UCAP :
                     {





                      
                        test_type2=UCAP;
                        float bank_res=5.0;
                        
                      for(int i=0;i<NOT;i++)
                        {       test_type=UCAP;
                                DT=0;I=0;
                                CAP=String(0);
                                timer_reset();
                                charge();
                                testno=i+1;
                                startMillis = millis()/1000;
                                                  
                                while(1)
                                          { if(status==idle)
                                            {
                                            Serial.println("inside if");  
                                            return;  
                                            }

                                             if(digitalRead(7)==0)
                                            {
                                              status=kit_off;
                                              return; 
                                            }
                                            
                                          
                                           Serial.print("status is");
                                            Serial.print(status);
                                            V=(((analogRead(A5))/(1024.0))*(5.0/scale));
                                            delay(10);
                                            I=(analogRead(A0)/(4096.0 * 37.5))*1000.0;
                                            delay(10);
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
                                                CT=(currentMillis-startMillis);
                                                Serial.println("  Seconds  ");    
                                                Serial.println("Timer hold.............................");  
                                                break;  
                                            }
                                        }
          
                                  startMillis =millis()/1000;
                                  currentMillis =millis()/1000;
                                  while(currentMillis-startMillis< Hold_time)
                                        {
                                             digitalWrite(2,0); 
                                                digitalWrite(6,0); 
                                                I=(analogRead(A0)/(4096 * 37.5))*1000.0;
                                                Serial.println("holding time"); 
                                                V=(((analogRead(A5))/(1024.0))*(5.0/scale));
                                                
                                                currentMillis= millis()/1000;
                                                TM=currentMillis-startMillis; 
                                                send_data();
                                                status=holding;
                                                if(currentMillis-startMillis>20)
                                                {
                                                    digitalWrite(5,0); //supply cuttoff 
                                                }
                                                 if(status==idle)
                                                {
                                                Serial.println("inside if");  
                                                return;  
                                                }

                                                  if(digitalRead(7)==0)
                                                  {
                                                    status=kit_off;
                                                    return; 
                                                  }
                                        }
                                        if(select_Rbank== 1)
                                        {
                                          digitalWrite(6,1); //discharge bank 1 
                                          Serial.println(" disscharge through bank 1");
                                          bank_res=6.93;
                                       }
                          
                                      else if(select_Rbank== 2)
                                      {
                                        digitalWrite(2,1); //discharge bank 1 
                                        digitalWrite(6,1); //discharge bank 2
                                        Serial.println(" disscharge through bank 1 and 2");
                                        bank_res=3.465;
                                      }

                                       
                                      digitalWrite(5,0); //supply cuttoff 
                                      timer_reset();
                                      
                                      dis_charge();
                                      flag=true;
                                  while(1)
                                        {     
                                          
                                                
                                                 if(status==idle)
                                                {
                                                Serial.println("inside if");  
                                                return;  
                                                }

                                                 /*if(digitalRead(7)==0)
                                                  {
                                                    status=kit_off;
                                                    return; 
                                                  }*/
                                            
                                                  
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
                                                     DT=(currentMillis-startMillis);
                                                     Serial.println(" Seconds");
                                                     Serial.println(" Capacitance  :");
                                                     C=((currentMillis-startMillis))/(bank_res);
                                                     CAP=String(C);
                                                     Serial.print(C);
                                                     Serial.print("  C");
                                                     delay(200);
                                                     flag=false;
                                                     //digitalWrite(4,1);
                                                 }
                                                  if(V<0.3 )
                                                  {
                                                    
                                                      Serial.println("return to loop again....................................................................");
                                                      
                                                      digitalWrite(6,0); //discharge bank 1 
                                                      digitalWrite(2,0); //discharge bank 2
                                                      digitalWrite(4,1);
                                                      digitalWrite(5,0);
                                                       //testno=i+1; 
                                                       digitalWrite(13,1);delay(3000); digitalWrite(13,0);
                                                       timer_reset();
                                                       break;
                                                   }
                                          }
                                          Serial.print("done test no:"); ////Serial.println(testno);
                                         
                        }  
                       status=idle;
                      break;
                        
                     }   
                      
       

     case ESR :
                    {
                      test_type2=ESR;
                      ////Serial.println("inside esr test");
                    /*if(complete):break;
                     test_typr=ESR;
                    status idle;
                    int esr value;
                    status idle;
                    break;
                     */  
                     send_data();
                     break;
                    }

      case CHARGE:{
                    timer_reset();
                    charge();
                     CAP=String(0);
                    digitalWrite(4,1);
                    while(1)
                    {

                                            if(status==idle)
                                            {
                                            Serial.println("inside if");  
                                            return;  
                                            }
                                            if(digitalRead(7)==0)
                                            {
                                              status=kit_off;
                                              return; 
                                            }
                                            
                                          
                                            V=(((analogRead(A5))/(1024.0))*(5.0/scale));
                                            I=(analogRead(A0)/(4096.0 * 37.5))*1000.0;
                                            Serial.print("bank 1 Charged upto :");
                                            status= charging;
                                            ////Serial.println(V);
                                            currentMillis = millis()/1000;
                                            Serial.println((currentMillis-startMillis));  
                                            TM=currentMillis-startMillis;   
                                            send_data();
                   }
                 }
       case DISCHARGE:
                    {           
                    timer_reset();
                    dis_charge();
                    CAP=String(0);
                    digitalWrite(4,1);
                    while(1)
                    {
                       if(status==idle)
                        {
                        Serial.println("inside if");  
                        return;  
                         }
                          if(digitalRead(7)==0)
                          {
                             status=kit_off;
                             return; 
                           }
                                            
                      
                      I=(analogRead(A1)/(4096 * 37.5))*1000.0;
                      V=(((analogRead(A5))/(1024.0))*(5.0/scale));
                      //Serial.print("bank 1 DisCharged upto :");
                      status=discharging;
                      Serial.print(V);
                      Serial.println("Volt");
                      currentMillis = millis()/1000;
                      TM=currentMillis-startMillis; 
                      send_data(); 
        
                    }
                    }
        
                                  
}

}
  
 
                     
      
int i =0;
 
void if_idle()
{


      if(digitalRead(7)==0)
      {
      status=idle;
      }
       else if(digitalRead(7)==1)
      {
      status=kit_off;
      }
      V=(((analogRead(A5))/(1024.0))*(5.0/scale));
      I=(analogRead(A0)/(4096.0 * 37.5))*1000.0;
      digitalWrite(4,0);
      
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
      // Attempt to de//Serialize the JSON-formatted message
       Serial.setTimeout(5000);
      DeserializationError error = deserializeJson(doc,message);
      Serial.println(" deSerialzation  ");
      if(error) {
       Serial.println(F("deSerializeJson() failed: "));
       Serial.println(error.c_str());
       /* s.print(F("deSerializeJson() failed: "));
        s.println(error.c_str());*/
       // return;
      }
      if((doc["type"] == "request")) {
      doc["type"] = "response";
      // Get data from analog sensors
      doc["ST"] = idle;//status;
      doc["V"] =  V;
      doc["I"] = I;
      doc["CT"] = CT;
      doc["DT"] = DT;
      doc["TM"] = 0;
      doc["C"] = CAP;
      doc["testno"] =0;
   
      
      serializeJson(doc,s);
      i+=100;
      messageReady1 = false;
      messageReady = false;
    }
    else if((doc["test_type"] == UCAP)) 
    {
      max_voltage= doc["max_voltage"]; //status;
      test_type=UCAP;
      Hold_time= doc["Hold_time"];
      select_Rbank= doc["select_Rbank"] ;
      NOT= doc["NOT"] ;
      Serial.print("UCAP TEST COMMAND");
      Serial.println(select_Rbank);
      Serial.println(NOT);

      if(select_Rbank== 1 or select_Rbank == 2)
      {
        status=start;

        ////Serial.println("starting system");
      }
    }  
     else if((doc["test_type"] == ESR))
     {
      ////Serial.println("ESR TEST COMMAND");
      test_type=ESR;;
      status=start;
     }  
     else if((doc["test_type"] == LIFE_CYCLE))
     {
      ////Serial.println("LIFE CYCLE TEST COMMAND");
      status=start;
      
      
     } 
     if((doc["test_type"] == 4 )or (doc["test_type"]) == 5 or (doc["test_type"] == 7)  )
     {
                  
         ////Serial.println(" critical action to be perform now");  
        test_type= doc["test_type"] ;
        if(doc["test_type"]==7)
        {
         ////Serial.println("stop everything");
        stop();
        test_type=0;
         status=idle;
        
         }
         else if(doc["test_type"]==4 or doc["test_type"]==5)
         {  ////Serial.println(" charge or discharge");  
            status=start;
            
         } 
     }  
    s.flush();
    message="";
    messageReady = false;
    messageReady1 = false;
 
  
}





void send_data()
{
           action=0;
          String message = "";
          boolean messageReady = false;
          boolean messageReady1 = false;
          ////Serial.println("in side send data");
          while( s.available()) 
                {
                message =s.readString();
                messageReady = true;
                messageReady1 = true;
                }
  
  if(messageReady)
  {
                  Serial.print(message);
                  // The only messages we'll parse will be formatted in JSON
                 //DynamicJsonDocument doc(2000); // ArduinoJson version 6+
                  // Attempt to de//Serialize the message
                  Serial.setTimeout(5000);
                  DeserializationError error = deserializeJson(doc,message);
                if(error) 
                {
                  Serial.print(F("de//SerializeJson() failed: "));
                  Serial.println(error.c_str());
                  messageReady = false;
                  //return;
                 }
                  if((doc["type"] == "request") or  messageReady1 ) 
                  {
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
                        doc["test_type_FA"] = test_type2;
                        
                        serializeJson(doc,s);
                        i+=100;
                         messageReady1 = false;
                         
                    
                }
                if((doc["test_type"] == 4 )or (doc["test_type"]) == 5 or (doc["test_type"] == 6)  )
                {
                  
                Serial.println(" critical action to be perform now");  
                test_type= doc["test_type"] ;
                 if(test_type==6)
                {
                  Serial.println("stop everything");
                  stop();
                
                  status=idle;
                  
                }
                else if(test_type==4 or test_type==5)
                { Serial.println("charge or discharge");
                  status=start;
                  
                  
                }
                  //messageReady1 = false;
                }

                 
                
                
    s.flush();
    message="";
    messageReady = false;
    messageReady1 = false;
  }
  
}



  
 
  
  
  

void loop() {

  
 if((status==idle) or (status==kit_off) )
 {
  Serial.println("if idle ");
  if_idle();
 }
 else if(status==start)
 {
     while(1)
    { 
     CT=0;
    testRB1();
    if((status==idle) or (status==kit_off) )
    {
      break;
      
    }
    
    }
 
 }
}
 

  
