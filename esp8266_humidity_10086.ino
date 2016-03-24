/*
buletooth car 
This sketch is written for soil humidity monitor.The version 1.0 was created on 11 march 2016.

Components:
nodemcu 
humidity sensor

 Circuit:
  sensor---------->nodemcu
 * SIG attached to analog pin A0
 * VCC  attached to 3.3V
 * GND attached to GND
http://open.iot.10086.cn/appview/p/f2f2a5ca74b3f2995f9637e2d485e77d
 created 11 March 2016
 by Yu Hui
*/
//nodemcu pinout https://github.com/esp8266/Arduino/issues/584
/*json
POST /devices/773441/datapoints HTTP/1.1
api-key:y6lt6OhD1t2FG4hWo0gYyszMfwI=
Host: api.heclouds.com
Content-Length:71

{"datastreams":[{"id":"test","datapoints":[{"value":"192.168.5.88"}]}]}

GET /devices/773441/datapoints HTTP/1.1
api-key:y6lt6OhD1t2FG4hWo0gYyszMfwI=
Host: api.heclouds.com
*/
#include <ESP8266WiFi.h>
#define myPeriodic 5 //in sec | Thingspeak pub is 15sec

float prevTemp = 0;
const char* server = "api.heclouds.com";
String apiKey = " y6lt6OhD1t2FG4hWo0gYyszMfwI=";
const char* MY_SSID = "mxedu";
const char* MY_PWD = "swjtumaker";
int sent = 0;


void connectWifi()
{
  Serial.print("Connecting to " + *MY_SSID);
  WiFi.begin(MY_SSID, MY_PWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected");
  Serial.println("");
  digitalWrite(0,HIGH);
  delay(1000);
  digitalWrite(0,LOW);
  delay(1000);
  digitalWrite(0,HIGH);
  delay(1000);
  digitalWrite(0,LOW);
  delay(1000);
  digitalWrite(0,HIGH);
}//end connect


void setup() {
  Serial.begin(115200);
  pinMode(0, OUTPUT);
  connectWifi();
  
}




void sendTeperatureTS(float temp,float temp2)
{
  WiFiClient client;

  if (client.connect(server, 80)) { // use ip 184.106.153.149 or api.thingspeak.com
    Serial.println("WiFi Client connected ");

    String postStr = "{\"datastreams\":[{\"id\":\"test\",\"datapoints\":[{\"value\":";
    postStr += "\""+String(temp)+"\"";
    postStr += "}]}]}";
    postStr += "\r\n\r\n";
    client.print("POST /devices/773441/datapoints HTTP/1.1\n");
    client.print("api-key:" + apiKey + "\n");
    client.print("Host: api.heclouds.com\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    delay(1000);

  }//end if
  sent++;
  client.stop();
}//end send


void loop() {
  float temp,temp2;
    temp = analogRead(A0);
    temp = temp * 3.3/1024*1000;   //使用浮点数存储温度数据，温度数据由电压值换算得到
    if(temp<35){
      digitalWrite(0,HIGH);
      delay(300);
      digitalWrite(0,LOW);
      delay(300);
      digitalWrite(0,HIGH);
      delay(300);
      digitalWrite(0,LOW);
      delay(300);
      digitalWrite(0,HIGH);
      delay(300);
      digitalWrite(0,LOW);
      delay(300);
      }
    Serial.print(String(sent)+" humidity: ");
    Serial.println(temp); 
  sendTeperatureTS(temp,temp2);
  int count = myPeriodic;
  while (count--)
    delay(1000);
}



