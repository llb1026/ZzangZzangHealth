// Include the libraries we need
#include <OneWire.h>

// Data wire is plugged into port 2 on the Arduino
int trigPin = D4;
int echoPin = D3;
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)


float Range[2]={20,40};

/*
 * The setup function. We only start the sensors here
 */

unsigned long previousTime = 0;
int seconds ;
int sum=0;
int checktime;
void setup(void)
{
  // start serial port
    Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

// I  think using microseconds is even more accurate

void loop(){
    float duration, distance;
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus

  digitalWrite(trigPin, HIGH);
  delay(10);   
  digitalWrite(trigPin, LOW);
 
  // echoPin 이 HIGH를 유지한 시간을 저장 한다.
  duration = pulseIn(echoPin, HIGH);
  // HIGH 였을 때 시간(초음파가 보냈다가 다시 들어온 시간)을 가지고 거리를 계산 한다.
  // 340은 초당 초음파(소리)의 속도, 10000은 밀리세컨드를 세컨드로, 왕복거리이므로 2로 나눠준다.
  distance = ((float)(340 * duration) / 10000) / 2;


if (millis() >= (previousTime)  ) {
previousTime = previousTime + 1000;  // use 100000 for uS
seconds = seconds +1;

if( distance <= Range[1] && distance >= Range[0]){
  if(checktime==(seconds-1)){
  sum=sum+1;
  }
  checktime=seconds;
}
else{
  
   if(sum>=10){
    Serial.println("good job!");
   }
  sum=0;  
   
}
Serial.print(seconds,DEC);
Serial.print("distance :");
Serial.println(distance);
Serial.print("sum :");
Serial.println(sum);
} // end 1 second

} // end loop
