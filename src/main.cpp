#include <FastLED.h>

#define NUM_LEDS 83      // 8x8 matrix
#define DATA_PIN 5       // GPIO5
#define BRIGHTNESS 10
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8

// Include files
#include "utilities.hpp"
#include "patterns.hpp"

int r1 = 8;
int r2 = 16;
int r3 = 24;
int r4 = 35;

int pinButtonP = 7;  // GPIO7

// Star type tracking (0=O hottest to 6=M coldest)
int currentStarType = 0;  // Start at type F (index 3)
uint32_t lastButtonPress = 0;
int prevButtonStateP = HIGH;
const uint32_t DEBOUNCE_TIME = 200;  // milliseconds

/*******************
 * DIFFERENT STARS AS A FUNCTION OF COLOR TEMPERATURE
 *******************/



// Display current star type
void displayCurrentStar() {
  switch (currentStarType) {
    case 0: type_O_star(); break;
    case 1: type_B_star(); break;
    case 2: type_A_star(); break;
    case 3: type_F_star(); break;
    case 4: type_G_star(); break;
    case 5: type_K_star(); break;
    case 6: type_M_star(); break;
    case 7: off_start(); break;
  }
}

void handleButtonPresses() {
  // Short press (< 1s) changes the color temperature
  // Long press (> 1s) increases/decreases noise strength
  static uint32_t buttonPHoldStart = 0;
  static uint32_t buttonPHoldStart_long = 0;
  uint32_t currentTime = millis();

  // Handle button + (colder stars - decrease index / decrease noise)
  int buttonPState = digitalRead(pinButtonP);
  if (buttonPState == LOW && prevButtonStateP == HIGH) {
    buttonPHoldStart = currentTime;
    buttonPHoldStart_long = currentTime; 
  } else if (buttonPState == HIGH && prevButtonStateP == LOW) {
    uint32_t holdDuration = currentTime - buttonPHoldStart;
    if (holdDuration < 1000) {
      // Short press: decrease star type
      currentStarType++;
      if (currentStarType > 7) currentStarType = 0;  // Wrap around
    }
    buttonPHoldStart = 0;
  } else if (buttonPState == LOW && prevButtonStateP == LOW) {
    uint32_t holdDuration = currentTime - buttonPHoldStart_long;
    
    if (holdDuration >= 2000) {
      noise_str = noise_str + 25;
      if (noise_str > 75) noise_str = 0;  // Cap noise strength
      buttonPHoldStart_long = currentTime;  // Reset to avoid multiple decreases
    }
  }
  prevButtonStateP = buttonPState;
}

void handleButtonLed() {
  // Simple LED to indicate button press
  static bool ledState = false;
  static uint32_t lastToggleTime = 0;
  uint32_t currentTime = millis();

  int buttonPState = digitalRead(pinButtonP);
  if (buttonPState == LOW) {
    if (!ledState && (currentTime - lastToggleTime > 100)) {
      ledState = true;
      lastToggleTime = currentTime;
      // Turn on LED (for example, set a pin HIGH)
      digitalWrite(LED_BUILTIN, HIGH);
    }
  } else {
    if (ledState && (currentTime - lastToggleTime > 100)) {
      ledState = false;
      lastToggleTime = currentTime;
      // Turn off LED (for example, set a pin LOW)
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(pinButtonP, INPUT_PULLUP);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
  delay(5000);
}

void loop() {
  // Blink the simple LED and the matrix to indicate the program is running
  
  //rainbowWave();
  // paletteCycle();
  // plasma();
  // confetti();
  // cylon();
  // breathing();
  // fire();
  
  // Handle button presses to change star type
  handleButtonPresses();
  
  // Display current star type
  displayCurrentStar();
  FastLED.show();
  FastLED.delay(100);  // 50 fps
}