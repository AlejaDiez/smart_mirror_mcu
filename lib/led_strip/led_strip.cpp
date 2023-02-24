#include <Arduino.h>
#include "led_strip.h"

#define ANIMATION_DELAY 2

// Helper function that changes brightness with animation
void changeBrightnessWithAnimation(int brightness);
// Helper function that combines one uint8_t with another by a given amount
void nblendU8TowardU8(uint8_t& cur, const uint8_t target, uint8_t amount);
// Helper function that gets the mixed color
CRGB fadeTowardColor(CRGB& cur, const CRGB& target, uint8_t amount);

LedStrip::LedStrip() {
    // Initialize led strip
    FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip).setTemperature(CarbonArc);
    // Power off all the leds
    FastLED.clear();
    FastLED.show();
}

LedStrip::LedStrip(int volts, int milliamps) {
    // Set max power
    FastLED.setMaxPowerInVoltsAndMilliamps(volts, milliamps);
    // Initialize led strip
    FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip).setTemperature(CarbonArc);
    // Power off all the leds
    FastLED.clear();
    FastLED.show();
}

bool LedStrip::getState() {
    return state;
}

void LedStrip::on() {
    // Check if it is possible to turn on the mirror
    if (state) {
        throw "The mirror is already on";
    }

    // Turn on the mirror
    state = true;
    fill_solid(leds, FastLED.size(), CRGB(color));
    for (int i = 0; i <= brightness; i++) {
        changeBrightnessWithAnimation(i);
    }
}

void LedStrip::off() {
    // Check if it is possible to turn off the mirror
    if (!state) {
        throw "The mirror is already off";
    }

    // Turn off the mirror
    state = false;
    for (int i = FastLED.getBrightness(); i >= 0; i--) {
        changeBrightnessWithAnimation(i);
    }
    FastLED.clear();
    FastLED.show();
}

float LedStrip::getBrightness() {
    float brightness = (float)this->brightness / 255;
    
    return ceil(brightness * 100) / 100;
}

void LedStrip::setBrightness(float brightness, bool animation) {
    // Check if it is possible to change the brightness
    if (brightness < 0.0 || brightness > 1.0) {
        throw "El valor debe estar entre 0.0 y 1.0";
    }

    // Change brightness
    this->brightness = brightness * 255;
    if (state) {
        if (animation) {
            if (FastLED.getBrightness() > this->brightness) {
                for (int i = FastLED.getBrightness(); i >= this->brightness; i--) {
                    changeBrightnessWithAnimation(i);
                }
            } else if (FastLED.getBrightness() < this->brightness) {
                for (int i = FastLED.getBrightness(); i <= this->brightness; i++) {
                    changeBrightnessWithAnimation(i);
                }
            }
        } else {
            FastLED.setBrightness(this->brightness);
            FastLED.show();
        }
    }
}

int LedStrip::getColor() {
    return color;
}

void LedStrip::setColor(int color, bool animation) {
    // Check if it is possible to change the color
    if (color < 0x000000 || color > 0xFFFFFF) {
        throw "El color debe estar entre #000000 y #FFFFFF";
    }

    CRGB currentColor(this->color);
    CRGB targetColor(color);

    // Change color
    this->color = color;
    if (state) {
        if (animation) {
            while (currentColor != targetColor) {
                currentColor = fadeTowardColor(currentColor, targetColor, 1);
                currentColor = CRGB(currentColor.r, currentColor.g, currentColor.b);
                fill_solid(leds, NUM_LEDS, currentColor);
                FastLED.show();
                delay(ANIMATION_DELAY);
            }
        } else {
            fill_solid(leds, NUM_LEDS, targetColor);
            FastLED.show();
        }
    }
}

void changeBrightnessWithAnimation(int brightness) {
    FastLED.setBrightness(brightness);
    FastLED.show();
    delay(ANIMATION_DELAY);
}

void nblendU8TowardU8(uint8_t& cur, const uint8_t target, uint8_t amount) {
    uint8_t delta = 0;

    if (cur == target) {
        return;
    }
    if (cur < target) {
        delta = target - cur;
        delta = scale8_video(delta, amount);
        cur += delta;
    } else {
        delta = cur - target;
        delta = scale8_video(delta, amount);
        cur -= delta;
    }
}

CRGB fadeTowardColor(CRGB& cur, const CRGB& target, uint8_t amount) {
    nblendU8TowardU8(cur.red, target.red, amount);
    nblendU8TowardU8(cur.green, target.green, amount);
    nblendU8TowardU8(cur.blue, target.blue, amount);
    return cur;
}