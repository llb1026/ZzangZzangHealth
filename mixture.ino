
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
/////////////////wifi//////////////////////////

#define ThingSpeak 0
#define IFTTT 1
#define TUMBLER 2
const int httpPort = 5000;
const char* host = "api.thingspeak.com"; // Your domain  
const char* host2 ="maker.ifttt.com";
//const char* host3="ec2-35-174-166-248.compute-1.amazonaws.com";
const char* host3="106.10.43.32";

String ApiKey = "U87CBNFW57C1V1CC";
String IFTTTKEY="dCsrRRUCkeIzC2irwNBaGL";
String EVENTO="nodeMcu_temperature";
String path = "/update?key=" + ApiKey + "&field1=";  
String path_pushup="/pushup?data=";
String path_plank="/testing/plank?data=";
String path_leg="/testing/leg?data=";
String path_start="/start";




//const char* ssid = "TOZ_SH";
//const char* pass = "1234567890";

//const char* ssid = "SK_WiFiE4EF";
//const char* pass = "1509011593";

const char* ssid = "sogang123";
const char* pass = "12345678";

//////////////////////////////////////////////
MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

#define LED_PIN 13
bool blinkState = false;


//leg-raise movement 
char tempString[6];
int Leg_checkTop=0;
int Leg_checkMiddle=0;
int Leg_checkLow=0;
int up=0;
int Leg_count=0;
float Leg_topRange[2]={65,90};
float Leg_midRange[2]={30,65};
float Leg_lowRange[2]={-30,30};

//squat movement
int squat_checkTop=0;
int squat_checkMiddle=0;
int squat_checkLow=0;
int squat_up=0;
float squat_topRange[2]={60,90};
float squat_midRange[2]={30,60};
float squat_lowRange[2]={-10,30};
int  squat_count=0;
int  previous_squat_count=0;

// accelerometer values
int accel_reading;
int accel_corrected;
int accel_offset = 200;
float accel_angle;
float accel_angle2;
float accel_angle3;
float accel_scale = 1; // set to 0.01



// gyro values
int gyro_offset = 151; // 151
int gyro_corrected;
int gyro_reading;

float gyro_rate;
float gyro_scale = 0.02; // 0.02 by default - tweak as required
float gyro_angle;

float loop_time = 0.05; // 50ms loop
float angle = 0.00; // value to hold final calculated gyro angle

// time stamp variables
int last_update;
int cycle_time;
long last_cycle = 0;


// Data wire is plugged into port 2 on the Arduino
int trigPin = D4;
int echoPin = D3;
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)

char temperatureString[6];
int checkTop=0;
int checkMiddle=0;
int checkLow=0;
int down=0;
float topRange[2]={35,50};
float midRange[2]={15,35};
float lowRange[2]={5,15};
int  count=0;



float Range[2]={14,37};
unsigned long squat_previousTime = 0;
unsigned long Leg_previousTime = 0;
unsigned long previousTime = 0;
int seconds ;
int sum=0;
int checktime;
/*
 * The setup function. We only start the sensors here
 */
void setup(void)
{
  // start serial port
  Wire.begin();
    Serial.begin(38400);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.println("Initializing I2C devices...");
accelgyro.initialize();
// verify connection
Serial.println("Testing device connections...");
Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
 WiFi.begin(ssid, pass);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  WiFiClient client;
/*
   if (!client.connect(host, httpPort)) {
          Serial.println("connection failed");
          return;
           }
                client.print(String("GET ") + path + 15 + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");




*/
  
if (!client.connect(host3, httpPort)) {
     Serial.println("HomeTraining connection failed");
     return;
     }
 client.print(String("GET ") + path_start + " HTTP/1.1\r\n" +
               "Host: " + host3 + "\r\n" +"User-agent: arduino/1.0\r\n"+ 
               "Connection: keep-alive\r\n\r\n");

client.print(String("GET /") + " HTTP/1.1\r\n"+ "Host: "+host3+path_start+"\r\n"+ "Connection: keep-alive\r\n"+ "\r\n");
Serial.print(String("GET /") + " HTTP/1.1\r\n"+ "Host: "+host3+path_start+"\r\n"+ "Connection: keep-alive\r\n"+ "\r\n");


               
                Serial.print(String("GET ") + path_start + " HTTP/1.1\r\n" +
               "Host: " + host3 + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");

               
}



/*
 * Main function, get and show the temperature
 */
void Leg_downCheck(float data){
  
if(data<=Leg_topRange[1]&& data>=Leg_topRange[0]){
  Leg_checkTop=1;
}
if(Leg_checkTop==1 && data<=Leg_midRange[1] && data >=Leg_midRange[0]){
  Leg_checkMiddle=1;
}
if(Leg_checkMiddle==1 && data<=Leg_lowRange[1] && data>=Leg_lowRange[0]){
  Leg_checkLow=1;
}


}
void Leg_upCheck(float data){
  
if(data<=Leg_lowRange[1] && data>=Leg_lowRange[0]){
  Leg_checkLow=1;
}
if(Leg_checkLow==1 && data<=Leg_midRange[1] && data >=Leg_midRange[0]){
  Leg_checkMiddle=1;
}
  
if(Leg_checkMiddle==1 && data<=Leg_topRange[1]&& data>=Leg_topRange[0]){
  Leg_checkTop=1;
}
if(Leg_checkTop==1 && Leg_checkMiddle==1 && Leg_checkLow==1)
{
  Leg_checkTop=0;
  Leg_checkMiddle=0;
  Leg_checkLow=0;
  up=1;
}

}
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
void squat_downCheck(float data){
  
if(data<=squat_topRange[1]&& data>=squat_topRange[0]){
  squat_checkTop=1;
}
if(squat_checkTop==1 && data<=squat_midRange[1] && data >=squat_midRange[0]){
  squat_checkMiddle=1;
}
if(squat_checkMiddle==1 && data<=squat_lowRange[1] && data>=squat_lowRange[0]){
  squat_checkLow=1;
}


}

void squat_upCheck(float data){
  
if(data<=squat_lowRange[1] && data>=squat_lowRange[0]){
  squat_checkLow=1;
}
if(squat_checkLow==1 && data<=squat_midRange[1] && data >=squat_midRange[0]){
  squat_checkMiddle=1;
}
  
if(squat_checkMiddle==1 && data<=squat_topRange[1]&& data>=squat_topRange[0]){
  squat_checkTop=1;
}
if(squat_checkTop==1 && squat_checkMiddle==1 && squat_checkLow==1)
{
  squat_checkTop=0;
  squat_checkMiddle=0;
  squat_checkLow=0;
  squat_up=1;
}

}


float data;
int i=0;
 int timepast=0;
 int  timeflag=0;

 int Leg_timepast=0;
 int Leg_timeflag=0;
void loop(void)
{ 
  float duration, distance;
   WiFiClient client;
  
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  /*if (!client.connect(host3, httpPort)) {
     Serial.println("HomeTraining connection failed");
     return;
     }*/
     

  digitalWrite(trigPin, HIGH);
  delay(10);   
  digitalWrite(trigPin, LOW);
 
  duration = pulseIn(echoPin, HIGH);
  distance = ((float)(340 * duration) / 10000) / 2;
  ////////////////////////////////////squat///////////////////////////////////////////////

   accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  // accelerometer_X_Axis angle calc
  accel_reading = az;
  accel_corrected = accel_reading - accel_offset;
  accel_corrected = map(accel_corrected, -16800, 16800, -90, 90);
  accel_corrected = constrain(accel_corrected, -90, 90);
  accel_angle = (float)(accel_corrected * accel_scale);


data =accel_angle;
if( squat_up==0){
   squat_upCheck(data);
}
else if( squat_up==1){
   squat_downCheck(data);
}

if( squat_up==1 &&  squat_checkLow==1){ 
  squat_count++;
    Serial.print(" squat_count:");
  Serial.println( squat_count);
   squat_up=0;
  squat_checkTop=0; 
  squat_checkMiddle=0;
  squat_checkLow=0;
}

/*
  Serial.print("   squat_checkTop:");
   Serial.print( squat_checkTop);
  Serial.print("   squat_checkMiddle:");
   Serial.print( squat_checkMiddle);
  Serial.print("   squat_checkLow:");
   Serial.print( squat_checkLow);
  Serial.print("   squat_angle:");
  Serial.print(accel_angle);
    Serial.print("\t");   Serial.print(" squat_count:");
  Serial.println( squat_count);
*/

  if(squat_count==1){
    timeflag=1;
  }
  
   if (millis() >= (squat_previousTime)  ) {
squat_previousTime = squat_previousTime + 1000;
if(timeflag==1)
timepast+=1;

if(squat_count==1 && timepast>=4){
  squat_count=0;
  timepast=0;
  timeflag=0;
}
}

////////////////////////////////////////////////////plank//////////////////////////////////////////
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
     client.print(String("GET ") + path_plank + sum + " HTTP/1.1\r\n" +
               "Host: " + host3 + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");
    Serial.println("good job!");
   }
  sum=0;  
   
}
/*
Serial.print(seconds,DEC);
Serial.print("distance :");
Serial.print(distance);
*/
Serial.print("plank time :");
Serial.print(sum);
Serial.print(" distance: ");
  Serial.println(distance);

} // end 1 second


   ////////////////////////////////////////////////push up///////////////////////////////////////////////////////
data =distance;
if(down==0){
  downCheck(data);
}
else if(down==1){
  upCheck(data);
}

if(down==1 && checkTop==1){ 
  count++;
 Serial.print("pushup count: ");
  Serial.println(count);
  
  if (!client.connect(host3, httpPort)) {
     Serial.println("HomeTraining connection failed");
     return;
     }
     
   client.print(String("GET ") + path_pushup + count + " HTTP/1.1\r\n" +
               "Host: " + host3 + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");
             /*
               Serial.print(String("GET ") + path_pushup + count + " HTTP/1.1\r\n" +
               "Host: " + host3 + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");
               */
  down=0;
  checkTop=0; 
  checkMiddle=0;
  checkLow=0;
}

/*
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  Serial.print(" pushup_checkTop:");
   Serial.print(checkTop);
  Serial.print("  pushup_checkMiddle:");
   Serial.print(checkMiddle);
  Serial.print("  pushup_checkLow:");
   Serial.print(checkLow);
  Serial.print(" distance: ");
  Serial.print(data);
  Serial.print("   count: ");
  Serial.println(count);

*/
////////////////////////////////////////////////////leg_raise/////////////////////////////////
 accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  // accelerometer_X_Axis angle calc
  accel_reading = ax;
  accel_corrected = accel_reading - accel_offset;
  accel_corrected = map(accel_corrected, -16800, 16800, -90, 90);
  accel_corrected = constrain(accel_corrected, -90, 90);
  accel_angle = (float)(accel_corrected * accel_scale);



data =accel_angle;
if(up==0){
  Leg_upCheck(data);
}
else if(up==1){
  Leg_downCheck(data);
}

if(up==1 && Leg_checkLow==1){ 
  Leg_count++;
   Serial.print("Leg_raise count:");
  Serial.println(Leg_count);
   client.print(String("GET ") + path_leg + Leg_count + " HTTP/1.1\r\n" +
               "Host: " + host3 + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");
  up=0;
  Leg_checkTop=0; 
  Leg_checkMiddle=0;
  Leg_checkLow=0;
}

/*
  Serial.print(" Leg_checkTop:");
   Serial.print(Leg_checkTop);
  Serial.print("  Leg_checkMiddle:");
   Serial.print(Leg_checkMiddle);
  Serial.print("  Leg_checkLow:");
   Serial.print(Leg_checkLow);
  Serial.print("  angle:");
  Serial.print(accel_angle);
    Serial.print("\t");   Serial.print("count:");
  Serial.println(Leg_count);
*/
  if(Leg_count==1){
    Leg_timeflag=1;
  }
  
   if (millis() >= (Leg_previousTime)  ) {
Leg_previousTime = Leg_previousTime + 1000;
if(Leg_timeflag==1)
Leg_timepast+=1;

if(Leg_count==1 && Leg_timepast>=4){
  Leg_count=0;
  Leg_timepast=0;
  Leg_timeflag=0;
}
}
  
 /*
if (!client.connect(host3, httpPort)) {
     Serial.println("TUMBLER connection failed");
     return;
     }
    Serial.println("Connected");
                 client.print(String("GET ") + path_pushup + count + " HTTP/1.1\r\n" +
               "Host: " + host3 + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");

if (!client.connect(host3, httpPort)) {
     Serial.println("TUMBLER connection failed");
     return;
     }
    Serial.println("Connected");
                 client.print(String("GET ") + path_plank + sum + " HTTP/1.1\r\n" +
               "Host: " + host3 + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");
               
if (!client.connect(host3, httpPort)) {
     Serial.println("TUMBLER connection failed");
     return;
     }
    Serial.println("Connected");
                 client.print(String("GET ") + path_leg + Leg_count + " HTTP/1.1\r\n" +
               "Host: " + host3 + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");

                Serial.print(String("GET ") + path2 + temperatureString + " HTTP/1.1\r\n" +
               "Host: " + host3 + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");
               
     
  }
  */

}
