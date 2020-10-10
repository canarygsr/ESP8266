#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
//#include "2SPIFFS.h"
#include "7Function.h"

ESP8266WebServer server(80);             // create a web server on port 80
ESP8266HTTPUpdateServer httpUpdater;
File fsUploadFile;                                    // a File variable to temporarily store the received file

String code;
//String call;
int call;

String getContentType(String filename) { // determine the filetype of a given filename, based on the extension
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

/*__________________________________________________________WEB_FUNCTIONS - Using SPIFFS__________________________________________________________*/


bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";          // If a folder is requested, send the index file
  String contentType = getContentType(path);             // Get the MIME type
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) { // If the file exists, either as a compressed archive, or normal
    if (SPIFFS.exists(pathWithGz))                         // If there's a compressed version available
      path += ".gz";                                         // Use the compressed verion
    File file = SPIFFS.open(path, "r");                    // Open the file
    size_t sent = server.streamFile(file, contentType);    // Send it to the client
    file.close();                                          // Close the file again
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);   // If the file doesn't exist, return false
  return false;
}

void handleFileUpload() { // upload a new file to the SPIFFS
  HTTPUpload& upload = server.upload();
  String path;
  if (upload.status == UPLOAD_FILE_START) {
    path = upload.filename;
    if (!path.startsWith("/")) path = "/" + path;
    if (!path.endsWith(".gz")) {                         // The file server always prefers a compressed version of a file
      String pathWithGz = path + ".gz";                  // So if an uploaded file is not compressed, the existing compressed
      if (SPIFFS.exists(pathWithGz))                     // version of that file must be deleted (if it exists)
        SPIFFS.remove(pathWithGz);
    }
    Serial.print("handleFileUpload Name: "); Serial.println(path);
    fsUploadFile = SPIFFS.open(path, "w");               // Open the file for writing in SPIFFS (create if it doesn't exist)
    path = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize); // Write the received bytes to the file
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {                                   // If the file was successfully created
      fsUploadFile.close();                               // Close the file again
      Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
      server.sendHeader("Location", "/success.html");     // Redirect the client to the success page
      server.send(303);
    } else {
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
}

void handleNotFound() { // if the requested file or page doesn't exist, return a 404 not found error
  if (!handleFileRead(server.uri())) {        // check if the file exists in the flash memory (SPIFFS), if so, send it
    server.send(404, "text/plain", "404: File Not Found");
  }
}

//version 1
/*
void handlepraise() {
    code = server.arg("Code"); //.toInt(); // remove toInt
    call = server.arg("String"); //.toInt(); // remove toInt
   Serial.print("Code: ");
   Serial.println(code);
   Serial.print("String: ");
   Serial.println(call);
    if (code == config.passcode1 and call == "Function_1")
    {
      Serial.println("Function 1 Called");
      Function_1();
      server.send(200, "text/plain", "Function 1 Complete");
      Serial.println("Function 1 Complete");
    }
    else
    {
     if (code == config.passcode1 and call == "Function_2")
    {
      Serial.println("Function 2 Called");
      Function_2();
      server.send(200, "text/plain", "Function 2 Complete");
      Serial.println("Function 2 Complete");
    }
    else
    {
      Serial.println("Function or Code Incorrect and unable to praise");
      server.send(404, "text/plain", "404: File Not Found");
    }
    }
    
}
*/
// version 2
void handlepraise() {
    code = server.arg("Code"); //.toInt(); // remove toInt
    call = server.arg("String").toInt(); // remove toInt
   Serial.print("Code: ");
   Serial.println(code);
   Serial.print("String: ");
   Serial.println(call);
    if (code == config.passcode1)
    {
      switch (call) { 
      case 1:
      GPIO_toggle (config.function1_gpio, config.function1_high_low);
      server.send(200, "text/plain", "Function 1 Complete");
      Serial.println("Function 1 Complete");
      break;
      case 2:
      GPIO_toggle (config.function2_gpio, config.function2_high_low);
      server.send(200, "text/plain", "Function 2 Complete");
      Serial.println("Function 2 Complete");
      break;
      default:
      server.send(200, "text/plain", "Incorrect Function Called");
      Serial.println("Incorrect Function Called");
      break;
    }}
    else
    {
      Serial.println("Function or Code Incorrect and unable to praise");
      server.send(404, "text/plain", "404: File Not Found");
    }
  }

void settingspraise() {
  String data = server.arg("plain");
  Serial.println(data);
  webConfig(data);
  saveConfig();
}

/*__________________________________________________________WEBSERVER_Actions and Setup__________________________________________________________*/


void startServer() { // Start a HTTP server with a file read handler and an upload handler

  server.on("/edit.html",  HTTP_POST, []() {  // If a POST request is sent to the /edit.html address,
    server.send(200, "text/plain", "");
  }, handleFileUpload);                       // go to 'handleFileUpload'

  server.onNotFound(handleNotFound);          // if someone requests any other file or page, go to function 'handleNotFound'
  // and check if the file exists

  server.on("/restart", []() {
    server.send(200, "text/plain", "Restarting...");
    WiFi.disconnect(true);
    delay(1000);
    ESP.restart();
  });


  server.on("/del", []() {
    server.send(200, "text/plain", "Deleting files...");
    //SPIFFS.remove("/config.json");
    SPIFFS.format();
    WiFi.disconnect(true);
    
    server.send(200, "text/plain", "Restarting...");
  });

  server.on("/files", []() {
    server.send(200, "text/plain", "Deleting files...");
    // code to display SPIFFS contense
    server.send(200, "text/plain", "Restarting...");
  });
  server.on("/praise.html", []() {
     handlepraise();
    server.send(200, "text/plain", "Deleting files...");
    // code to display SPIFFS contense
  });

  server.on("/updatesettings", []() {
     settingspraise();
    server.send(200, "text/plain", "Settings Updated...");
    // code to display SPIFFS contense
  });


//server.on settings...goes here

  httpUpdater.setup(&server);
  server.begin();                             // start the HTTP server
  Serial.println("HTTP server started.");
}
