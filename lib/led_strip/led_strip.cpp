#include "led_strip.hpp"

/// Helper function that changes brightness with animation
///
/// @param brightness
void changeBrightnessWithAnimation(int brightness) {
  FastLED.setBrightness(brightness);
  FastLED.show();
  delay(ANIMATION_DELAY);
}

/// Helper function that combines one uint8_t with another by a given amount
///
/// @param current current color
/// @param target target color
/// @param amount amount of change
void nblendU8TowardU8(uint8_t& current, const uint8_t target, uint8_t amount) {
  uint8_t delta = 0;

  if (current == target) {
    return;
  }
  if (current < target) {
    delta = target - current;
    delta = scale8_video(delta, amount);
    current += delta;
  } else {
    delta = current - target;
    delta = scale8_video(delta, amount);
    current -= delta;
  }
}

/// Helper function that gets the mixed color
///
/// @param current current color
/// @param target target color
/// @param amount amount of change
/// @return mixed color
CRGB fadeTowardColor(CRGB& current, const CRGB& target, uint8_t amount) {
  nblendU8TowardU8(current.red, target.red, amount);
  nblendU8TowardU8(current.green, target.green, amount);
  nblendU8TowardU8(current.blue, target.blue, amount);
  return current;
}

bool LedStrip::getState() {
  // Check if the led strip is initialized
  if (leds == NULL) {
    throw "The led strip isn't initialized";
  }

  return state;
}

void LedStrip::on() {
  // Check if the led strip is initialized
  if (leds == NULL) {
    throw "The led strip isn't initialized";
  }

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
  // Check if the led strip is initialized
  if (leds == NULL) {
    throw "The led strip isn't initialized";
  }

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
  // Check if the led strip is initialized
  if (leds == NULL) {
    throw "The led strip isn't initialized";
  }

  float brightness = (float)this->brightness / 255;

  return ceil(brightness * 100) / 100;
}

void LedStrip::setBrightness(float brightness, bool animation) {
  // Check if the led strip is initialized
  if (leds == NULL) {
    throw "The led strip isn't initialized";
  }

  // Check if it is possible to change the brightness
  if (brightness < 0.0 || brightness > 1.0) {
    throw "The brightness should be between 0.0 y 1.0";
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
  // Check if the led strip is initialized
  if (leds == NULL) {
    throw "The led strip isn't initialized";
  }

  return color;
}

void LedStrip::setColor(int color, bool animation) {
  // Check if the led strip is initialized
  if (leds == NULL) {
    throw "The led strip isn't initialized";
  }

  // Check if it is possible to change the color
  if (color < 0x000000 || color > 0xFFFFFF) {
    throw "The color should be between #000000 y #FFFFFF";
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
        fill_solid(leds, FastLED.size(), currentColor);
        FastLED.show();
        delay(ANIMATION_DELAY);
      }
    } else {
      fill_solid(leds, FastLED.size(), targetColor);
      FastLED.show();
    }
  }
}