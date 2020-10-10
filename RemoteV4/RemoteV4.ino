

/*------------------------------------------------------------------------------
  10/01/2018
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: simple_ota.ino
  ------------------------------------------------------------------------------
  ------------------------------------------------------------------------------*/

//#ifndef BEENHERE            // Using variables across multiple tabs This is the MyProject.ino file
//#include "header.h"
//#endif

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include "2SPIFFS.h"

#include "3json.h"
#include "5timer.h"
#include "3WebUpdater.h"
//#include "4Blynk.h"

void setup() {


  Serial.begin(115200);
  Serial.println("Booting");

  startSPIFFS();               // Start the SPIFFS and list all contents
  delay(500);
  loadConfig();
  Serial.println("loaded config");
  Serial.println(config.client_SSID);

  WiFi.mode(WIFI_OFF);
  WiFi.persistent(false);
 // WiFi.mode(WIFI_AP_STA);
  WiFi.hostname(config.hostName);

  //not tried yet
  Serial.print("strlen:");
  Serial.println(strlen(config.client_SSID));
  if (strlen(config.client_SSID) == 0) {
    WiFi.mode(WIFI_STA);
    WiFi.softAP(config.AP_SSID, config.AP_password);
    Serial.println("loaded APSSID");
    Serial.println("Client Mode not Setup, go to 192.168.1.4...");
  } else {
    WiFi.mode(WIFI_STA);
    Serial.print("ClientSSID is:");
    Serial.println(config.client_SSID);
    WiFi.begin(config.client_SSID, config.client_password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connection Failed! Rebooting...");
      delay(5000);
      ESP.restart();
    }
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  startMDNS();                 // Start the mDNS responder

  startServer();               // Start a HTTP server with a file read handler and an upload handler

  startUDP();                  // Start listening for UDP messages to port 123

  setupFunctions();
}



void loop() {
  server.handleClient();
}
