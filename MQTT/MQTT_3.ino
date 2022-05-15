/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <ArduinoJson.h>

#include <DHT.h>
/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "iPhone"
#define WLAN_PASS       "00000000"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "khanhtran01"
#define AIO_KEY         "aio_aVYg97AKI9fGh3lqSa9lShUqm48Z"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient espClient;
#define slowLED D1
#define emerLED D0
//#define weather D3
#define timeChanged D5
#define isRed D2
#define DHTTYPE DHT11

#define DHTPIN 12 //doc nhiet do va do am qua chan nay cua ESP8266 - GPIO2 - D4 D6
DHT dht(DHTPIN, DHTTYPE);
// or... use WiFiClientSecure for SSL
//WiFiClientSecure client;
//-----------------------------------------//
DynamicJsonDocument cmd(1024); // luu gia tri control Traffic Light
DynamicJsonDocument timeCmd(1024); //luu gia tri dieu chinh thoi gian
//-----------------------------------------//
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&espClient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Publish to these feeds
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish p_humiTempFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humi-and-temp");
Adafruit_MQTT_Publish p_redFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/red-light");
// Subscribe to these feeds
Adafruit_MQTT_Subscribe s_humiTempFeed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/humi-and-temp");
Adafruit_MQTT_Subscribe s_controlFeed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/control");
Adafruit_MQTT_Subscribe s_timeFeed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/time");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  pinMode(emerLED, OUTPUT);
  pinMode(slowLED, OUTPUT);
  //pinMode(weather, OUTPUT);
  pinMode(timeChanged, OUTPUT);
  pinMode(isRed, INPUT);
  digitalWrite(slowLED, LOW);
  digitalWrite(emerLED, LOW);
  //digitalWrite(weather, LOW);
  digitalWrite(timeChanged, LOW);
  Serial.begin(9600);
  dht.begin();
  delay(10);

  //Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  //Serial.println(); Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println();

  //Serial.println("WiFi connected");
  //Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&s_controlFeed);
  mqtt.subscribe(&s_humiTempFeed);
  mqtt.subscribe(&s_timeFeed);
}

//uint32_t x=0;
uint32_t humi=0;
uint32_t temp=0;


void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &s_controlFeed) {
      //Serial.print(F("Got: "));
      //Serial.println((char *)s_controlFeed.lastread);
      deserializeJson(cmd, (char*)s_controlFeed.lastread);
      //Serial.println((String)cmd["slow"]);
      if((String)cmd["emer"] == "1"){
        digitalWrite(slowLED, LOW);
        digitalWrite(emerLED, HIGH);
       
      }
      if((String)cmd["slow"] == "1"){
        digitalWrite(slowLED, HIGH);
        digitalWrite(emerLED, LOW);
      
      }
      if((String)cmd["emer"] == "0" && (String)cmd["slow"] == "0"){
        digitalWrite(slowLED, LOW);
        digitalWrite(emerLED, LOW);
        
      }
    }
    if(subscription == &s_humiTempFeed){
        //Serial.print(F("Got Humi & Temp: "));
        //Serial.println((char *)s_humiTempFeed.lastread);
        
    }
    if(subscription == &s_timeFeed){
      //Serial.print(F("Got: "));
      //Serial.println((char *)s_timeFeed.lastread);
      deserializeJson(timeCmd, (char*)s_timeFeed.lastread);
      String cmdTimeUart = "!set-cuTime-";
      cmdTimeUart += (String)timeCmd["valueTime"];
      if((String)timeCmd["changed"] == "1"){
        digitalWrite(timeChanged, HIGH);
        Serial.println(cmdTimeUart);
      }
      else digitalWrite(timeChanged, LOW);
      
    }
  }

  // Now we can publish stuff!
  //Serial.print(F("\nSending Humi & Temp val "));
  //Serial.print(x);
  //Serial.print("...");
  DynamicJsonDocument resp(1024); 
  String cmdText ; 
  
  humi = dht.readHumidity();
  temp = dht.readTemperature();

  resp["humi"] = humi;
  resp["temp"] = temp;
  serializeJson(resp, cmdText);
  const char* cmdResp = cmdText.c_str();
  p_humiTempFeed.publish(cmdResp);
//  if (! p_humiTempFeed.publish(cmdResp)) {
//    Serial.println(F("Failed"));
//  } else {
//    Serial.println(F("OK!"));
//  }
//  if(digitalRead(isRed) == HIGH){
//    p_redFeed.publish(1);
//  }else{
//    p_redFeed.publish(0);
//  }
//  delay(200);
  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  //Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       //Serial.println(mqtt.connectErrorString(ret));
       //Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  //Serial.println("MQTT Connected!");
}
