#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define USE_SERIAL Serial
ESP8266WiFiMulti WiFiMulti;
#include <ArduinoJson.h>

void setup() {
  // define output pins
  pinMode(14,OUTPUT); //D5
  pinMode(12,OUTPUT); //D6
  pinMode(13,OUTPUT); //D7
  pinMode(15,OUTPUT); //D8
  
  // put your setup code here, to run once:
   USE_SERIAL.begin(9600);
  // USE_SERIAL.setDebugOutput(true);
   USE_SERIAL.println();
   USE_SERIAL.println();
   USE_SERIAL.println();
   for(uint8_t t = 4; t > 0; t--) {
     //  USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
       USE_SERIAL.flush();
       delay(1000);
   }
   WiFi.mode(WIFI_STA);
   // provide our SSID and Password for WIFI network connection
   WiFiMulti.addAP("SLT-4G_914BF", "prolink12345");
}

void loop() {
    // wait for WiFi connection
   if((WiFiMulti.run() == WL_CONNECTED)) {
       HTTPClient http;
       USE_SERIAL.println("Sending Get Request to Server.......");
       http.begin("http://192.168.1.10:5000/api"); //HTTP URL for hosted server(local server)
       //192.168.1.10 - HOST     PORT: 5000 and /api is the target api we need to hit to get response
       int httpCode = http.GET();
      // USE_SERIAL.println("After GET Request");
       // httpCode will be negative on error
       if(httpCode > 0) {
        // Parsing
        const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
        DynamicJsonBuffer jsonBuffer(bufferSize);
        JsonObject& root = jsonBuffer.parseObject(http.getString());

        String pinD5 = root["D1"];
        String pinD6 = root["D2"];
        String pinD7 = root["D3"];
        String pinD8 = root["D4"];

        Serial.println(pinD5);
        Serial.println(pinD6);
        Serial.println(pinD7);
        Serial.println(pinD8);

        if(pinD5 == "true"){
          digitalWrite(14,HIGH);
        }else{
          digitalWrite(14,LOW);
        }

        if(pinD6 == "true"){
          digitalWrite(12,HIGH);
        }else{
          digitalWrite(12,LOW);
        }

        if(pinD7 == "true"){
          digitalWrite(13,HIGH);
        }else{
          digitalWrite(13,LOW);
        }

        if(pinD8 == "true"){
          digitalWrite(15,HIGH);
        }else{
          digitalWrite(15,LOW);
        }
        
       } else {
           USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
       }
       http.end();
   }
   delay(5000);// repeat the cycle every 5 seconds.

}
