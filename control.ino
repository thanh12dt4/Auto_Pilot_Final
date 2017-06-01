#include <Wire.h> 
#include <Servo.h>
Servo motor;
Servo servo;
float lastmillis = 0;
float rpm = 1;
float revs = 0;
float curSpeed = 0.5;
int motorSpeed;
int count = 0;
String inAngle = "";    // biến inString dùng để lưu giá trị từ input
int angle;
int trig = 8;
int echo = 7;
unsigned long duration;
int distance;
void setup() {
  // Mở cổng Serial với mức baudrate 115200
  Serial.begin(115200);
  motor.attach(9);
  servo.attach(10);
  pinMode(2, INPUT_PULLUP);
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  pinMode(echo, INPUT);
  //attachInterrupt(0, rpm_fan, FALLING);
  lastmillis = millis()/1000.0;
  for (motorSpeed = 1500;motorSpeed <= 1670;motorSpeed  = motorSpeed + 10)
    {
      motor.writeMicroseconds(motorSpeed);
      delay(100);
    }
    delay(1000);
}
/*void runMotor(int inSpeed){
  for (
}*/

void loop() {
  // Đọc giá trị từ Serial
  if (millis() - lastmillis*1000 >= 200){ //Uptade every one second, this will be like reading frecuency
      detachInterrupt(0);
      rpm = 5*revs * 60 / 5.0;  
      // Speed = 2*pi*r × RPM × (60/1000) km/hr
      // Here radius(r) = 0.03 metres
      curSpeed = (2*3.1415926536*0.03*rpm*60)/1000.0;
      // lines to print the above values     
      // Show speeds
      revs = 0; 
      lastmillis = millis()/1000.0;
      attachInterrupt(0, rpm_fan, FALLING);
      if (curSpeed < 2){
        count++;
        if(motorSpeed < 1660){
          motorSpeed = motorSpeed + 20;
        }
        else{
          motorSpeed = motorSpeed + 1;
          motor.writeMicroseconds(motorSpeed);
        }
       }
      if (curSpeed > 0.5 && count > 10){
         motorSpeed = motorSpeed - 7;
         motor.writeMicroseconds(motorSpeed);
         count = 0;
      }
      //Serial.print(curSpeed);
      //Serial.print("  ");
      //Serial.println(motorSpeed);
      if (curSpeed > 4){
        motorSpeed = motorSpeed - 1;
         motor.writeMicroseconds(motorSpeed);
      }
   }
   digitalWrite(trig, LOW);
   delayMicroseconds(2);
   digitalWrite(trig, HIGH);
   delayMicroseconds(5);
   digitalWrite(trig, LOW);
   duration = pulseIn(echo, HIGH);
   distance = int(duration/2/29.412);
   //Serial.println(distance);
   if(distance < 40 && distance > 5){
     motorSpeed = 1400;
   }   
}
void rpm_fan(){
  revs++;
}
void serialEvent()
{
   while(Serial.available()){
      char inChar = Serial.read(); 
      if(inChar != '.'){
      inAngle = inAngle + inChar;
      }
      else{
        angle = -inAngle.toInt();
        if(angle < 45 && angle > -45){
          angle=map(angle,-90,90,0,180);
          servo.write(angle);
        }
        inAngle = "";
      }
   }
}
