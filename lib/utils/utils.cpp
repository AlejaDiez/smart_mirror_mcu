#include "utils.hpp"
#include <WiFi.h>

#define WIFI_CONNECTION_TIMEOUT 16000

bool setupWifi(const char* ssid, const char* password) {
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
        return true;
    } else {
        Serial.printf("%s %s\n", "Failed to connect to", ssid);
        return false;
    }
}