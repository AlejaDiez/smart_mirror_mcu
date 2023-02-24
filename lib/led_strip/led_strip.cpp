#include "led_strip.hpp"

void changeBrightnessWithAnimation(int brightness) {
    FastLED.setBrightness(brightness);
    FastLED.show();
    delay(ANIMATION_DELAY);
}

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

CRGB fadeTowardColor(CRGB& current, const CRGB& target, uint8_t amount) {
    nblendU8TowardU8(current.red, target.red, amount);
    nblendU8TowardU8(current.green, target.green, amount);
    nblendU8TowardU8(current.blue, target.blue, amount);
    return current;
}