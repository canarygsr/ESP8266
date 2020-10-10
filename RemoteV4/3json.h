#include <ArduinoJson.h>
#include "FS.h"

const char* client_SSID_default = "";
const char* client_password_default = "password";

struct Config {
  char hostName[64];
  char MDSNname[64];
  char password[64];
  char client_SSID[64];
  char client_password[64];
  char AP_SSID[64];
  char AP_password[64];
  char passcode0[64];
  char passcode1[64];
  char passcode2[64];
  char passcode3[64];
  int function1_gpio;
  int function2_gpio;
  bool function1_high_low;
  bool function2_high_low;
};
Config config;

bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    //return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  // StaticJsonDocument<512> json;
  const size_t capacity = JSON_OBJECT_SIZE(15)  + 830;;
  DynamicJsonDocument json(capacity);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  //  configFile.readBytes(buf.get(), size);

  // StaticJsonBuffer<200> jsonBuffer;
  //  JsonObject& json = jsonBuffer.parseObject(buf.get());
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(json, configFile);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  // Copy values from the JsonDocument to the Config
  strlcpy(config.MDSNname,                  // <- destination
          json["MDSNname"] | "default.example.com",  // <- source | and default value, change for
          sizeof(config.MDSNname));        // <- destination's capacity
  strlcpy(config.hostName, json["hostName"] | "hostName_default", sizeof(config.hostName));
  strlcpy(config.password, json["password"] | "password_default", sizeof(config.password));
  strlcpy(config.client_password, json["client_password"] | client_password_default, sizeof(config.client_password));
  strlcpy(config.client_SSID, json["client_SSID"] | client_SSID_default, sizeof(config.client_SSID));
  strlcpy(config.AP_SSID, json["AP_SSID"] | "AP_SSID_default", sizeof(config.AP_SSID));
  strlcpy(config.AP_password, json["AP_password"] | "AP_password_default", sizeof(config.AP_password));
  strlcpy(config.passcode0, json["passcode0"] | "passcode0_default", sizeof(config.passcode0));
  strlcpy(config.passcode1, json["passcode1"] | "passcode1_default", sizeof(config.passcode1));
  strlcpy(config.passcode2, json["passcode2"] | "passcode2_default", sizeof(config.passcode2));
  strlcpy(config.passcode3, json["passcode3"] | "passcod3_default", sizeof(config.passcode3));
  config.function1_gpio = json["function1_gpio"] | 1;
  config.function2_gpio = json["function2_gpio"] | 2;
  //added below
  config.function1_high_low = json["function1_high_low"] | false;
  config.function2_high_low = json["function2_high_low"] | true;

  //Print settings to serial
  Serial.print("hostName: ");
  Serial.println(config.hostName);
  Serial.print("MDSNname: ");
  Serial.println(config.MDSNname);
  Serial.print("password: ");
  Serial.println(config.password);
  Serial.print("client_SSID: ");
  Serial.println(config.client_SSID);
  Serial.print("client_password: ");
  Serial.println(config.client_password);
  Serial.print("AP_SSID: ");
  Serial.println(config.AP_SSID);
  Serial.print("AP_password: ");
  Serial.println(config.AP_password);
  Serial.print("Passcode0: ");
  Serial.println(config.passcode0);
  Serial.print("Passcode1: ");
  Serial.println(config.passcode1);
  Serial.print("Passcode2: ");
  Serial.println(config.passcode2);
  Serial.print("Passcode3: ");
  Serial.println(config.passcode3);
  Serial.print("function1_gpio: ");
  Serial.println(config.function1_gpio);
  Serial.print("function2_gpio: ");
  Serial.println(config.function2_gpio);
  Serial.print("function1_high_low: ");
  Serial.println(config.function1_high_low);
  Serial.print("function2_high_low: ");
  Serial.println(config.function2_high_low);
  
  return true;
}
//String data1;
void webConfig(String data1) {
 
 const size_t capacity = JSON_OBJECT_SIZE(15)  + 830;;
  DynamicJsonDocument json(capacity);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  //  configFile.readBytes(buf.get(), size);

  // StaticJsonBuffer<200> jsonBuffer;
  //  JsonObject& json = jsonBuffer.parseObject(buf.get());
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(json, data1);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  // Copy values from the JsonDocument to the Config
  strlcpy(config.hostName, json["hostName"] | config.hostName, sizeof(config.hostName));
  strlcpy(config.MDSNname, json["MDSNname"] | config.MDSNname, sizeof(config.MDSNname));        // <- destination's capacity
  strlcpy(config.password, json["password"] | config.password, sizeof(config.password));
  strlcpy(config.client_password, json["client_password"] | config.client_password, sizeof(config.client_password));
  strlcpy(config.client_SSID, json["client_SSID"] | config.client_SSID, sizeof(config.client_SSID));
  strlcpy(config.AP_SSID, json["AP_SSID"] | config.AP_SSID, sizeof(config.AP_SSID));
  strlcpy(config.AP_password, json["AP_password"] | "AP_password_default", sizeof(config.AP_password));
  strlcpy(config.passcode0, json["passcode0"] | config.passcode0, sizeof(config.passcode0));
  strlcpy(config.passcode1, json["passcode1"] | config.passcode1, sizeof(config.passcode1));
  strlcpy(config.passcode2, json["passcode2"] | config.passcode2, sizeof(config.passcode2));
  strlcpy(config.passcode3, json["passcode3"] | config.passcode3, sizeof(config.passcode3));
  config.function1_gpio = json["function1_gpio"] | config.function1_gpio;
  config.function2_gpio = json["function2_gpio"] | config.function2_gpio;
//added below
config.function1_high_low = json["function1_high_low"] | config.function1_high_low;
config.function2_high_low = json["function2_high_low"] | config.function2_high_low;

  //Print settings to serial
  Serial.print("hostName: ");
  Serial.println(config.hostName);  
  Serial.print("MDSNname: ");
  Serial.println(config.MDSNname);
  Serial.print("password: ");
  Serial.println(config.password);
  Serial.print("client_SSID: ");
  Serial.println(config.client_SSID);
  Serial.print("client_password: ");
  Serial.println(config.client_password);
  Serial.print("AP_SSID: ");
  Serial.println(config.AP_SSID);
  Serial.print("AP_password: ");
  Serial.println(config.AP_password);
  Serial.print("Passcode0: ");
  Serial.println(config.passcode0);
  Serial.print("Passcode1: ");
  Serial.println(config.passcode1);
  Serial.print("Passcode2: ");
  Serial.println(config.passcode2);
  Serial.print("Passcode3: ");
  Serial.println(config.passcode3);
  Serial.print("function1_gpio: ");
  Serial.println(config.function1_gpio);
  Serial.print("function2_gpio: ");
  Serial.println(config.function2_gpio);
  Serial.print("function1_high_low: ");
  Serial.println(config.function1_high_low);
  Serial.print("function2_high_low: ");
  Serial.println(config.function2_high_low);
  //return true;
}

bool saveConfig() {
  const size_t capacity = JSON_OBJECT_SIZE(15) + 410;
  //StaticJsonDocument<512> json;
  DynamicJsonDocument json(capacity);

  //  JsonObject& json = jsonBuffer.createObject();
  json["hostName"] = config.hostName;
  json["MDSNname"] = config.MDSNname;
  json["password"] = config.password;
  json["client_SSID"] = config.client_SSID;
  json["client_password"] = config.client_password;
  json["AP_SSID"] = config.AP_SSID;
  json["AP_password"] = config.AP_password;
  json["passcode0"] = config.passcode0;
  json["passcode1"] = config.passcode1;
  json["passcode2"] = config.passcode2;
  json["passcode3"] = config.passcode3;
  json["function1_gpio"] = config.function1_gpio;
  json["function2_gpio"] = config.function2_gpio;
// added below
  json["function1_high_low"] = config.function1_high_low;
  json["function2_high_low"] = config.function2_high_low;
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  // Serialize JSON to file
  if (serializeJson(json, configFile) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  return true;
}
