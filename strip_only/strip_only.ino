
// Simple strand test for Adafruit Dot Star RGB LED strip.
// This is a basic diagnostic tool, NOT a graphics demo...helps confirm
// correct wiring and tests each pixel's ability to display red, green
// and blue and to forward data down the line.  By limiting the number
// and color of LEDs, it's reasonably safe to power a couple meters off
// the Arduino's 5V pin.  DON'T try that with other code!

#include <Adafruit_DotStar.h>
// Because conditional #includes don't work w/Arduino sketches...
//#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET

 // Number of LEDs in strip
#define NUMPIXELS 72

// Here's how to control the LEDs from any two pins:
#define DATAPIN    0
#define CLOCKPIN   1
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
//Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_BRG);

// lower is faster
//20 milliseconds (~50 FPS)
#define SPEED_DELAY_MS 10

void setup() {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  strip.begin(); // Initialize pins for output

  strip.setBrightness(10); // 1/3 brightness

  strip.show();  // Turn all LEDs off ASAP
}

// Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.

#define TAIL_LENGTH 2
uint32_t color = strip.Color(255, 0, 0);      // 'On' color (starts red)

#define OFF_COLOR strip.Color(0, 0, 0)

#define INTIAL_STEP 1
int step = INTIAL_STEP;
int direction = 1;

// colors
int red = 255;
int green = 0;
int blue = 0;
#define COLOR_INCREMENT 1

void loop() {
  // refresh strip
  strip.show();
  
  // pause (controls step of steps)
  delay(SPEED_DELAY_MS);

  if (step == NUMPIXELS) {
    //step = NUMPIXELS;
    nextLine();
    direction = -1;
  } else if (step == 0)  {
    //step = 1;
    nextLine();
    direction = 1;
  }

  nextStep();

  step = step + direction; 
}



void nextStep() {
  // move leds over 1 position

  // turn head onto current color
  color = strip.Color(red, blue, green);
  strip.setPixelColor(step, color); // 'On' pixel at head

  if (direction > 0) {
      // turn off tail
      strip.setPixelColor(step-TAIL_LENGTH, OFF_COLOR);     // 'Off' pixel at tail
  }

  if (direction < 0) {
      strip.setPixelColor(step+TAIL_LENGTH, OFF_COLOR);     // 'Off' pixel at tail
  }

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

  // color change
  //if((color >>= 8) == 0)   {  //  Next color (R->G->B) ... past blue now?
  //    color = strip.Color(red, green, blue); // red
  //}
}


void nextLine() {
    // color change
    //if((color >>= 8) == 0)   {  //  Next color (R->G->B) ... past blue now?
    //    color = strip.Color(255, 0, 0); // red
    //}

}

// color changing algorithm
// http://codepen.io/Codepixl/pen/ogWWaK/
// try changing the colors one step (led) at a time


//
// if(r > 0 && b == 0){
//    r--;
//    g++;
//  }
//  if(g > 0 && r == 0){
//    g--;
//    b++;
//  }
//  if(b > 0 && g == 0){
//    r++;
//    b--;
//  }
 


