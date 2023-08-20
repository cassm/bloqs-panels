#include <Adafruit_NeoPixel.h>
//#define LED_LAYOUT_SMALL_CIRCLE 1
//#define LED_LAYOUT_LARGE_CIRCLE 1
//#define LED_LAYOUT_TINY_HEART 1
#define LED_LAYOUT_SMALL_HEART 1
//#define LED_LAYOUT_MEDIUM_HEART 1
//#define LED_LAYOUT_LARGE_HEART 1


#define LOVE 1
//#define HATE 1
//#define EYES 1

#if EYES
#include "palette_eyes.h"
#elif LOVE
#include "palette_lovefish.h"
#elif HATE
#include "palette_hatefish.h"
#endif

#include "led_coords.h"

#include <math.h>
#include <stdint.h>

#define PIN 3

#define BRIGHTNESS 255

float palettes_visible_at_a_time = 0.25;
float palettes_per_r = float(palette_len) / float(max_r) * palettes_visible_at_a_time;

uint64_t ms_per_palette = float(7500) / (float(721) / float(max_r));
//uint64_t ms_per_palette = 7500;
float palettes_per_ms = float(palette_len-1) / float(ms_per_palette);

//uint8_t px_buff[num_leds][3] = {{0,0,0}};
float lpf = 0.95;

float phi_offset_factor = (palettes_visible_at_a_time * float(palette_len)) / float(10);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(num_leds, PIN, NEO_GRB + NEO_KHZ800);

uint8_t gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

uint8_t Red(uint32_t color)
{
    return (color >> 16) & 0xFF;
}

// Returns the Green component of a 32-bit color
uint8_t Green(uint32_t color)
{
    return (color >> 8) & 0xFF;
}

// Returns the Blue component of a 32-bit color
uint8_t Blue(uint32_t color)
{
    return color & 0xFF;
}

void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.clear();
  strip.show(); // Initialize all pixels to 'off'
}


void loop() {
    float time_offset = float(millis() % ms_per_palette) * palettes_per_ms;
    float phi_interval = 6.28/4;
    
    for (int i = 0; i < num_leds; i++) {
        
        #if LOVE
        int palette_idx = int(float(led_r_vals[i]) * palettes_per_r - time_offset);
        #else
        int palette_idx = int(float(led_r_vals[i]) * palettes_per_r + time_offset);
        #endif
        while (palette_idx < 0) {
            palette_idx = palette_idx + palette_len;
        }
        palette_idx = palette_idx % palette_len;
        uint8_t result[3];
        uint32_t px_col = strip.getPixelColor(i);
        
        /*
        result[0] = float(Red(px_col)) * lpf + float(palette[palette_idx][0]) * (1.0-lpf);
        result[1] = float(Green(px_col)) * lpf + float(palette[palette_idx][1]) * (1.0-lpf);
        result[2] = float(Blue(px_col)) * lpf + float(palette[palette_idx][2]) * (1.0-lpf);
        */
        
        result[0] = palette[palette_idx][0];
        result[1] = palette[palette_idx][1];
        result[2] = palette[palette_idx][2];
        /*
        for (int ch = 0; ch < 3; ch++) {
            result[ch] = px_buff[i][ch] * lpf + palette[palette_idx][ch] * (1.0-lpf);
        }*/
        strip.setPixelColor(i, strip.Color(gamma[result[0]], gamma[result[1]], gamma[result[2]])); 
    }
    strip.show();

}
