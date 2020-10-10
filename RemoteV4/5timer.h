#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <Time.h>
#include <TimeLib.h>
//#include "3json.h"

/*#ifndef BEENHERE            // This is the top of the Support.cpp file
#include "header.h"
#endif*/

#define ONE_HOUR 3600000UL

IPAddress timeServerIP;        // The time.nist.gov NTP server's IP address
const char* ntpServerName = "time.nist.gov";

const int NTP_PACKET_SIZE = 48;          // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[NTP_PACKET_SIZE];      // A buffer to hold incoming and outgoing packets

String formatBytes(size_t bytes) { // convert sizes in bytes to KB and MB
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  }
}

//const char* mdnsName = "esp8266";        // Domain name for the mDNS responder

WiFiUDP UDP;                   // Create an instance of the WiFiUDP class to send and receive UDP messages

const unsigned long intervalNTP = ONE_HOUR; // Update the time every hour
unsigned long prevNTP = 0;
unsigned long lastNTPResponse = millis();
uint32_t timeUNIX = 0;                      // The most recent timestamp received from the time server
//uint32_t time = 0;
String timestring;
String datestring;
uint32_t hr = 0;
uint32_t mn = 0;
uint32_t sec = 0;

unsigned long getTime() { // Check if the time server has responded, if so, get the UNIX time, otherwise, return 0
  if (UDP.parsePacket() == 0) { // If there's no response (yet)
    return 0;
  }
  UDP.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
  // Combine the 4 timestamp bytes into one 32-bit number
  uint32_t NTPTime = (packetBuffer[40] << 24) | (packetBuffer[41] << 16) | (packetBuffer[42] << 8) | packetBuffer[43];
  // Convert NTP time to a UNIX timestamp:
  // Unix time starts on Jan 1 1970. That's 2208988800 seconds in NTP time:
  const uint32_t seventyYears = 2208988800UL;
  const uint32_t timeZone = 43200UL;
  // subtract seventy years:
  uint32_t UNIXTime = NTPTime - seventyYears + timeZone;
  return UNIXTime;
}

void startUDP() {
  Serial.println("Starting UDP");
  UDP.begin(123);                          // Start listening for UDP messages to port 123
  Serial.print("Local port:\t");
  Serial.println(UDP.localPort());
}


void startMDNS() { // Start the mDNS responder

  if (MDNS.begin("config.MDNSname")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
    Serial.print("mDNS responder started: http://");
    Serial.print("config.MDNSname");
    Serial.println(".local");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }
  //MDNS.begin(mdnsName);                        // start the multicast domain name server

}

void sendNTPpacket(IPAddress& address) {
  Serial.println("Sending NTP request");
  memset(packetBuffer, 0, NTP_PACKET_SIZE);  // set all bytes in the buffer to 0
  // Initialize values needed to form NTP request
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode

  // send a packet requesting a timestamp:
  UDP.beginPacket(address, 123); // NTP requests are to port 123
  UDP.write(packetBuffer, NTP_PACKET_SIZE);
  UDP.endPacket();
}

  String timenow () {
  //uint32_t time1 = getTime();
 // setTime(time1);
 String hr = String(hour());
 String mn = "";
 String sec = "";
 if (minute() < 10) {
 mn += "0";} 
 mn += String(minute());
  if (second() < 10) {
 sec += "0";}
 sec += String(second());
timestring = hr + ":" + mn + ":" + sec;
//Serial.println(timestring);
return timestring;
  }

  String datenow () {
  datestring = (String(day()) + "-" + String(month()) + "-" + String(year()));
  Serial.println(datestring);
  return datestring;
  }

void ntpstuff() {
  unsigned long currentMillis = millis();
  sendNTPpacket(timeServerIP);
  delay(1000);
   uint32_t time = getTime();                   // Check if the time server has responded, if so, get the UNIX time
    Serial.print("NPTstuf before loop time");
    Serial.println(timeUNIX);
  
  if (time > 0) {
    timeUNIX = time;
    setTime(time);
    Serial.print("NPTstuff if time is");
    Serial.println(timeUNIX);
    lastNTPResponse = millis();
  } else if ((millis() - lastNTPResponse) > 24UL * ONE_HOUR) {
    Serial.println("More than 24 hours since last NTP response. Rebooting.");
    Serial.flush();
    ESP.reset();
  }
  }

  
  
