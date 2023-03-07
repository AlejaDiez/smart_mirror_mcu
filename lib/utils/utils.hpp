#ifndef __UTILS
#define __UTILS

#include <Arduino.h>
#include "led_strip.hpp"

/// Helper function that configures the WiFi and connects to a network
///
/// @param ssid WiFi ssid
/// @param password WiFi password 
/// @return true if connected, false if not connected
void setupWifi(const char*, const char*);
/// Helper function that configures the WiFi and connects to a network
///
/// @param ledStrip 
/// @param port server port
/// @return true if connected, false if not connected
void initWebServer(LedStrip*, int port = 80);
///
void loopTask();

/// Helper function that decodes the serial number of the board
///
/// @return serial number
inline int getChipId() {
  uint32_t chipId = 0;

  for (int i = 0; i < 17; i += 8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  return chipId;
}

#endif