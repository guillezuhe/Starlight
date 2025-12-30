#include <FastLED.h>
#include <vector>

CRGB leds[NUM_LEDS];
uint8_t hue = 0;
int iter = 0;
int niter = 50;
int niter_def = 50;

// Store the value of the previous state of the leds
std::vector<CRGB> prev_state(NUM_LEDS, CRGB::Black);
std::vector<CRGB> next_state(NUM_LEDS, CRGB::Black);


void type_O_star() {
  // Blueish star
  // fill_solid(leds, NUM_LEDS, CRGB::Blue);

  iter++;
  if (iter >= niter) {
    CRGB basecolor = CRGB::Blue;
    int noise_str = stateManager.getNoiseStr();
    niter = int(niter_def - noise_str / 4); // Adjust niter based on noise_str
    iter = 0;
    // Add random noise
    for (int i = 0; i < NUM_LEDS; i++) {
      prev_state[i] = next_state[i];
      // Generate random offset (0 to noise_str*2), then subtract noise_str
      int8_t offset_r = random8(noise_str * 2) - noise_str;
      int8_t offset_g = random8(noise_str * 2) - noise_str;
      int8_t offset_b = random8(noise_str * 2) - noise_str;
      
      // Use qadd8/qsub8 for saturating math (clamps to 0-255)
      if (offset_r >= 0) next_state[i].r = qadd8(basecolor.r, offset_r);
      else next_state[i].r = qsub8(basecolor.r, -offset_r);
      
      if (offset_g >= 0) next_state[i].g = qadd8(basecolor.g, offset_g);
      else next_state[i].g = qsub8(basecolor.g, -offset_g);
      
      if (offset_b >= 0) next_state[i].b = qadd8(basecolor.b, offset_b);
      else next_state[i].b = qsub8(basecolor.b, -offset_b);
    }
  }
  // Define the interpolation
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].r = lerp8by8(prev_state[i].r, next_state[i].r, (iter * 256) / niter);
    leds[i].g = lerp8by8(prev_state[i].g, next_state[i].g, (iter * 256) / niter);
    leds[i].b = lerp8by8(prev_state[i].b, next_state[i].b, (iter * 256) / niter);
  }
}

void type_B_star() {
  // Blue-white star
  //fill_solid(leds, NUM_LEDS, CRGB::Cyan);
  iter++;
  if (iter >= niter) {
    CRGB basecolor = CRGB::Cyan;
    int noise_str = stateManager.getNoiseStr();
    niter = int(niter_def - noise_str / 4); // Adjust niter based on noise_str
    iter = 0;
    // Add random noise
    for (int i = 0; i < NUM_LEDS; i++) {
      prev_state[i] = next_state[i];
      // Generate random offset (0 to noise_str*2), then subtract noise_str
      int8_t offset_r = random8(noise_str * 2) - noise_str;
      int8_t offset_g = random8(noise_str * 2) - noise_str;
      int8_t offset_b = random8(noise_str * 2) - noise_str;
      
      // Use qadd8/qsub8 for saturating math (clamps to 0-255)
      if (offset_r >= 0) next_state[i].r = qadd8(basecolor.r, offset_r);
      else next_state[i].r = qsub8(basecolor.r, -offset_r);
      
      if (offset_g >= 0) next_state[i].g = qadd8(basecolor.g, offset_g);
      else next_state[i].g = qsub8(basecolor.g, -offset_g);
      
      if (offset_b >= 0) next_state[i].b = qadd8(basecolor.b, offset_b);
      else next_state[i].b = qsub8(basecolor.b, -offset_b);
    }
  }
  // Define the interpolation
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].r = lerp8by8(prev_state[i].r, next_state[i].r, (iter * 256) / niter);
    leds[i].g = lerp8by8(prev_state[i].g, next_state[i].g, (iter * 256) / niter);
    leds[i].b = lerp8by8(prev_state[i].b, next_state[i].b, (iter * 256) / niter);
  }
}

void type_A_star() {
  // White star
  //fill_solid(leds, NUM_LEDS, CRGB::White);
  iter++;
  if (iter >= niter) {
    CRGB basecolor = CRGB::White;
    int noise_str = stateManager.getNoiseStr();
    niter = int(niter_def - noise_str / 4); // Adjust niter based on noise_str
    iter = 0;
    // Add random noise
    for (int i = 0; i < NUM_LEDS; i++) {
      prev_state[i] = next_state[i];
      // Generate random offset (0 to noise_str*2), then subtract noise_str
      int8_t offset_r = random8(noise_str * 2) - noise_str;
      int8_t offset_g = random8(noise_str * 2) - noise_str;
      int8_t offset_b = random8(noise_str * 2) - noise_str;
      
      // Use qadd8/qsub8 for saturating math (clamps to 0-255)
      if (offset_r >= 0) next_state[i].r = qadd8(basecolor.r, offset_r);
      else next_state[i].r = qsub8(basecolor.r, -offset_r);
      
      if (offset_g >= 0) next_state[i].g = qadd8(basecolor.g, offset_g);
      else next_state[i].g = qsub8(basecolor.g, -offset_g);
      
      if (offset_b >= 0) next_state[i].b = qadd8(basecolor.b, offset_b);
      else next_state[i].b = qsub8(basecolor.b, -offset_b);
    }
  }
  // Define the interpolation
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].r = lerp8by8(prev_state[i].r, next_state[i].r, (iter * 256) / niter);
    leds[i].g = lerp8by8(prev_state[i].g, next_state[i].g, (iter * 256) / niter);
    leds[i].b = lerp8by8(prev_state[i].b, next_state[i].b, (iter * 256) / niter);
  }
}

void type_F_star() {
  // Yellow-white star
  //fill_solid(leds, NUM_LEDS, CRGB(255, 255, 60)); // RGB (255, 255, 200);
  iter++;
  if (iter >= niter) {
    CRGB basecolor = CRGB(255, 255, 50);
    int noise_str = stateManager.getNoiseStr();
    niter = int(niter_def - noise_str / 4); // Adjust niter based on noise_str
    iter = 0;
    // Add random noise
    for (int i = 0; i < NUM_LEDS; i++) {
      prev_state[i] = next_state[i];
      // Generate random offset (0 to noise_str*2), then subtract noise_str
      int8_t offset_r = random8(noise_str * 2) - noise_str;
      int8_t offset_g = random8(noise_str * 2) - noise_str;
      int8_t offset_b = random8(noise_str * 2) - noise_str;
      
      // Use qadd8/qsub8 for saturating math (clamps to 0-255)
      if (offset_r >= 0) next_state[i].r = qadd8(basecolor.r, offset_r);
      else next_state[i].r = qsub8(basecolor.r, -offset_r);
      
      if (offset_g >= 0) next_state[i].g = qadd8(basecolor.g, offset_g);
      else next_state[i].g = qsub8(basecolor.g, -offset_g);
      
      if (offset_b >= 0) next_state[i].b = qadd8(basecolor.b, offset_b);
      else next_state[i].b = qsub8(basecolor.b, -offset_b);
    }
  }
  // Define the interpolation
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].r = lerp8by8(prev_state[i].r, next_state[i].r, (iter * 256) / niter);
    leds[i].g = lerp8by8(prev_state[i].g, next_state[i].g, (iter * 256) / niter);
    leds[i].b = lerp8by8(prev_state[i].b, next_state[i].b, (iter * 256) / niter);
  }
}

void type_G_star() {
  // Yellow star
  //fill_solid(leds, NUM_LEDS, CRGB::Yellow);

  iter++;
  if (iter >= niter) {
    CRGB basecolor = CRGB::Yellow;
    int noise_str = stateManager.getNoiseStr();
    niter = int(niter_def - noise_str / 4); // Adjust niter based on noise_str
    iter = 0;
    // Add random noise
    for (int i = 0; i < NUM_LEDS; i++) {
      prev_state[i] = next_state[i];
      // Generate random offset (0 to noise_str*2), then subtract noise_str
      int8_t offset_r = random8(noise_str * 2) - noise_str;
      int8_t offset_g = random8(noise_str * 2) - noise_str;
      int8_t offset_b = random8(noise_str * 2) - noise_str;
      
      // Use qadd8/qsub8 for saturating math (clamps to 0-255)
      if (offset_r >= 0) next_state[i].r = qadd8(basecolor.r, offset_r);
      else next_state[i].r = qsub8(basecolor.r, -offset_r);
      
      if (offset_g >= 0) next_state[i].g = qadd8(basecolor.g, offset_g);
      else next_state[i].g = qsub8(basecolor.g, -offset_g);
      
      if (offset_b >= 0) next_state[i].b = qadd8(basecolor.b, offset_b);
      else next_state[i].b = qsub8(basecolor.b, -offset_b);
    }
  }
  // Define the interpolation
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].r = lerp8by8(prev_state[i].r, next_state[i].r, (iter * 256) / niter);
    leds[i].g = lerp8by8(prev_state[i].g, next_state[i].g, (iter * 256) / niter);
    leds[i].b = lerp8by8(prev_state[i].b, next_state[i].b, (iter * 256) / niter);
  }
}

void type_K_star() {
  // Orange star
  //fill_solid(leds, NUM_LEDS, CRGB::DarkOrange); // Orange
  iter++;
  if (iter >= niter) {
    CRGB basecolor = CRGB::DarkOrange;
    int noise_str = stateManager.getNoiseStr();
    niter = int(niter_def - noise_str / 4); // Adjust niter based on noise_str
    iter = 0;
    // Add random noise
    for (int i = 0; i < NUM_LEDS; i++) {
      prev_state[i] = next_state[i];
      // Generate random offset (0 to noise_str*2), then subtract noise_str
      int8_t offset_r = random8(noise_str * 2) - noise_str;
      int8_t offset_g = random8(noise_str * 2) - noise_str;
      int8_t offset_b = random8(noise_str * 2) - noise_str;
      
      // Use qadd8/qsub8 for saturating math (clamps to 0-255)
      if (offset_r >= 0) next_state[i].r = qadd8(basecolor.r, offset_r);
      else next_state[i].r = qsub8(basecolor.r, -offset_r);
      
      if (offset_g >= 0) next_state[i].g = qadd8(basecolor.g, offset_g);
      else next_state[i].g = qsub8(basecolor.g, -offset_g);
      
      if (offset_b >= 0) next_state[i].b = qadd8(basecolor.b, offset_b);
      else next_state[i].b = qsub8(basecolor.b, -offset_b);
    }
  }
  // Define the interpolation
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].r = lerp8by8(prev_state[i].r, next_state[i].r, (iter * 256) / niter);
    leds[i].g = lerp8by8(prev_state[i].g, next_state[i].g, (iter * 256) / niter);
    leds[i].b = lerp8by8(prev_state[i].b, next_state[i].b, (iter * 256) / niter);
  }
}

void type_M_star() {
  // Red star
  //fill_solid(leds, NUM_LEDS, CRGB::OrangeRed);
  iter++;
  if (iter >= niter) {
    CRGB basecolor = CRGB(255, 40, 0);
    int noise_str = stateManager.getNoiseStr();
    niter = int(niter_def - noise_str / 4);  // Adjust niter based on noise_str
    iter = 0;
    // Add random noise
    for (int i = 0; i < NUM_LEDS; i++) {
      prev_state[i] = next_state[i];
      // Generate random offset (0 to noise_str*2), then subtract noise_str
      int8_t offset_r = random8(noise_str * 2) - noise_str;
      int8_t offset_g = random8(noise_str * 2) - noise_str;
      int8_t offset_b = random8(noise_str * 2) - noise_str;
      
      // Use qadd8/qsub8 for saturating math (clamps to 0-255)
      if (offset_r >= 0) next_state[i].r = qadd8(basecolor.r, offset_r);
      else next_state[i].r = qsub8(basecolor.r, -offset_r);
      
      if (offset_g >= 0) next_state[i].g = qadd8(basecolor.g, offset_g);
      else next_state[i].g = qsub8(basecolor.g, -offset_g);
      
      if (offset_b >= 0) next_state[i].b = qadd8(basecolor.b, offset_b);
      else next_state[i].b = qsub8(basecolor.b, -offset_b);
    }
  }
  // Define the interpolation
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].r = lerp8by8(prev_state[i].r, next_state[i].r, (iter * 256) / niter);
    leds[i].g = lerp8by8(prev_state[i].g, next_state[i].g, (iter * 256) / niter);
    leds[i].b = lerp8by8(prev_state[i].b, next_state[i].b, (iter * 256) / niter);
  }
}

void off_star() {
  // Red star
  //fill_solid(leds, NUM_LEDS, CRGB::OrangeRed);
  iter++;
  if (iter >= niter_def) {
    CRGB basecolor = CRGB(0, 0, 0);
    iter = 0;
    // Add random noise
    for (int i = 0; i < NUM_LEDS; i++) {
      prev_state[i] = next_state[i];
    }
  }
  // Define the interpolation
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].r = lerp8by8(prev_state[i].r, next_state[i].r, (iter * 256) / niter_def);
    leds[i].g = lerp8by8(prev_state[i].g, next_state[i].g, (iter * 256) / niter_def);
    leds[i].b = lerp8by8(prev_state[i].b, next_state[i].b, (iter * 256) / niter_def);
  }
}


void off_star_deprecated() {  // TODO: Remove after testing
  fill_solid(leds, NUM_LEDS, CRGB::Black);
}



/************************
 * LED ANIMATION PATTERNS
 ************************/



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