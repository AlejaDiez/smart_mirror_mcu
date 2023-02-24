#ifndef __LED_STRIP
#define __LED_STRIP

#include "FastLED.h"

#define LED_TYPE WS2812B // Led type
#define DATA_PIN 32      // Data pin in esp32
#define NUM_LEDS 30      // Number of leds in the strip

class LedStrip {
    private:
        /// An array of CRGB objects representing the individual LEDs in the strip
        CRGBArray<NUM_LEDS> leds;
        /// A boolean indicating whether the strip is currently on or off
        bool state = false;
        /// An integer indicating the current brightness of the LEDs, with a default value of 255
        int brightness = 255;
        /// An integer indicating the current color of the LEDs in hexadecimal format, with a default value of white
        int color = 0xFFFFFF;

    public:
        /// Constructor that initializes a new object of type LedStrip
        LedStrip();
        /// Constructor that initializes a new object of type LedStrip with a specified maximum power
        ///
        /// @param volts maximum voltage
        /// @param milliamps maximum current
        LedStrip(int, int);
        /// Returns the current state of the LED strip
        ///
        /// @return true if on, false if off
        bool getState();
        /// Turns on the LED strip
        void on();
        /// Turns off the LED strip
        void off();
        /// Returns the current brightness level of the LED strip
        ///
        /// @return current brightness between 0.0 and 1.0
        float getBrightness();
        /// Sets the brightness level of the LED strip (with an optional boolean argument indicating whether to animate the LEDs)
        ///
        /// @param brightness between 0.0 and 1.0
        /// @param animation true
        void setBrightness(float, bool = true);
        /// Returns the current color of the LED strip in hexadecimal format
        ///
        /// @return current hex color
        int getColor();
        /// Sets the color of the LED strip (with an optional boolean argument indicating whether to animate the LEDs)
        ///
        /// @param color hex color
        /// @param animation true
        void setColor(int, bool = true);
};

#endif