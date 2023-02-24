#ifndef __LED_STRIP
#define __LED_STRIP

#include "FastLED.h"

#define ANIMATION_DELAY 2

/// Helper function that changes brightness with animation
///
/// @param brightness
void changeBrightnessWithAnimation(int);
/// Helper function that combines one uint8_t with another by a given amount
///
/// @param current current color
/// @param target target color
/// @param amount amount of change
void nblendU8TowardU8(uint8_t&, const uint8_t, uint8_t);
/// Helper function that gets the mixed color
///
/// @param current current color
/// @param target target color
/// @param amount amount of change
/// @return mixed color
CRGB fadeTowardColor(CRGB&, const CRGB&, uint8_t);

template<template<uint8_t DATA_PIN, EOrder RGB_ORDER> class CHIPSET, int PIN, int SIZE>
class LedStrip {
    private:
        /// An array of CRGB objects representing the individual LEDs in the strip
        CRGBArray<SIZE> leds;
        /// A boolean indicating whether the strip is currently on or off
        bool state = false;
        /// An integer indicating the current brightness of the LEDs, with a default value of 255
        int brightness = 255;
        /// An integer indicating the current color of the LEDs in hexadecimal format, with a default value of white
        int color = 0xFFFFFF;

    public:
        /// Constructor that initializes a new object of type LedStrip
        LedStrip() {
            // Initialize led strip
            FastLED.addLeds<CHIPSET, PIN, GRB>(leds, SIZE).setCorrection(TypicalLEDStrip).setTemperature(CarbonArc);
            // Power off all the leds
            FastLED.clear();
            FastLED.show();
        }

        /// Constructor that initializes a new object of type LedStrip with a specified maximum power
        ///
        /// @param volts maximum voltage
        /// @param milliamps maximum current
        LedStrip(int volts, int milliamps) {
            // Set max power
            FastLED.setMaxPowerInVoltsAndMilliamps(volts, milliamps);
            // Initialize led strip
            FastLED.addLeds<CHIPSET, PIN, GRB>(leds, SIZE).setCorrection(TypicalLEDStrip).setTemperature(CarbonArc);
            // Power off all the leds
            FastLED.clear();
            FastLED.show();
        }

        /// Returns the current state of the LED strip
        ///
        /// @return true if on, false if off
        inline bool getState() {
            return state;
        }

        /// Turns on the LED strip
        void on() {
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

        /// Turns off the LED strip
        void off() {
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

        /// Returns the current brightness level of the LED strip
        ///
        /// @return current brightness between 0.0 and 1.0
        inline float getBrightness() {
            float brightness = (float)this->brightness / 255;
    
            return ceil(brightness * 100) / 100;
        }

        /// Sets the brightness level of the LED strip (with an optional boolean argument indicating whether to animate the LEDs)
        ///
        /// @param brightness between 0.0 and 1.0
        /// @param animation true
        void setBrightness(float brightness, bool animation) {
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

        /// Returns the current color of the LED strip in hexadecimal format
        ///
        /// @return current hex color
        inline int getColor() {
            return color;
        }

        /// Sets the color of the LED strip (with an optional boolean argument indicating whether to animate the LEDs)
        ///
        /// @param color hex color
        /// @param animation true
        void setColor(int color, bool animation = true) {
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
                        fill_solid(leds, SIZE, currentColor);
                        FastLED.show();
                        delay(ANIMATION_DELAY);
                    }
                } else {
                    fill_solid(leds, SIZE, targetColor);
                    FastLED.show();
                }
            }
        }
};

#endif