#include "utils.hpp"
#include <SPIFFS.h>
#include <WiFi.h>
#include <uri/UriBraces.h>
#include <WebServer.h>

#define WIFI_CONNECTION_TIMEOUT 16000

WebServer* webServer;

String getHexColor(int color) {
  char hexColor[8] = "";
  
  sprintf(hexColor, "#%06X", color);
  return String(hexColor);
}

void setupWifi(const char* ssid, const char* password) {
  unsigned long currentTime = 0;

  // Turn on WiFi
  WiFi.mode(WIFI_STA);

  // Connect to WiFi
  Serial.printf("%s %s", "Connecting to", ssid);
  currentTime = millis();
  WiFi.begin(ssid, password);
  do {
    Serial.print('.');
    delay(400);
  } while ((WiFi.status() != WL_CONNECTED) && (millis() - currentTime < WIFI_CONNECTION_TIMEOUT));
  Serial.println();

  // Check if is connected to WiFi
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("%s %s\n", "Successful connection to", ssid);
  } else {
    throw (String("Failed to connect to ") + String(ssid)).c_str();
  }
}

void initWebServer(LedStrip* ledStrip, int port) {
  // Initialize web server
  webServer = new WebServer(port);

  // Check if it is possible to initialize SPIFFS
  if(!SPIFFS.begin(true)) {
    throw "Unable to initialize SPIFFS";
  }

  // Static Server
  webServer->on("/", HTTP_GET, [ledStrip]() {
    String file = SPIFFS.open("/index.html", "r").readString();

    file.replace("{{version}}", FIRMWARE_VERSION);
    file.replace("{{state}}", ledStrip->getState() ? "true" : "false");
    file.replace("{{brightness}}", String((int)(ledStrip->getBrightness() * 100)));
    file.replace("{{color}}", getHexColor(ledStrip->getColor()));
    webServer->send(200, "text/html", file);
  });
  webServer->serveStatic("/styles/style.css", SPIFFS, "/styles/style.css");
  webServer->serveStatic("/scripts/script.js", SPIFFS, "/scripts/script.js");
  webServer->serveStatic("/fonts/Gilroy_Light.otf", SPIFFS, "/fonts/Gilroy_Light.otf");
  webServer->serveStatic("/fonts/Gilroy_ExtraBold.otf", SPIFFS, "/fonts/Gilroy_ExtraBold.otf");

  // REST API
  webServer->on("/state", HTTP_GET, [ledStrip]() {
    webServer->send(200, "text/plain", "The mirror has turned " + String(ledStrip->getState() ? "on" : "off"));
  });
  webServer->on(UriBraces("/state/{}"), HTTP_POST, [ledStrip]() {
    String value = webServer->pathArg(0);

    try {
      if (value.equals("on")) {
        ledStrip->on();
        webServer->send(200, "text/plain", "The mirror has turned on");
      } else if (value.equals("off")) {
        ledStrip->off();
        webServer->send(200, "text/plain", "The mirror has turned off");
      } else {
        throw "Failed to turn mirror on/off";
      }
    } catch (const char* ex) {
      String error = ex;

      error.toLowerCase();
      webServer->send(400, "text/plain", "ERR: " + error);
    }
  });
  webServer->on("/brightness", HTTP_GET, [ledStrip]() {
    webServer->send(200, "text/plain", "The mirror brightness is at " + String((int)(100 * ledStrip->getBrightness())) + " %");
  });
  webServer->on(UriBraces("/brightness/{}"), HTTP_POST, [ledStrip]() {
    int value = webServer->pathArg(0).toInt();
        
    try {
      ledStrip->setBrightness(((float)value) / 100);
      webServer->send(200, "text/plain", "Brightness has been changed to " + String(value) + " %");
    } catch (const char* ex) {
      String error = ex;

      error.replace("0.0", "0");
      error.replace("1.0", "100");
      error.toLowerCase();
      webServer->send(400, "text/plain", "ERR: " + error);
    }
  });
  webServer->on("/color", HTTP_GET, [ledStrip]() {
    webServer->send(200, "text/plain", "The mirror color is: " + getHexColor(ledStrip->getColor()));
  });
  webServer->on(UriBraces("/color/{}"), HTTP_POST, [ledStrip]() {
    int value = (int)strtol(webServer->pathArg(0).c_str(), NULL, 16);

    try {
      ledStrip->setColor(value);
      webServer->send(200, "text/plain", "Color has been changed to " + getHexColor(ledStrip->getColor()));
    } catch (const char* ex) {
      String error = ex;

      error.toLowerCase();
      webServer->send(400, "text/plain", "ERR: " + error);
    }
  });
  webServer->onNotFound([]() {
    webServer->send(404, "text/plain", "ERR: not found");
  });

  // Begin Web Server
  webServer->enableCORS(true);
  webServer->begin();
  Serial.printf("%s%s\n", "Web Server running in http://", WiFi.localIP().toString());
}

void loopTask() {
  if (webServer != NULL) {
    webServer->handleClient();
  }
}