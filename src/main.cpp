#include <Arduino.h>
#include "led_strip.hpp"
#include "utils.hpp"

#define FIRMWARE_VERSION "1.0.0"

#define WIFI_SSID "vodafone0128" // "REPLACE_WITH_YOUR_SSID"
#define WIFI_PASSWORD "TMLHDLDMXJZQDX" // "REPLACE_WITH_YOUR_PASSWORD"

LedStrip<WS2812B, 32, 30>* ledStrip;

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
    ledStrip = new LedStrip<WS2812B, 32, 30>(5, 2000);

    // Initialize Wi-Fi
    setupWifi(WIFI_SSID, WIFI_PASSWORD);
    Serial.println();
}

void loop() {
    ledStrip->on();
    delay(2000);
    ledStrip->off();
    delay(2000);
}