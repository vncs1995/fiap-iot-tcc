#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN        12
#define NUMPIXELS 7

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

void setup() {
  Serial.begin(115200);
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  pixels.begin();
}

void turnOnLed() {
  pixels.setPixelColor(3, pixels.Color(150, 0, 0));
  pixels.show();
}

void turnOffLed() {
   pixels.setPixelColor(3, pixels.Color(0, 0, 0));
   pixels.show();
}

void loop() {
    pixels.clear();
    for(int i=0; i<NUMPIXELS; i++) {
      turnOnLed();
      delay(DELAYVAL);
      turnOffLed();
      delay(DELAYVAL);
    }
}
