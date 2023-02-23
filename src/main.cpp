#include <Arduino.h>
#include "utils.h"

#define FIRMWARE_VERSION "0.0.0"

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
}

void loop() { }