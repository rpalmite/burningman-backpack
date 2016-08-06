
// Low power NeoPixel goggles example.  Makes a nice blinky display
// with just a few LEDs on at any time.

#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
 #include <avr/power.h>
#endif

#define LEFT_EYE_PIN 0
#define RIGHT_EYE_PIN 1

Adafruit_NeoPixel neopixel1 = Adafruit_NeoPixel(32, LEFT_EYE_PIN); // left eyeball
Adafruit_NeoPixel neopixel2 = Adafruit_NeoPixel(32, RIGHT_EYE_PIN); // right eyeball

uint8_t  mode   = 0, // Current animation effect
         offset = 0; // Position of spinny eyes
uint32_t color  = 0xFF0000; // Start red
uint32_t prevTime;

void setup() {
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  neopixel1.begin();
  neopixel1.setBrightness(85); // 1/3 brightness

  neopixel2.begin();
  neopixel2.setBrightness(85); // 1/3 brightness
  
  prevTime = millis();
}

void loop() {
  uint8_t  i;
  uint32_t t;

  switch(1) {

   case 0: // Random sparks - just one LED on at a time!
    i = random(32);
    neopixel1.setPixelColor(i, color);
    neopixel1.show();
    delay(10);
    neopixel1.setPixelColor(i, 0);
    break;
 
   case 1: // Spinny wheels (8 LEDs on at a time)
    for(i=0; i<16; i++) {
      uint32_t c = 0;
      if(((offset + i) & 7) < 2) c = color; // 4 pixels on...
      neopixel1.setPixelColor(   i, c); // First eye
      neopixel1.setPixelColor(31-i, c); // Second eye (flipped)

      neopixel2.setPixelColor(   i, c); // First eye
      neopixel2.setPixelColor(31-i, c); // Second eye (flipped)
    }
    neopixel1.show();
    neopixel2.show();
    
    offset++;
    delay(50);
    break;
  }

  t = millis();
  if((t - prevTime) > 8000) {      // Every 8 seconds...
    mode++;                        // Next mode
    if(mode > 1) {                 // End of modes?
      mode = 0;                    // Start modes over
      color >>= 8;                 // Next color R->G->B
      if(!color) {
        color = 0xFF0000; // Reset to red
      }
    }
    for(i=0; i<32; i++) {
        neopixel1.setPixelColor(i, 0);
        neopixel2.setPixelColor(i, 0);
    }
    prevTime = t;
  }
}


408-784-5116
