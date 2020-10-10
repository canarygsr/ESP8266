#include <FS.h>
//json.h tab to load json.settings
/*__________________________________________________________SPIFFS_HANDLERS__________________________________________________________*/

void spiffscontence() {
  Serial.println("SPIFFS updated Contents below:");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    Serial.print(dir.fileName() + ": ");
    if (dir.fileSize()) {
      File f = dir.openFile("r");
      Serial.println(f.size());
    }
  }
}


/*
 void writetospiffs() {
  timenow ();
  if (SPIFFS.exists("/log.csv")) {

    File f = SPIFFS.open("/log.csv", "a");
    if (!f) {
      Serial.println("file open failed");
    }
    else {

      Serial.println("====== Writing to SPIFFS file{i} =========");
      // write  strings to file
      f.print(timenow());
      for (int i = 0; i < NUM_MAX31856; i++) {
        f.print(", ");
        f.print(Probe[i].CurrentTemp);
      }
      f.println();
    }
    f.close();
  }
  else
  {
    File f = SPIFFS.open("/log.csv", "w");
    if (!f) {
    Serial.println("failed to create new file");
      }
    else {

    Serial.println("====== Writing to SPIFFS file{i} =========");
    // write  strings to file
    f.print(timenow());
    for (int i = 0; i < NUM_MAX31856; i++) {
      f.print(", ");
      f.print(Probe[i].CurrentTemp);
    }
    f.println();
  }
  f.close();
  }
  Serial.println("/log.csv updated");
}
*/

void startSPIFFS() { // Start the SPIFFS and list all contents
  Serial.println("Mounting FS...");

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  } else
  {
    if (!SPIFFS.exists("/formatComplete.txt")) {
    Serial.println("Please wait 30 secs for SPIFFS to be formatted");
    SPIFFS.format();
    Serial.println("Spiffs formatted");
    
    File f = SPIFFS.open("/formatComplete.txt", "w");
    if (!f) {
        Serial.println("file open failed");
    } else {
        f.println("Format Complete");
    }
  } else {
    Serial.println("SPIFFS is formatted. Moving along...");
  }
    spiffscontence();
  }
}
