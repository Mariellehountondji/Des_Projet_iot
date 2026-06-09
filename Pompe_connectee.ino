


#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "youpilab_fibre 2G";
const char* password = "Washingi_loV3_yl2025Fibre#Cit1";

const String BASE_URL = "http://iot.youpilab.com/api";
const String APP_ID   = "sysaebed"; 
const String APP_KEY  = "300aad7d"; 

#define RELAY_PIN 12 // D6

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); 
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
}

void checkControls() {
  WiFiClient client;
  HTTPClient http;
  String url = BASE_URL + "/controls/get?APP_ID=" + APP_ID + "&APP_KEY=" + APP_KEY;
  
  if (http.begin(client, url)) {
    int httpCode = http.GET();
    if (httpCode == 200) {
      String payload = http.getString();
      StaticJsonDocument<1024> doc;
      deserializeJson(doc, payload);
      
      JsonArray arr = doc.as<JsonArray>();
      if (arr.size() > 0) {
        /
        String lastValue = arr[arr.size() - 1].as<String>();
        
        Serial.println("Derniere valeur : " + lastValue);
        
        // Commande du relais
        if (lastValue == "1") {
          digitalWrite(RELAY_PIN, HIGH);  
          Serial.println("Action : POMPE ON");
        } else {
          digitalWrite(RELAY_PIN, LOW); 
          Serial.println("Action : POMPE OFF");
        }
      }
    }
    http.end();
  }
}

void loop() {
  checkControls();
  delay(1000); 
}