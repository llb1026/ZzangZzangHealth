// Include the libraries we need
#include <OneWire.h>

// Data wire is plugged into port 2 on the Arduino
int trigPin = D4;
int echoPin = D3;
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)

char temperatureString[6];
int checkTop=0;
int checkMiddle=0;
int checkLow=0;
int down=0;
float topRange[2]={40,50};
float midRange[2]={15,40};
float lowRange[2]={5,15};
int  count=0;

/*
 * The setup function. We only start the sensors here
 */
void setup(void)
{
  // start serial port
    Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

/*
 * Main function, get and show the temperature
 */

void downCheck(float data){
  
if(data<=topRange[1]&& data>=topRange[0]){
  checkTop=1;
}
if(checkTop==1 && data<=midRange[1] && data >=midRange[0]){
  checkMiddle=1;
}
if(checkMiddle==1 && data<=lowRange[1] && data>=lowRange[0]){
  checkLow=1;
}

if(checkTop==1 && checkMiddle==1 && checkLow==1)
{
  checkTop=0;
  checkMiddle=0;
  checkLow=0;
  down=1;
}
}
void upCheck(float data){
  
if(data<=lowRange[1] && data>=lowRange[0]){
  checkLow=1;
}
if(checkLow==1 && data<=midRange[1] && data >=midRange[0]){
  checkMiddle=1;
}
  
if(checkMiddle==1 && data<=topRange[1]&& data>=topRange[0]){
  checkTop=1;
}


}

float data[3000];
int i=0;
void loop(void)
{ 
  float duration, distance;
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  if(i==2999){
    i=0;
  }
  digitalWrite(trigPin, HIGH);
  delay(10);   
  digitalWrite(trigPin, LOW);
 
  // echoPin 이 HIGH를 유지한 시간을 저장 한다.
  duration = pulseIn(echoPin, HIGH);
  // HIGH 였을 때 시간(초음파가 보냈다가 다시 들어온 시간)을 가지고 거리를 계산 한다.
  // 340은 초당 초음파(소리)의 속도, 10000은 밀리세컨드를 세컨드로, 왕복거리이므로 2로 나눠준다.
  distance = ((float)(340 * duration) / 10000) / 2;
data[i] =distance;
if(down==0){
  downCheck(data[i]);
}
else if(down==1){
  upCheck(data[i]);
}

if(down==1 && checkTop==1){ 
  count++;
  down=0;
  checkTop=0; 
  checkMiddle=0;
  checkLow=0;
}


dtostrf(data[i], 2, 2, temperatureString);
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  Serial.print(i);
  Serial.print("  ");
   Serial.print(checkTop);
  Serial.print("  ");
   Serial.print(checkMiddle);
  Serial.print("  ");
   Serial.print(checkLow);
  Serial.print("  ");
  Serial.println(temperatureString);
  Serial.println(count);
  i++;
  delay(50);
}
