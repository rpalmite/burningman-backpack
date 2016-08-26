
// Simple strand test for Adafruit Dot Star RGB LED strip.
// This is a basic diagnostic tool, NOT a graphics demo...helps confirm
// correct wiring and tests each pixel's ability to display red, green
// and blue and to forward data down the line.  By limiting the number
// and color of LEDs, it's reasonably safe to power a couple meters off
// the Arduino's 5V pin.  DON'T try that with other code!

#include <Adafruit_NeoPixel.h>

#include <Adafruit_DotStar.h>
//#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET

#define BRIGHTNESS 30

// Number of LEDs in strip
#define NUMPIXELS 20
#define PIXEL_PER_NEOPIXEL_RING 16

// define gpio pins
#define LEFT_EYE_PIN 3
#define RIGHT_EYE_PIN 4
#define LIPS_DATAPIN    0
#define LIPS_CLOCKPIN   1

Adafruit_NeoPixel left_eye = Adafruit_NeoPixel(PIXEL_PER_NEOPIXEL_RING, LEFT_EYE_PIN);
Adafruit_NeoPixel right_eye = Adafruit_NeoPixel(PIXEL_PER_NEOPIXEL_RING, RIGHT_EYE_PIN);

Adafruit_DotStar lips = Adafruit_DotStar(NUMPIXELS, LIPS_DATAPIN, LIPS_CLOCKPIN, DOTSTAR_BRG);


// lower is faster
//20 milliseconds (~50 FPS)
#define SPEED_DELAY_MS 18

void setup() {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif
  // Initialize pins for output
  lips.begin();
  // set brightness (out of ?)
  lips.setBrightness(BRIGHTNESS);
  // turn leds off asap
  lips.show();

  left_eye.begin();
  left_eye.setBrightness(BRIGHTNESS);

  right_eye.begin();
  right_eye.setBrightness(BRIGHTNESS);
}

// Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.

#define TAIL_LENGTH 2
uint32_t color = lips.Color(255, 0, 0);      // 'On' color (starts red)

#define OFF_COLOR lips.Color(0, 0, 0)

#define INTIAL_STEP 1


// colors
int red = 255;
int green = 0;
int blue = 0;
#define COLOR_INCREMENT 1


// implement color change frequency (skip some iterations of movement)
#define COLOR_CHANGE_FREQUENCY 2

int step = INTIAL_STEP;
int direction = 1;


int eyeStep = 1;

void loop() {
  // refresh lights
  lips.show();
  left_eye.show();
  right_eye.show();
  
  // pause (controls step of steps)
  delay(SPEED_DELAY_MS);

  if (step == NUMPIXELS) {
    nextLine();
    direction = -1;
  } else if (step == 0)  {
    nextLine();
    direction = 1;
  }

  nextStepLips();
  
  step = step + direction;

  if (eyeStep == PIXEL_PER_NEOPIXEL_RING) {
    eyeStep = 0;
  } else {
    eyeStep++;
  }
  
  nextStepEyes();
  
}


// move leds over 1 position
void nextStepLips() {

  // turn on head of lips
  color = lips.Color(red, blue, green);
  lips.setPixelColor(step, color); // 'On' pixel at head

  // turn off the tail if lips
  if (direction > 0) {
      lips.setPixelColor(step-TAIL_LENGTH, OFF_COLOR);     // 'Off' pixel at tail
  }
  if (direction < 0) {
      lips.setPixelColor(step+TAIL_LENGTH, OFF_COLOR);     // 'Off' pixel at tail
  }
  
  // color change
  interateColor();

  
  //if((color >>= 8) == 0)   {  //  Next color (R->G->B) ... past blue now?
  //    color = strip.Color(red, green, blue); // red
  //}
}

void nextStepEyes() {
   int i = eyeStep % PIXEL_PER_NEOPIXEL_RING;

  // left eye
  //left_eye.setPixelColor(eyeStep, color);
  //left_eye.setPixelColor(PIXEL_PER_NEOPIXEL_RING-1-eyeStep, color);

  // trailing color black
  //if (eyeStep == 0) {
  //    left_eye.setPixelColor(PIXEL_PER_NEOPIXEL_RING-1, OFF_COLOR);
  //} else {
  //    left_eye.setPixelColor(eyeStep-TAIL_LENGTH, OFF_COLOR);
  //}
  //left_eye.setPixelColor(PIXEL_PER_NEOPIXEL_RING-1-eyeStep-1, OFF_COLOR);

   // right eye
  left_eye.setPixelColor(i, color);
  left_eye.setPixelColor(PIXEL_PER_NEOPIXEL_RING-1-i, color);
  //setEye(right_eye, i, color);
  left_eye.setBrightness(BRIGHTNESS+(step));

  
  // right eye
  right_eye.setPixelColor(i, color);
  right_eye.setPixelColor(PIXEL_PER_NEOPIXEL_RING-1-i, color);
  //setEye(right_eye, i, color);
  right_eye.setBrightness(BRIGHTNESS+(step));
  
}


void interateColor() {
  if (red > 0 && blue == 0) {
    red = red - COLOR_INCREMENT;
    green = green + COLOR_INCREMENT;
  }
  if (green > 0 && red == 0) {
    green = green - COLOR_INCREMENT;
    blue = blue + COLOR_INCREMENT;
  }
  if (blue > 0 && green == 0) {
    red = red + COLOR_INCREMENT;
    blue = blue - COLOR_INCREMENT;
  }
}


void nextLine() {
    // color change
    //if((color >>= 8) == 0)   {  //  Next color (R->G->B) ... past blue now?
    //    color = strip.Color(255, 0, 0); // red
    //}

}


 


