


#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

// ================= WIFI =================
const char* ssid = "youpilab_fibre 2G";
const char* password = "Washingi_loV3_yl2025Fibre#Cit1";



// ================= API =================
const String BASE_URL = "https://iot.youpilab.com/api";
const String APP_ID   = "qua22167";   
const String APP_KEY  = "ab6497fa";   

// ================= PINS =================
#define MQ6_PIN A0 

// ================= TIMERS =================
unsigned long previousMillis = 0;
const long interval = 1000; 

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\n[WIFI] Connecté !");
}

// ================= ENVOI DONNÉES MQ6 =================
void sendMQ6Data(int valeurGaz) {
  if (WiFi.status() != WL_CONNECTED) return;

  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  
  String url = BASE_URL + "/data/send?APP_ID=" + APP_ID + "&APP_KEY=" + APP_KEY + "&gaz=" + String(valeurGaz);

  http.begin(client, url);
  int httpCode = http.GET();
  
  Serial.print("[HTTP SEND] Code : ");
  Serial.println(httpCode);
  http.end();
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
}

void loop() {
  
  int lectureGaz = analogRead(MQ6_PIN);
  
  Serial.print("Qualite air (valeur brute) : ");
  Serial.println(lectureGaz);

 
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    sendMQ6Data(lectureGaz);
  }

  delay(500);
}