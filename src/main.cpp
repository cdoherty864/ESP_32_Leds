#include <FastLED.h>

//#define LED_PIN     2      // GPIO connected to the strip's data line
//#define NUM_LEDS    300     // change to your strip's LED count
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB    // most WS2812 strips use GRB; try RGB if colors look wrong
#define BRIGHTNESS  10    // 0-255

CRGB leds[NUM_LEDS];

// Fire tuning parameters — play with these to change the flame character
#define COOLING     55     // higher = shorter flames (more cooling). Try 20-100.
#define SPARKING    120    // higher = more sparks/flicker. Try 50-200.

// stores "heat" value for each LED
byte heat[NUM_LEDS];

void fireEffect() {
    // Step 1: cool down every cell a little
    for (int i = 0; i < NUM_LEDS; i++) {
        heat[i] = qsub8(heat[i], random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }

    // Step 2: heat drifts upward, with a bit of diffusion
    for (int k = NUM_LEDS - 1; k >= 2; k--) {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }

    // Step 3: randomly ignite new sparks near the bottom
    if (random8() < SPARKING) {
        int y = random8(7);
        heat[y] = qadd8(heat[y], random8(160, 255));
    }

    // Step 4: map heat values to LED colors (black -> red -> yellow -> white)
    for (int j = 0; j < NUM_LEDS; j++) {
        CRGB color = HeatColor(heat[j]);
        leds[j] = color;
    }
}

void setup() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    random16_set_seed(analogRead(0));
}

void loop() {
    fireEffect();
    FastLED.show();
    delay(15);
}

