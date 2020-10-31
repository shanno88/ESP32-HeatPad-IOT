#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

const char *ssid =  "FAST_0456";     // Enter your WiFi Name
const char *pass =  "lw730208"; // Enter your WiFi Password

WiFiClient client;
#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "gada888" // Your Adafruit IO Username
#define MQTT_PASS "aio_MhaX678mQcD1CLPTeJlrbkSwcJBM" // Adafruit IO AIO key

int PadPin = 21;         //  Connect heatpad pin to Pin 9 
int PulsePin = A0;
int mValue;// holds the incoming raw data. Signal value can range from 0-1024          
int Threshold = 90;            // Determine which Signal to “count as a beat”,



//Set up the feed you're publishing to
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);
// mData is the feed name where you will publish your data
Adafruit_MQTT_Publish mData = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/mData");  


void setup()
{
  Serial.begin(115200);
  delay(10);
    Serial.println("Connecting to ");
  Serial.println(ssid);
  pinMode(21,OUTPUT);         // pin that will heating to your pulseSensor
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");              // print ... till not connected
  }
  Serial.println("");
  Serial.println("WiFi connected");
}
 void loop()
{
  MQTT_connect();
  mValue = (100 - ((analogRead(PulsePin) / 1023.00) * 10));
  Serial.print("Pulse Value is  = ");
  Serial.print(mValue);
  Serial.println(" ");
 
  if(mValue < Threshold){     
  digitalWrite(21,HIGH);
  } else {
  digitalWrite(21,LOW);           
  }
 delay(10);

  String data1 = String(mValue);
//This condition is used to publish the Variable (pValue) on adafruit IO. Change thevariable according to yours.
       if (! mData.publish(mValue)) 
       {                     
         delay(2000);   
          }
 delay(3000);
}

void MQTT_connect() 
{
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) 
  {
    return;
  }
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  { 
       mqtt.disconnect();
       delay(3000);  // wait 5 seconds
       retries--;
       if (retries == 0) 
       {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
}