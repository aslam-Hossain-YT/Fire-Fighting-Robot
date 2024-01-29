//Aslam Hossain YT: https://www.youtube.com/channel/UCdaQfJEKvpoAPIzQNwFGlHw
//Facebook Page: https://www.facebook.com/aslamhossain3852

#include <SoftwareSerial.h>

#define enA 44//Enable1 L298 Pin enA 
#define in1 30 //Motor1  L298 Pin in1 
#define in2 31 //Motor1  L298 Pin in2 
#define in3 32 //Motor2  L298 Pin in3 
#define in4 33 //Motor2  L298 Pin in4 
#define enB 45 //Enable2 L298 Pin enB 

boolean fire = false;

const int buzzerPin = 9;

const String PHONE = "+8801877543825"; //use your number with country code

#define rxPin 2
#define txPin 3 
SoftwareSerial sim800L(rxPin,txPin);

#define GAS_SENSOR A3    //Gas sensor


#define ir_R A0    
#define ir_F A1  
#define ir_L A2  

#define servo A4

#define pump 10 

int Speed = 120; // Write The Duty Cycle 0 to 255 Enable for Motor Speed 

int s1, s2, s3; 

void setup(){ // put your setup code here, to run once
   
Serial.begin(115200);
 
sim800L.begin(9600);

sim800L.println("AT");
delay(1000);
sim800L.println("AT+CMGF=1");
delay(1000);

pinMode(ir_R, INPUT);// declare fire sensor pin as input
pinMode(ir_F, INPUT);// declare fire sensor pin as input
pinMode(ir_L, INPUT);// declare fire sensor pin as input

pinMode(enA, OUTPUT); // declare as output for L298 Pin enA 
pinMode(in1, OUTPUT); // declare as output for L298 Pin in1 
pinMode(in2, OUTPUT); // declare as output for L298 Pin in2 
pinMode(in3, OUTPUT); // declare as output for L298 Pin in3   
pinMode(in4, OUTPUT); // declare as output for L298 Pin in4 
pinMode(enB, OUTPUT); // declare as output for L298 Pin enB 
pinMode(GAS_SENSOR, INPUT);

pinMode(servo, OUTPUT);
pinMode(buzzerPin, OUTPUT);

pinMode(pump, OUTPUT); 

 for (int angle = 90; angle <= 140; angle += 5)  {
   servoPulse(servo, angle);  }
 for (int angle = 140; angle >= 40; angle -= 5)  {
   servoPulse(servo, angle);  }

 for (int angle = 40; angle <= 90; angle += 5)  {
   servoPulse(servo, angle);  }

analogWrite(enA, Speed); // Write The Duty Cycle 0 to 255 Enable Pin A for Motor1 Speed 
analogWrite(enB, Speed); // Write The Duty Cycle 0 to 255 Enable Pin B for Motor2 Speed 

while(sim800L.available()){
  Serial.println(sim800L.readString());
  }

delay(500);
}


void loop(){  
s1 = digitalRead(ir_R);
s2 = digitalRead(ir_F);
s3 = digitalRead(ir_L);
int gas = analogRead(GAS_SENSOR);

if(gas >=600) {
  Serial.println("Gas is Detected.");
  Serial.print(gas);
  Serial.print("   ");
  beep(1000, 500);
  // Delay for 1 second between beeps
  delay(1000);
  send_sms();
}
   
//=============================================================
//                         Auto Control
//=============================================================   

Serial.print(s1);
Serial.print("\t");
Serial.print(s2);
Serial.print("\t");
Serial.println(s3);
delay(50);

if(s2 == 0){
forword();
delay(200);
Stop();
digitalWrite(pump, 1);

for(int angle = 90; angle <= 140; angle += 3){
servoPulse(servo, angle);  
}

for(int angle = 140; angle >= 40; angle -= 3){
servoPulse(servo, angle);    
}

for(int angle = 40; angle <= 90; angle += 3){
servoPulse(servo, angle);  
} 
beep(1000, 500);
delay(1000); 
//fire == true;
}

else if(s1 == 0){
backword();
delay(100); 
turnRight();
delay(200);
Stop();
digitalWrite(pump, 1);

for(int angle = 90; angle <= 140; angle += 3){
servoPulse(servo, angle);  
}

for(int angle = 140; angle >= 40; angle -= 3){
servoPulse(servo, angle);    
}

for(int angle = 40; angle <= 90; angle += 3){
servoPulse(servo, angle);  
}
beep(1000, 500);
delay(1000);
fire == true; 
}

else if(s3 == 0){
backword();
delay(100); 
turnLeft();
delay(200);
Stop();
digitalWrite(pump, 1);

for(int angle = 90; angle <= 140; angle += 3){
servoPulse(servo, angle);  
}

for(int angle = 140; angle >= 40; angle -= 3){
servoPulse(servo, angle);    
}

for(int angle = 40; angle <= 90; angle += 3){
servoPulse(servo, angle);  
} 
beep(1000, 500);
delay(1000);
fire == true;
}

else{
digitalWrite(pump, 0);
Stop();
}
delay(10);

while (fire == true)
     {
      Serial.println("Fire Detected.");
      make_call();
     }
}

void make_call()
{
    Serial.println("calling....");
    sim800L.println("ATD"+PHONE+";");
    delay(20000); //20 sec delay
    sim800L.println("ATH");
    delay(1000); //1 sec delay
    fire == false;
}

void send_sms(){

    Serial.println("sending sms....");
    delay(50);
    sim800L.print("AT+CMGF=1\r");
    delay(1000);
    sim800L.print("AT+CMGS=\""+PHONE+"\"\r");
    delay(1000);
    sim800L.print("Gas Detected");
    delay(100);
    sim800L.write(0x1A);
    delay(5000);
}

void servoPulse (int pin, int angle){
int pwm = (angle*11) + 500;      // Convert angle to microseconds
 digitalWrite(pin, HIGH);
 delayMicroseconds(pwm);
 digitalWrite(pin, LOW);
 delay(50);                   // Refresh cycle of servo
}

void forword(){  //forword
digitalWrite(in1, HIGH); //Right Motor forword Pin 
digitalWrite(in2, LOW);  //Right Motor backword Pin 
digitalWrite(in3, LOW);  //Left Motor backword Pin 
digitalWrite(in4, HIGH); //Left Motor forword Pin 
}

void backword(){ //backword
digitalWrite(in1, LOW);  //Right Motor forword Pin 
digitalWrite(in2, HIGH); //Right Motor backword Pin 
digitalWrite(in3, HIGH); //Left Motor backword Pin 
digitalWrite(in4, LOW);  //Left Motor forword Pin 
}

void turnRight(){ //turnRight
digitalWrite(in1, LOW);  //Right Motor forword Pin 
digitalWrite(in2, HIGH); //Right Motor backword Pin  
digitalWrite(in3, LOW);  //Left Motor backword Pin 
digitalWrite(in4, HIGH); //Left Motor forword Pin 
}

void turnLeft(){ //turnLeft
digitalWrite(in1, HIGH); //Right Motor forword Pin 
digitalWrite(in2, LOW);  //Right Motor backword Pin 
digitalWrite(in3, HIGH); //Left Motor backword Pin 
digitalWrite(in4, LOW);  //Left Motor forword Pin 
}

void Stop(){ //stop
digitalWrite(in1, LOW); //Right Motor forword Pin 
digitalWrite(in2, LOW); //Right Motor backword Pin 
digitalWrite(in3, LOW); //Left Motor backword Pin 
digitalWrite(in4, LOW); //Left Motor forword Pin 
}

void beep(int frequency, int duration) {
  tone(buzzerPin, frequency, duration); // Generate the tone
  delay(duration + 50); // Delay to allow the tone to finish
  noTone(buzzerPin); // Turn off the tone
}



