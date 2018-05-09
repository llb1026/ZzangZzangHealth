
#include <OneWire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D4
#define ThingSpeak 0
#define IFTTT 1
#define TUMBLER 2

const char* host = "api.thingspeak.com"; // Your domain  
const char* host2 ="maker.ifttt.com";
//const char* host3="ec2-35-174-166-248.compute-1.amazonaws.com";
const char* host3="zhuote13t0.execute-api.ap-northeast-2.amazonaws.com";

String ApiKey = "U87CBNFW57C1V1CC";
String IFTTTKEY="dCsrRRUCkeIzC2irwNBaGL";
String EVENTO="nodeMcu_temperature";
String path = "/update?key=" + ApiKey + "&field1=";  
String path2="/testing/update?data=";


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

//const char* ssid = "SK_WiFiE4EF";
//const char* pass = "1509011593";

const char* ssid = "sogang123";
const char* pass = "12345678";


char temperatureString[6];

void setup(void){
  Serial.begin(115200);
  Serial.println("");
  
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

  DS18B20.begin();
   

}

float getTemperature() {
  float temp;
  do {
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
    delay(100);
  } while (temp == 85.0 || temp == (-127.0));
  return temp;
}

void loop() {

  float temperature = getTemperature();
  
  dtostrf(temperature, 2, 2, temperatureString);
  // send temperature to the serial console
  Serial.println(temperatureString);

  WiFiClient client;
  const int httpPort = 80;
  
  int key=TUMBLER;
  
  if(key==ThingSpeak){

          if (!client.connect(host, httpPort)) {
          Serial.println("connection failed");
          return;
           }
                client.print(String("GET ") + path + temperatureString + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");
  }

  
  else if(key==IFTTT){
        
     if (!client.connect(host2, httpPort)) {
     Serial.println("connection failed");
     return;
     }
    Serial.println("Connected");
   
    String toSend = "GET /trigger/";
    toSend += EVENTO;
    toSend += "/with/key/";
    toSend += IFTTTKEY;
    toSend += "?value1=";
    toSend += temperatureString;
    toSend += " HTTP/1.1\r\n";
    toSend += "Host: ";
    toSend += host2;
    toSend += "\r\n";
    toSend += "Connection: close\r\n\r\n";

    client.print(toSend);
  }

  else if(key==TUMBLER){
 if (!client.connect(host3, httpPort)) {
     Serial.println("TUMBLER connection failed");
     return;
     }
    Serial.println("Connected");
                 client.print(String("GET ") + path2 + temperatureString + " HTTP/1.1\r\n" +
               "Host: " + host3 + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");

                Serial.print(String("GET ") + path2 + temperatureString + " HTTP/1.1\r\n" +
               "Host: " + host3 + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");
               
     
  
  }
 delay(5000);
  }


