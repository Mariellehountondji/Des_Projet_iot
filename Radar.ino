



#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

// ================= WIFI =================
const char* ssid = "youpilab_fibre 2G";
const char* password = "Washingi_loV3_yl2025Fibre#Cit1";

// ================= API =================
const String BASE_URL  = "https://iot.youpilab.com/api";


const String APP_ID  = "rad54b09";     
const String APP_KEY = "a10e30e9";     

// ================= PINS (ESP8266) =================
#define TRIG_PIN 14  // Broche D5 sur la carte
#define ECHO_PIN 12  // Broche D6 sur la carte
#define LED_PIN  4   // Broche D2 sur la carte

// ================= VARIABLES =================
const int seuilDistance = 50; 
long duree;
int distance;


unsigned long previousMillis = 0;
const long interval = 3000;    // Intervalle d'envoi des données (3 secondes)

// ================= WIFI SETUP =================
void setup_wifi()
{
  WiFi.begin(ssid, password);
  Serial.print("[WIFI] Connexion");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n[WIFI] Connecté !");
  Serial.println(WiFi.localIP());
}




void sendRadarData(int dist)
{
  if (WiFi.status() != WL_CONNECTED) return;

 
  WiFiClientSecure client;
  
  
  client.setInsecure();

  HTTPClient http;

  
  String url =
    BASE_URL +
    "/data/send?APP_ID=" + APP_ID +
    "&APP_KEY=" + APP_KEY +
    "&distance=" + String(dist); 

  
  http.begin(client, url);

  int httpCode = http.GET();

  Serial.print("[HTTP SEND] Code Retour : ");
  Serial.println(httpCode); 

  if (httpCode > 0)
  {
    String payload = http.getString();
    Serial.print("[REPONSE YOUPILAB] : ");
    Serial.println(payload);
  }
  else 
  {
    Serial.print("[HTTP ERROR] Échec de la connexion sécurisée, erreur : ");
    Serial.println(http.errorToString(httpCode).c_str());
  }

  http.end();
}



void setup()
{
  Serial.begin(115200);

  // Configuration des broches
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // Initialisation du Wi-Fi
  setup_wifi();
}


void loop()
{
 
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duree = pulseIn(ECHO_PIN, HIGH);
  distance = duree * 0.034 / 2; 

  
  Serial.print("Distance actuelle: ");
  Serial.print(distance);
  Serial.println(" cm");

 
  if (distance > 0 && distance <= seuilDistance)
  {
    digitalWrite(LED_PIN, HIGH); // Présence détectée -> la LED reste allumée
    Serial.println("[RADAR] Présence détectée !");
  }
  else
  {
    digitalWrite(LED_PIN, LOW);  // Pas de présence -> la LED s'éteint
  }

  // 3. ===== ENVOI DES DONNÉES SUR L'API YOUPILAB =====
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    
    if (distance > 0 && distance < 400) 
    {
      sendRadarData(distance);
    }
  }

  delay(100); 
}
