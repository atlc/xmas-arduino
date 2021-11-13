#include <FastLED.h>

#define buttonPin 4
#define neoPin 7
#define NUM_LEDS 64

# define BLAZER_GREEN CRGB(0,30,0)
# define BLAZER_GOLD CRGB(255,209,0)
CRGB leds[NUM_LEDS];

CRGBPalette16 blendable_palette (BLAZER_GOLD, BLAZER_GREEN, BLAZER_GOLD);

int buttonState = 0;
int mode = 0;

const char *modes[] = { "off", "gold", "goldtrail", "green", "greentrail", "greenandgold", "greenandgoldblend", "rgb01", "rgb02", "rgb03" };

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);
  FastLED.addLeds<NEOPIXEL, neoPin>(leds, NUM_LEDS);
  Serial.begin(9600);
}

void checkButtonPress() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    const int arrayLength = sizeof(modes) / sizeof(modes[0]);

    if (mode + 1 == arrayLength) {
      mode = 0;
    } else {
      mode++;
    }
    delay(200); // Debouncing for button presses
  }
}

void loop() {
  checkButtonPress();

  String setting = modes[mode];
  Serial.println(setting);
  delay(50);

  if (setting == "off") {
    FastLED.clear();
    FastLED.show();
  } else if (setting == "gold") {
    FastLED.setBrightness(5);
    fill_solid(leds, NUM_LEDS, BLAZER_GOLD);
    FastLED.show();
  } else if (setting == "goldtrail") {
    colorTrail(BLAZER_GOLD);
  } else if (setting == "green") {
    FastLED.setBrightness(250);
    fill_solid(leds, NUM_LEDS, BLAZER_GREEN);
    FastLED.show();
  } else if (setting == "greentrail") {
    FastLED.setBrightness(40);
    colorTrail(BLAZER_GREEN);
  } else if (setting == "greenandgold") {
    for (int i = 0; i < NUM_LEDS; i++) {
      for (int j = 0; j < NUM_LEDS; j++) {
        if (i + j <= NUM_LEDS) {
          leds[i + j] = BLAZER_GREEN;
        } else {
          leds[(i + j) - NUM_LEDS] = BLAZER_GOLD;
        }
      }
      FastLED.show();

      checkButtonPress();
      delay(50);
    }
  } else if (setting == "greenandgoldblend") {
    for (int i = 200; i > 0; i--) {
      int j = 180;
      if (j >= 30) {
        j--;
      }
      fill_solid(leds, NUM_LEDS, CRGB(i, j, 0));
      FastLED.show();
      delay(45);
    }

    delay(4500);

    for (int i = 0; i < 200; i++) {
      int j;
      if (i >= j && i <= 200) {
        j = i;
      } else {
        j = 200;
      }
      fill_solid(leds, NUM_LEDS, CRGB(i, j, 0));
      FastLED.show();
      delay(45);
    }

    delay(4500);

  } else if (setting == "rgb01") {
    fill_rainbow(leds, NUM_LEDS, 0, 255 / NUM_LEDS);
    FastLED.show();
  } else if (setting == "rgb02") {
    fill_rainbow(leds, NUM_LEDS, 0, 255 / NUM_LEDS * 2);
    FastLED.show();
  } else if (setting == "rgb03") {
    uint8_t waveCycle = beat8(10, 255);
    fill_rainbow(leds, NUM_LEDS, waveCycle, 10);
    FastLED.setBrightness(40);
    FastLED.show();
  }

}

void colorTrail (CRGB color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    for (int j = 0; j < NUM_LEDS / 2; j++) {
      if (i + j <= NUM_LEDS) {
        leds[i + j] = color;
      } else {
        leds[(i + j) - NUM_LEDS] = color;
      }
    }
    FastLED.show();
    leds[i] = CRGB::Black;

    checkButtonPress();
    delay(50);
  }
}
