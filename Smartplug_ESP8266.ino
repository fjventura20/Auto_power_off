/*
Please replace <Your Printer IP>, <Your WiFi SSID>, <Your WiFi Password>, and <Your Webhooks Key> 
with your actual information. Remember to keep this information secure and do not share it publicly.
*/

#include <ESP8266WiFi.h>
#include <ESP8266Webhook.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

DynamicJsonDocument doc(1024);
unsigned long Interval;                // milliseconds between updates
String PRINTER_IP = "<Your Printer IP>";  // the ip address of moonraker
unsigned long pollInterval = 3000;     //poll every 3 seconds
unsigned long lastUpdate;
uint8_t modus;
uint8_t lastModus;
uint8_t timeouts = 0;
int poweronflag = 0;
int printcompleteflag = 0;
int printpausedflag = 0;
String LASTSTATE = "";

//Querry URL
const char *prefix = "http://";
const char *postfix = "/printer/objects/query?print_stats";
String url = prefix + PRINTER_IP + postfix;
const char *URL = url.c_str();

// Insert your network credentials
#define WIFI_SSID "<Your WiFi SSID>"
#define WIFI_PASSWORD "<Your WiFi Password>"
#define KEY "<Your Webhooks Key>"  // Webhooks Key
#define EVENT "printerstate"          // Webhooks Event Name
#define EVENT2 "printerpaused"        // Webhooks Event Name
#define EVENT3 "printerpowered"       // Webhooks Event Name
Webhook webhook(KEY, EVENT);          // Create an object.
Webhook webhook2(KEY, EVENT2);        // Create an object.
Webhook webhook3(KEY, EVENT3);        // Create an object.

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println();
  Serial.print("Connecting to Wifi: ");
  Serial.print(WiFi.localIP());
  Serial.println();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  initWiFi();
}

void loop() {
  String state;  
  if (millis() - lastUpdate > pollInterval) {
    lastUpdate = millis();
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient c;
      HTTPClient http;
      http.begin(c, URL);
      int httpCode = http.GET();

      if (httpCode > 0) {
        timeouts = 0;
        String payload = http.getString();
        deserializeJson(doc, payload);
        JsonObject payloadObject = doc.as<JsonObject>();

        if (httpCode == HTTP_CODE_OK) {
          state = payloadObject["result"]["status"]["print_stats"]["state"].as<String>();  
          Serial.print("The printer state is: ");
          Serial.println(state);

          if (LASTSTATE == "standby" and state == "printing") {
            poweronflag = 0;
            printcompleteflag = 0;
            printpausedflag = 0;
          }
          if (poweronflag == 0) {
            webhook3.trigger(state);  
            poweronflag = 1;
          }
          if (state == "error") {
            modus = 0;  
          } else if (state == "printing") {
            modus = 3;  
          } else if (state == "standby") {
            modus = 4;  
          } else if (state == "paused") {
            modus = 5;  
            if (printpausedflag == 0) {
              webhook2.trigger(state);  
              printpausedflag = 1;
            }
          } else if (state == "complete" or state == "cancelled") {
            modus = 6;  
            if (printcompleteflag == 0) {
              webhook.trigger(state);  
              printcompleteflag = 1;
            }
          }
          LASTSTATE = state;
        }
      } else {
        timeouts++;
        if (timeouts > 10) {
          modus = 2;
        }
      }
    } else {
      modus = 1;
    }
  }
}
