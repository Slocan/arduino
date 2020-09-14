// Hardware: ESP32 - DHT11
// Purpose: Tracks the temperature and humidity, and sends the data using line protocol to Influxdb
// Authorization_header is used to authenticate to the web server

// Wifi functions from https://techtutorialsx.com/2017/06/29/esp32-arduino-getting-started-with-wifi/

#include <WiFi.h>
#include <HTTPClient.h>

#include "DHT.h"

#define DHTPIN 4     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11
// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "ssid";
const char* password =  "secret";
const char* url = "https://metrics.website.com/write?db=telegraf";
const char* authorization_header = "BASIC aaaaaaaaaaaaaaaaaaaaaaaaaaaa==";

// Utility functions
String translateEncryptionType(wifi_auth_mode_t encryptionType) {
  switch (encryptionType) {
    case (WIFI_AUTH_OPEN):
      return "Open";
    case (WIFI_AUTH_WEP):
      return "WEP";
    case (WIFI_AUTH_WPA_PSK):
      return "WPA_PSK";
    case (WIFI_AUTH_WPA2_PSK):
      return "WPA2_PSK";
    case (WIFI_AUTH_WPA_WPA2_PSK):
      return "WPA_WPA2_PSK";
    case (WIFI_AUTH_WPA2_ENTERPRISE):
      return "WPA2_ENTERPRISE";
  }
}
 
void scanNetworks() {
 
  int numberOfNetworks = WiFi.scanNetworks();
 
  Serial.print("Number of networks found: ");
  Serial.println(numberOfNetworks);
 
  for (int i = 0; i < numberOfNetworks; i++) {
 
    Serial.print("Network name: ");
    Serial.println(WiFi.SSID(i));
 
    Serial.print("Signal strength: ");
    Serial.println(WiFi.RSSI(i));
 
    Serial.print("MAC address: ");
    Serial.println(WiFi.BSSIDstr(i));
 
    Serial.print("Encryption type: ");
    String encryptionTypeDescription = translateEncryptionType(WiFi.encryptionType(i));
    Serial.println(encryptionTypeDescription);
    Serial.println("-----------------------");
 
  }
}
 
void connectToNetwork() {
  WiFi.begin(ssid, password);
 
  delay(1000);
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to network");
  } else {
    Serial.println("Connection failed");
  }
 
}
 
void setup() {
  Serial.begin(115200);
 
  // Initialize the DHT sensor
  dht.begin();
 
}
 
void loop() {
  delay(10000);
  connectToNetwork();

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  String out1 = "weather,sensor=esp32 temperature=";
  String output = String( out1 + t + ",humidity="+h);
  Serial.println(output);

  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
  
   HTTPClient http;   
  
   http.begin(url);
   http.addHeader("Content-Type", "text/plain");
   http.addHeader("Authorization", authorization_header);
  
   int httpResponseCode = http.POST(output);   //Send the actual POST request
  
   if(httpResponseCode>0){
  
    String response = http.getString();                       //Get the response to the request
  
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
  
   }else{
  
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  
   }
  
   http.end();  //Free resources
  
 }else{
  
    Serial.println("Error in WiFi connection");   
  
 }
 WiFi.disconnect(true);
  
}
