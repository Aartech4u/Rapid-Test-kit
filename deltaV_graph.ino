float R1=31.94,R2=5.49;
float scale=R2/(R1+R2);
void setup() {
   Serial.begin(115200);


   


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

void loop() {
  double V,V2;
  V=(((analogRead(A5))/(1024.0))*(5.0/scale));
  delay(1);
// Serial.println(V);
  V2=(((analogRead(A5))/(1024.0))*(5.0/scale));

  if(abs(V-V2)>0.4)
  {
 
  Serial.print(V);   Serial.print("    ");
    Serial.println(V2);
  
  }
  

}
