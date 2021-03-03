#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial s(8,9);
int i=0;

char mystr[10]; //Initialized variable to store recieved data
//char mystr[20]; //Initialized variable to store recieved data
char sel[20];
//String FRAME="00";
String Status="1";
String Charge_time="23";
String volatge="45";
String Current="67";
String DisCharge_time="89";
String timer="12.23";

String frame=Status+Charge_time+volatge+Current+DisCharge_time;





int  startMillis,currentMillis;
double R1=31.94,R2=5.49;
double scale=R2/(R1+R2);

void setup() {
Serial.begin(9600);
s.begin(9600);
Serial.println("hi in setup");
delay(1000);
pinMode(2,OUTPUT);     // CHARGE
pinMode(3,OUTPUT);     // DISCHARGE RB1 
pinMode(4,OUTPUT);     //discharge RB2
pinMode(5,OUTPUT);     //TIMER
pinMode(6,OUTPUT);     //TIMER RESET
pinMode(A1,INPUT);     //VOLATAGE READ

pinMode(8,0);
pinMode(9,1);

Serial.println("SELECT OPERATION");
Serial.println("1. Resitor bank 1");
Serial.println("2. Resitor bank 2");
Serial.println("3. Resitor bank 1+2");

digitalWrite(2,1); 
digitalWrite(3,1); 
digitalWrite(4,1); 
digitalWrite(5,1); 

 digitalWrite(6,1); // reset timer
  delay(100);
  digitalWrite(6,0);
  delay(100);
  Serial.println("TIMER RESET");



}

void testRB1()
{
  
 
  digitalWrite(6,0);
  delay(100);
  digitalWrite(6,1); // reset timer
  
  delay(100);
  Serial.println("TIMER RESET");
  delay(5000);
  Serial.println("start charging....");
  digitalWrite(5,LOW); //............................start timer............................................................................
  
  digitalWrite(2,LOW); //start charging 
  startMillis =millis()/1000;

  
double volt;


   while(1)
  {
    
    
      volt=(((analogRead(A1))/(1024.0))*(5.0/scale));
      Serial.print("bank 1 Charged upto :");
      Serial.println(volt);
      if(volt>23)
      {
       
        
        digitalWrite(5,HIGH); //...............................stop timer......................................... 
  
        currentMillis = millis()/1000;
      
       
        Serial.print("charging time :   ");
        Serial.print((currentMillis-startMillis));   
        Serial.println("  Seconds  ");    

        
       
        digitalWrite(5,1);
        Serial.println("Timer hold.............................");    


        Serial.println("hold time 15 seconds");   
        for(int i=0;i<15;i++)
        {
        delay(1000);
        Serial.println("Wait holding");   
        }
        



        
        
        digitalWrite(2,1); //supply cuttoff 
       
          delay(1000);
   
          
          Serial.println("discharing started");    
          digitalWrite(3,0); //discharge bank 1
          startMillis = millis()/1000;
          digitalWrite(6,0);
          delay(100);
          digitalWrite(6,1); //************************************************* reset timer*************************************************************************
          

          digitalWrite(5,0);
          


        while(1)
        {
                  double C;
                  volt=(((analogRead(A1))/(1024.0))*(5.0/scale));
                  Serial.print("bank 1 DisCharged upto :");
                  Serial.print(volt);
                  Serial.println("Volt");

                 //delay(1000);

                  if(volt<12 && volt>10)
                  {
                       currentMillis= millis()/1000;
                        
                        digitalWrite(3,1); //discharge bank 1 off 
                       
                        digitalWrite(5,1); //.................................................hold timer.................................................
                       
      
                        Serial.println("discharged");
                       
                         Serial.print("discharging time :     ");
                         
                        Serial.print((currentMillis-startMillis)); 

                        Serial.println(" Seconds");
                         
                         Serial.println(" Capacitance  :");
      
                         C=((currentMillis-startMillis))/(6.93);
                         Serial.print(C);
                          Serial.print("  C");
                         return;
                        
                        
      
                        
                    
                  }
                  else 
                  {
                        Serial.println("nhi hora");

                    
                  }
                   
        }
        

        
      }


      
     
  
  }
}



void testRB2()
{
  
  digitalWrite(6,1); // reset timer
  delay(100);
  digitalWrite(6,0);
  delay(100);
  Serial.println("TIMER RESET");
  delay(5000);
  Serial.println("start charging....");
 
  
  digitalWrite(2,HIGH); //start charging 
  startMillis = millis();

  
double volt;


  while(1)
  {
    
    
       volt=(analogRead(A1)*5.0)/(1024.0*scale);
      Serial.print("bank 1 Charged upto :");
      Serial.println(volt);
      if(volt>=24)
      {

        currentMillis = millis();
      
       
        Serial.print("charging time :");
        Serial.print((currentMillis-startMillis)/1000);   
        Serial.println("Seconds");    

        
        digitalWrite(5,1);
        delay(1000);
        digitalWrite(5,0);
        Serial.println("Timer hold.............................");    


        Serial.println("hold time 25 minute");   
        for(int i=0;i<1500;i++)
        {
        delay(1000);
        Serial.println("Wait holding");   
        }
        



        
        
        digitalWrite(2,0); //supply cuttoff 
        Serial.println("discharing started");    

        delay(10000);

        digitalWrite(3,1); //discharge bank 1
        startMillis = millis(); 


        while(1)
        {
                  double C;
                  volt=(analogRead(A1)*5.0)/(1024.0*scale);
                  Serial.print("bank 1 DisCharged upto :");
                  Serial.print(volt);
                  Serial.println("Volt");

                  if(volt<=12 && volt>10)
                  {
                  currentMillis=millis(); 
                  delay(500);
                  digitalWrite(3,0); //discharge bank 1 off 
                  digitalWrite(5,1); // hold timer
                  delay(500);
                  digitalWrite(5,0);
                 

                  Serial.println("discharged");
                  Serial.println("discharging time :");
                   Serial.print("discharging time :");
                   Serial.println((currentMillis-startMillis)/1000);

                   
                   Serial.print("Capacitance  :");

                   C=((currentMillis-startMillis)/1000)/(6.93);
                   Serial.print(C);
                    Serial.print("  C");
                   while(1);
                  
                  

                  
                    
                  }
                   
        }
        

        
      }


      
     
  
  }
}




void both_bank()
{
  
  digitalWrite(6,1); // reset timer
  delay(100);
  digitalWrite(6,0);
  delay(100);
  Serial.println("TIMER RESET");
  delay(5000);
  Serial.println("start charging....");
 
  
  digitalWrite(2,HIGH); //start charging 
  startMillis = millis();

  
double volt;


   while(1)
  {
    
    
      volt=(analogRead(A1)*5.0)/(1024.0*scale);
      Serial.print("bank 1 Charged upto :");
      Serial.println(volt);
      if(volt>=24)
      {

        currentMillis = millis();
      
       
        Serial.print("charging time :");
        Serial.print((currentMillis-startMillis)/1000);   
        Serial.println("Seconds");    

        
        digitalWrite(5,1);
        delay(1000);
        digitalWrite(5,0);
        Serial.println("Timer hold.............................");    


        Serial.println("hold time 25 minute");   
        for(int i=0;i<1500;i++)
        {
        delay(1000);
        Serial.println("Wait holding");   
        }
        



        
        
        digitalWrite(2,0); //supply cuttoff 
        Serial.println("discharing started");    

        delay(10000);

        digitalWrite(4,1);
        digitalWrite(3,1); 
        startMillis = millis(); 


        while(1)
        {
                  double C;
                  volt=(analogRead(A1)*5.0)/(1024.0*scale);
                  Serial.print("bank 1 DisCharged upto :");
                  Serial.print(volt);
                  Serial.println("Volt");

                  if(volt<=12)
                  {
                  currentMillis=millis(); 
                  delay(500);
                  digitalWrite(4,0); //discharge bank 1,2 off 
                  digitalWrite(3,0);
                  digitalWrite(5,1); // hold timer
                  delay(500);
                  digitalWrite(5,0);
                 

                  Serial.println("discharged");
                  Serial.println("discharging time :");
                   Serial.print("discharging time :");
                   Serial.println((currentMillis-startMillis)/1000);

                   
                   Serial.print("Capacitance  :");

                   C=((currentMillis-startMillis)/1000)/(6.93);
                   Serial.print(C);
                    Serial.print("  C");
                   while(1);
                  
                  

                  
                    
                  }
                   
        }
        

        
      }


      
     
  
  }
}

void json_sender()
{



  
 StaticJsonBuffer<1000> jsonBuffer;
 JsonObject& root = jsonBuffer.createObject();
  root["S"] = Status;
  root["Ct"] = Charge_time;
  root["V"] =  i;
  /*root["C"] =Current;
  
  root["Dt"] =DisCharge_time;
  root["TM"] = timer;*/
  


  
if(s.available()>0)
{
 root.printTo(s);
}
//root.printTo(s);
i++;
}




void loop()
{json_sender();

/*  delay(2000);
 
  
 if ( mySerial.available() >=0) {
  mySerial.readBytes(mystr,20); //Read the serial data and store in var
  Serial.println(mystr); //Print data on Serial Monitor


  Serial.println("hi");
  
  Serial.println(int(mystr[0]));
   if ( int(mystr[0])==49) {
  Serial.println("hifgfgfgfgfgfgfgfgfgfgfgfgfgd");
  testRB1();
  }
 
   }

   else{
    
    
    
      Serial.println("in esle");
    
    }



 
char sel;
*/
//sel=Serial.read();
//Serial.println("in side loop......");
/*while(Serial.available()> 0)
{
sel=char(Serial.
read());
Serial.println(sel);
delay(1000);


switch(atol(sel))
{
  
  case 1: testRB1();
          break;
          
  case 2: testRB2();
          break;
   case 3: both_bank();
         break;       
  
  
  
}
}*/

 //testRB1();
  
    /*  Serial.print("bank 1 Charged upto :");
      Serial.println(volt);


 Serial.print("binary :");
      Serial.println(analogRead(A1));
      delay(1000);*/
 
 /*if(int(mystr)==1)
 {
  
  Serial.println("binar1123555");
 } 
  */
}
