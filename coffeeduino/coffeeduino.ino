/*
  To upload through terminal you can use: curl -u admin:admin -F "image=@firmware.bin" esp8266-webupdate.local/firmware
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

#include "credentials.h"

//#ifndef STASSID
//#define STASSID "abc"
//#define STAPSK  "123"
//#endif

const char* host = "esp8266-webupdate";
const char* update_path = "/firmware";
const char* update_username = UPDATE_USER;
const char* update_password = UPDATE_PASS;
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup(void) {

  Serial.begin(115200);

  Serial.println();
  Serial.print("Booting Sketch...");

  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println("WiFi failed, retrying.");
  }

  MDNS.begin(host);

  httpUpdater.setup(&httpServer, update_path, update_username, update_password);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);

  Serial.println("HTTPUpdateServer ready!\n");
  Serial.printf("Open http://%s.local%s and login with username = '%s' / password = '%s'\n", host, update_path, update_username, update_password);
}

void loop(void) {
  httpServer.handleClient();
  MDNS.update();
}
