// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

#define LED_PIN 13
bool blinkState = false;


//leg-raise movement 
char tempString[6];
int checkTop=0;
int checkMiddle=0;
int checkLow=0;
int up=0;
float topRange[2]={65,90};
float midRange[2]={30,65};
float lowRange[2]={-30,30};
int  count=0;


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

void setup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
Wire.begin();

// initialize serial communication
Serial.begin(9600);

// initialize device
Serial.println("Initializing I2C devices...");
accelgyro.initialize();

// verify connection
Serial.println("Testing device connections...");
Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

// configure Arduino LED for
pinMode(LED_PIN, OUTPUT);
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
if(checkTop==1 && checkMiddle==1 && checkLow==1)
{
  checkTop=0;
  checkMiddle=0;
  checkLow=0;
  up=1;
}

}

float data[3000];
int i=0;

void loop(){
  // read raw accel/gyro measurements from device
    if(i==2999){
    i=0;
  }
  
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  // accelerometer_X_Axis angle calc
  accel_reading = ax;
  accel_corrected = accel_reading - accel_offset;
  accel_corrected = map(accel_corrected, -16800, 16800, -90, 90);
  accel_corrected = constrain(accel_corrected, -90, 90);
  accel_angle = (float)(accel_corrected * accel_scale);



data[i] =accel_angle;
if(up==0){
  upCheck(data[i]);
}
else if(up==1){
  downCheck(data[i]);
}

if(up==1 && checkLow==1){ 
  count++;
  up=0;
  checkTop=0; 
  checkMiddle=0;
  checkLow=0;
}

  Serial.print("i: ");
    Serial.print(i);
  Serial.print("  checkTop:");
   Serial.print(checkTop);
  Serial.print("  checkMiddle:");
   Serial.print(checkMiddle);
  Serial.print("  checkLow:");
   Serial.print(checkLow);
  Serial.print("  angle:");
  Serial.println(accel_angle);
    Serial.print("\t");   Serial.print("count:");
  Serial.println(count);
  i++;
  
  // blink LED to indicate activity
  blinkState = !blinkState;
  digitalWrite(LED_PIN, blinkState);
  
//timestamp
time_stamp();
}
void time_stamp(){
  while ((millis() - last_cycle) < 50){
  delay(1);
  }
  // once loop cycle reaches 50ms, reset timer value and continue
  cycle_time = millis() - last_cycle;
  last_cycle = millis();
}
