#include <FastLED.h>

#define NUM_LEDS 83      // 8x8 matrix
#define DATA_PIN 5       // GPIO5
#define BRIGHTNESS 10
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8

int r1 = 8;
int r2 = 16;
int r3 = 24;
int r4 = 35;

int pinButtonP = 7;  // GPIO7
int pinButtonM = 6;  // GPIO6
CRGB leds[NUM_LEDS];
uint8_t hue = 0;

// Star type tracking (0=O hottest to 6=M coldest)
int currentStarType = 3;  // Start at type F (index 3)
uint32_t lastButtonPress = 0;
const uint32_t DEBOUNCE_TIME = 200;  // milliseconds

int noise_str = 0;

/*******************
 * DIFFERENT STARS AS A FUNCTION OF COLOR TEMPERATURE
 *******************/

void type_O_star() {
  // Blueish star
  // fill_solid(leds, NUM_LEDS, CRGB::Blue);

  CRGB basecolor = CRGB::Blue;

  // Add random noise
  for (int i = 0; i < NUM_LEDS; i++) {
    // Generate random offset (0 to noise_str*2), then subtract noise_str
    int8_t offset_r = random8(noise_str * 2) - noise_str;
    int8_t offset_g = random8(noise_str * 2) - noise_str;
    int8_t offset_b = random8(noise_str * 2) - noise_str;
    
    // Use qadd8/qsub8 for saturating math (clamps to 0-255)
    if (offset_r >= 0) leds[i].r = qadd8(basecolor.r, offset_r);
    else leds[i].r = qsub8(basecolor.r, -offset_r);
    
    if (offset_g >= 0) leds[i].g = qadd8(basecolor.g, offset_g);
    else leds[i].g = qsub8(basecolor.g, -offset_g);
    
    if (offset_b >= 0) leds[i].b = qadd8(basecolor.b, offset_b);
    else leds[i].b = qsub8(basecolor.b, -offset_b);
  }
}

void type_B_star() {
  // Blue-white star
  //fill_solid(leds, NUM_LEDS, CRGB::Cyan);
  CRGB basecolor = CRGB::Cyan;

  // Add random noise
  for (int i = 0; i < NUM_LEDS; i++) {
    // Generate random offset (0 to noise_str*2), then subtract noise_str
    int8_t offset_r = random8(noise_str * 2) - noise_str;
    int8_t offset_g = random8(noise_str * 2) - noise_str;
    int8_t offset_b = random8(noise_str * 2) - noise_str;
    
    // Use qadd8/qsub8 for saturating math (clamps to 0-255)
    if (offset_r >= 0) leds[i].r = qadd8(basecolor.r, offset_r);
    else leds[i].r = qsub8(basecolor.r, -offset_r);
    
    if (offset_g >= 0) leds[i].g = qadd8(basecolor.g, offset_g);
    else leds[i].g = qsub8(basecolor.g, -offset_g);
    
    if (offset_b >= 0) leds[i].b = qadd8(basecolor.b, offset_b);
    else leds[i].b = qsub8(basecolor.b, -offset_b);
  }
}

void type_A_star() {
  // White star
  //fill_solid(leds, NUM_LEDS, CRGB::White);
  CRGB basecolor = CRGB::White;

  // Add random noise
  for (int i = 0; i < NUM_LEDS; i++) {
    // Generate random offset (0 to noise_str*2), then subtract noise_str
    int8_t offset_r = random8(noise_str * 2) - noise_str;
    int8_t offset_g = random8(noise_str * 2) - noise_str;
    int8_t offset_b = random8(noise_str * 2) - noise_str;
    
    // Use qadd8/qsub8 for saturating math (clamps to 0-255)
    if (offset_r >= 0) leds[i].r = qadd8(basecolor.r, offset_r);
    else leds[i].r = qsub8(basecolor.r, -offset_r);
    
    if (offset_g >= 0) leds[i].g = qadd8(basecolor.g, offset_g);
    else leds[i].g = qsub8(basecolor.g, -offset_g);
    
    if (offset_b >= 0) leds[i].b = qadd8(basecolor.b, offset_b);
    else leds[i].b = qsub8(basecolor.b, -offset_b);
  }
}

void type_F_star() {
  // Yellow-white star
  //fill_solid(leds, NUM_LEDS, CRGB(255, 255, 60)); // RGB (255, 255, 200);

  CRGB basecolor = CRGB(255, 255, 60);

  // Add random noise
  for (int i = 0; i < NUM_LEDS; i++) {
    // Generate random offset (0 to noise_str*2), then subtract noise_str
    int8_t offset_r = random8(noise_str * 2) - noise_str;
    int8_t offset_g = random8(noise_str * 2) - noise_str;
    int8_t offset_b = random8(noise_str * 2) - noise_str;
    
    // Use qadd8/qsub8 for saturating math (clamps to 0-255)
    if (offset_r >= 0) leds[i].r = qadd8(basecolor.r, offset_r);
    else leds[i].r = qsub8(basecolor.r, -offset_r);
    
    if (offset_g >= 0) leds[i].g = qadd8(basecolor.g, offset_g);
    else leds[i].g = qsub8(basecolor.g, -offset_g);
    
    if (offset_b >= 0) leds[i].b = qadd8(basecolor.b, offset_b);
    else leds[i].b = qsub8(basecolor.b, -offset_b);
  }
}

void type_G_star() {
  // Yellow star
  //fill_solid(leds, NUM_LEDS, CRGB::Yellow);

  CRGB basecolor = CRGB::Yellow;

  // Add random noise
  for (int i = 0; i < NUM_LEDS; i++) {
    // Generate random offset (0 to noise_str*2), then subtract noise_str
    int8_t offset_r = random8(noise_str * 2) - noise_str;
    int8_t offset_g = random8(noise_str * 2) - noise_str;
    int8_t offset_b = random8(noise_str * 2) - noise_str;
    
    // Use qadd8/qsub8 for saturating math (clamps to 0-255)
    if (offset_r >= 0) leds[i].r = qadd8(basecolor.r, offset_r);
    else leds[i].r = qsub8(basecolor.r, -offset_r);
    
    if (offset_g >= 0) leds[i].g = qadd8(basecolor.g, offset_g);
    else leds[i].g = qsub8(basecolor.g, -offset_g);
    
    if (offset_b >= 0) leds[i].b = qadd8(basecolor.b, offset_b);
    else leds[i].b = qsub8(basecolor.b, -offset_b);
  }
}

void type_K_star() {
  // Orange star
  //fill_solid(leds, NUM_LEDS, CRGB::DarkOrange); // Orange
  CRGB basecolor = CRGB::DarkOrange;

  // Add random noise
  for (int i = 0; i < NUM_LEDS; i++) {
    // Generate random offset (0 to noise_str*2), then subtract noise_str
    int8_t offset_r = random8(noise_str * 2) - noise_str;
    int8_t offset_g = random8(noise_str * 2) - noise_str;
    int8_t offset_b = random8(noise_str * 2) - noise_str;
    
    // Use qadd8/qsub8 for saturating math (clamps to 0-255)
    if (offset_r >= 0) leds[i].r = qadd8(basecolor.r, offset_r);
    else leds[i].r = qsub8(basecolor.r, -offset_r);
    
    if (offset_g >= 0) leds[i].g = qadd8(basecolor.g, offset_g);
    else leds[i].g = qsub8(basecolor.g, -offset_g);
    
    if (offset_b >= 0) leds[i].b = qadd8(basecolor.b, offset_b);
    else leds[i].b = qsub8(basecolor.b, -offset_b);
  }

}

void type_M_star() {
  // Red star
  //fill_solid(leds, NUM_LEDS, CRGB::OrangeRed);

  CRGB basecolor = CRGB::OrangeRed;

  // Add random noise
  for (int i = 0; i < NUM_LEDS; i++) {
    // Generate random offset (0 to noise_str*2), then subtract noise_str
    int8_t offset_r = random8(noise_str * 2) - noise_str;
    int8_t offset_g = random8(noise_str * 2) - noise_str;
    int8_t offset_b = random8(noise_str * 2) - noise_str;
    
    // Use qadd8/qsub8 for saturating math (clamps to 0-255)
    if (offset_r >= 0) leds[i].r = qadd8(basecolor.r, offset_r);
    else leds[i].r = qsub8(basecolor.r, -offset_r);
    
    if (offset_g >= 0) leds[i].g = qadd8(basecolor.g, offset_g);
    else leds[i].g = qsub8(basecolor.g, -offset_g);
    
    if (offset_b >= 0) leds[i].b = qadd8(basecolor.b, offset_b);
    else leds[i].b = qsub8(basecolor.b, -offset_b);
  }
}

// Helper: Convert X,Y to LED index (for serpentine wiring)
uint16_t XY(uint8_t x, uint8_t y) {
  if (y & 1) {  // Odd rows run backwards
    return (y * MATRIX_WIDTH) + (MATRIX_WIDTH - 1 - x);
  } else {
    return (y * MATRIX_WIDTH) + x;
  }
}

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
  }
}

// Check and handle button presses
void handleButtonPresses() {
  uint32_t currentTime = millis();
  
  // Check button + (hotter stars - decrease index)
  if (digitalRead(pinButtonP) == LOW) {
    Serial.println("Button P pressed");
    if (currentTime - lastButtonPress > DEBOUNCE_TIME) {
      if (currentStarType > 0) {
        currentStarType--;
      }
      lastButtonPress = currentTime;
    }
  }
  
  // Check button - (colder stars - increase index)
  if (digitalRead(pinButtonM) == LOW) {
    Serial.println("Button M pressed");
    if (currentTime - lastButtonPress > DEBOUNCE_TIME) {
      if (currentStarType < 6) {
        currentStarType++;
      }
      lastButtonPress = currentTime;
    }
  }
}

// Animation 1: Rainbow wave
void rainbowWave() {
  fill_rainbow(leds, NUM_LEDS, hue, 7);
  hue++;
}

// Animation 2: Color palette cycling
void paletteCycle() {
  static uint8_t startIndex = 0;
  startIndex++;
  fill_palette(leds, NUM_LEDS, startIndex, 4, RainbowColors_p, 255, LINEARBLEND);
}

// Animation 3: Plasma effect using noise
void plasma() {
  static uint16_t time = 0;
  time++;
  for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
    for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
      uint8_t noise = inoise8(x * 50, y * 50, time);
      leds[XY(x, y)] = CHSV(noise, 255, 255);
    }
  }
}

// Animation 4: Confetti - random colored speckles
void confetti() {
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(hue + random8(64), 200, 255);
  hue++;
}

// Animation 5: Cylon/Knight Rider scanner
void cylon() {
  static uint8_t pos = 0;
  static int8_t dir = 1;
  
  fadeToBlackBy(leds, NUM_LEDS, 20);
  leds[pos] = CHSV(hue, 255, 255);
  
  pos += dir;
  if (pos >= NUM_LEDS - 1 || pos <= 0) dir = -dir;
  hue++;
}

// Animation 6: Breathing effect
void breathing() {
  uint8_t brightness = beatsin8(15, 20, 255);  // 15 BPM, range 20-255
  fill_solid(leds, NUM_LEDS, CHSV(hue, 255, brightness));
  EVERY_N_MILLISECONDS(50) { hue++; }
}

// Animation 7: Fire effect
void fire() {
  static byte heat[NUM_LEDS];
  
  // Cool down
  for (int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8(heat[i], random8(0, 30));
  }
  
  // Heat rises
  for (int i = NUM_LEDS - 1; i >= 2; i--) {
    heat[i] = (heat[i - 1] + heat[i - 2]) / 2;
  }
  
  // Random sparks at bottom
  if (random8() < 120) {
    heat[random8(8)] = qadd8(heat[random8(8)], random8(160, 255));
  }
  
  // Map heat to colors
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = HeatColor(heat[i]);
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(pinButtonP, INPUT_PULLUP);
  pinMode(pinButtonM, INPUT_PULLUP);
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