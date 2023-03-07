#include <Arduino.h>
#include "led_strip.hpp"
#include "utils.hpp"

#define WIFI_SSID "REPLACE_WITH_YOUR_SSID"
#define WIFI_PASSWORD "REPLACE_WITH_YOUR_PASSWORD"

LedStrip* ledStrip;

void setup() {
  Serial.begin(115200);
  // Banner
  Serial.printf("%s\n", "╔═╗┌┬┐┌─┐┬─┐┌┬┐  ╔╦╗┬┬─┐┬─┐┌─┐┬─┐");
  Serial.printf("%s\n", "╚═╗│││├─┤├┬┘ │   ║║║│├┬┘├┬┘│ │├┬┘");
  Serial.printf("%s\n", "╚═╝┴ ┴┴ ┴┴└─ ┴   ╩ ╩┴┴└─┴└─└─┘┴└─");
  Serial.println();
  Serial.printf("+------------+--------------------------+\n");
  Serial.printf("| Version    | %-24s |\n", FIRMWARE_VERSION);
  Serial.printf("| Model      | %s Rev. %-6d |\n", ESP.getChipModel(), ESP.getChipRevision());
  Serial.printf("| Cores      | %-24d |\n", ESP.getChipCores());
  Serial.printf("| Chip ID    | %-24d |\n", getChipId());
  Serial.printf("+------------+--------------------------+\n");
  Serial.println();

  // Initialize Smart Mirror led strip
  ledStrip = new LedStrip();
  ledStrip->init<WS2812B, 32, 30>(5, 2000);

  try {
    // Initialize Wi-Fi
    setupWifi(WIFI_SSID, WIFI_PASSWORD);
    // Initialize Web Server
    initWebServer(ledStrip);
  } catch (const char* ex) {
    Serial.println(ex);
  }
  Serial.println();
}

void loop() {
  loopTask();
  delay(100);
}