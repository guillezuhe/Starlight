struct AnimationKeyframe {
  const char* stageName;
  CRGB baseColor;
  uint8_t noiseStrength;
  uint8_t brightness;
  unsigned long durationMs;
  unsigned long transitionMs;  // Time to transition to next stage
  uint8_t effectType;  // 0=normal, 1=pulse partial, 2=pulse total, 3=explosion, 4=fade
  // Explosion proceeds in 1 second, so adjust duration accordingly
};

struct EvolutionPath {
  float stellarMass;  // in solar masses
  uint8_t numStages;
  const AnimationKeyframe* stages;
};

const AnimationKeyframe path_50Ms_stages[] = {
  {"Pre-Main Sequence", CRGB::Amethyst, 25, 30, 5000, 4000, 1},
  {"Main Sequence", CRGB::Cyan, 50, 70, 20000, 5000, 0},
  {"SuperGiant", CRGB::Blue, 50, 80, 10000, 0, 0},
  {"Supernova", CRGB::Blue, 100, 80, 700, 700, 3},
  {"Supernova-2", CRGB::Violet, 5, 30, 10, 10, 4},
  {"Black", CRGB::Black, 0, 0, 3000, 0, 0}
};

const AnimationKeyframe path_10Ms_stages[] = {
  {"Pre-Main Sequence", CRGB::Amethyst, 25, 30, 5000, 4000, 1},
  {"Main Sequence", CRGB::White, 50, 70, 25000, 5000, 0},
  {"Giant Star", CRGB::DarkOrange, 15, 70, 5000, 5000, 0},
  {"SuperGiant", CRGB::Orange, 50, 80, 10000, 0, 0},
  {"Supernova", CRGB::Orange, 100, 80, 700, 700, 3},
  {"Neutron Star", CRGB::White, 5, 30, 15000, 5000, 2},
  {"Black", CRGB::Black, 0, 0, 3000, 0, 0}
};

const AnimationKeyframe path_1Ms_stages[] = {
  {"Pre-Main Sequence", CRGB::Amethyst, 25, 30, 5000, 4000, 1},
  {"Main Sequence", CRGB::Yellow, 50, 70, 30000, 5000, 0},
  {"Giant Star", CRGB::DarkOrange, 15, 70, 5000, 5000, 0},
  {"SuperGiant", CRGB::Red, 5, 80, 5000, 0, 0},
  {"Helium Flash", CRGB::Red, 10, 80, 700, 0, 3},
  {"SuperGiant", CRGB::Red, 5, 80, 5000, 5000, 4},
  {"Planetary Nebula", CRGB::Cyan, 15, 30, 5000, 5000, 1},
  {"White Dwarf", CRGB::White, 5, 30, 8000, 5000, 4},
  {"Black", CRGB::Black, 0, 0, 3000, 0, 0}
};

const AnimationKeyframe path_04Ms_stages[] = {
  {"Pre-Main Sequence", CRGB::Amethyst, 25, 40, 5000, 4000, 1},
  {"Red Dwarf", CRGB::Red, 40, 70, 60000, 5000, 0},
  {"Blue Dwarf", CRGB::Cyan, 40, 50, 5000, 5000, 0},
  {"White Dwarf", CRGB::White, 20, 40, 8000, 5000, 4},
  {"Black", CRGB::Black, 0, 0, 3000, 0, 0}
};

const EvolutionPath evolutionPaths[] = {
  {1, 6, path_50Ms_stages},
  {1, 7, path_10Ms_stages},
  {1, 9, path_1Ms_stages},
  {0.4, 5, path_04Ms_stages}
};

const uint8_t NUM_PATHS = 4;


/***************************
 ***************************
 * 
 * ANIMATION CONTROLLER CLASS
 * 
 ***************************
 ***************************/

class AnimationController {
private:
  bool isPlaying;
  uint8_t currentPathIndex;  // Which mass category
  uint8_t currentStageIndex;
  unsigned long stageStartTime;
  float stageProgress;  // 0.0 to 1.0
  const int n_iters_noise = 50;
  int iter_noise = 0;
  std::array<std::array<int, 3>, NUM_LEDS> next_noise_offsets;
  std::array<std::array<int, 3>, NUM_LEDS> prev_noise_offsets;
  
  // Private helper methods to access keyframes
  AnimationKeyframe getCurrentKeyframe() {
    if (currentPathIndex >= NUM_PATHS) {
      // Return safe default (black/off state)
      return AnimationKeyframe{"Error", CRGB::Black, 0, 0, 1000, 0, 0};
    }
    
    const EvolutionPath* path = &evolutionPaths[currentPathIndex];
    
    if (currentStageIndex >= path->numStages) {
      // Return safe default
      return AnimationKeyframe{"Complete", CRGB::Black, 0, 0, 1000, 0, 0};
    }
    
    // Direct access - stored in RAM, so simple array indexing
    return path->stages[currentStageIndex];
  }
  
  AnimationKeyframe getNextKeyframe() {
    if (currentPathIndex >= NUM_PATHS) {
      return AnimationKeyframe{"Error", CRGB::Black, 0, 0, 1000, 0, 0};
    }
    
    const EvolutionPath* path = &evolutionPaths[currentPathIndex];
    uint8_t nextIndex = currentStageIndex + 1;
    
    // If at last stage, return current stage (or black for fade out)
    if (nextIndex >= path->numStages) {
      // Return black keyframe for final fade
      return AnimationKeyframe{"End", CRGB::Black, 0, 0, 1000, 3, 0};
    }
    
    return path->stages[nextIndex];
  }
  
  const EvolutionPath* getCurrentPath() {
    if (currentPathIndex >= NUM_PATHS) {
      return nullptr;
    }
    return &evolutionPaths[currentPathIndex];
  }
  
public:
  AnimationController() 
    : isPlaying(false), currentPathIndex(0), currentStageIndex(0),
      stageStartTime(0), stageProgress(0.0), next_noise_offsets{}, 
      prev_noise_offsets{} {}

  void begin() {
    isPlaying = false;
    currentPathIndex = 0;
    currentStageIndex = 0;
    stageStartTime = 0;
    stageProgress = 0.0;
    // Initialize noise offsets
    for (int i=0; i<NUM_LEDS; i++) {
      next_noise_offsets[i] = {0, 0, 0};
      prev_noise_offsets[i] = {0, 0, 0};
    }
  }
  
  // Select path by index
  void selectPathByIndex(uint8_t index) {
    if (index < NUM_PATHS) {
      currentPathIndex = index;
      currentStageIndex = 0;
      stageProgress = 0.0;
    }
  }
  
  void start() {
    isPlaying = true;
    currentStageIndex = 0;
    stageStartTime = millis();
    stageProgress = 0.0;
  }
  
  void pause() {
    isPlaying = false;
  }
  
  void stop() {
    // Restore brightness to default
    FastLED.setBrightness(10);
    currentStageIndex = 0;
    stageProgress = 0.0;
    isPlaying = false;
  }
  
  void update() { // Call in loop()
    if (!isPlaying) return;
    
    const EvolutionPath* path = getCurrentPath();
    if (!path) return;
    
    // Calculate how much time has passed
    unsigned long elapsed = millis() - stageStartTime;
    AnimationKeyframe current = getCurrentKeyframe();
    
    // Calculate progress (0.0 to 1.0)
    stageProgress = (float)elapsed / (float)current.durationMs;
    
    // Check if stage is complete
    if (stageProgress >= 1.0) {
      currentStageIndex++;
      stageStartTime = millis();
      stageProgress = 0.0;
      
      // Check if animation is complete
      if (currentStageIndex >= path->numStages) {
        /*
        isPlaying = false;
        currentStageIndex = 0;
        stageProgress = 0.0;
        // Restore brightness to default
        FastLED.setBrightness(stateManager.getBrightness());
        */
        stop();
      }
    }
  }
  
  void render() {
    if (!isPlaying) return;

    AnimationKeyframe current = getCurrentKeyframe();
    AnimationKeyframe next = getNextKeyframe();

    // Check if in transition phase
    unsigned long elapsed = millis() - stageStartTime;
    unsigned long transitionStart = current.durationMs - current.transitionMs;
    CRGB interpColor = current.baseColor;

    if (elapsed >= transitionStart && current.transitionMs > 0) {
      // Calculate transition progress
      float transitionProgress = float(elapsed - transitionStart) / float(current.transitionMs);
      if (transitionProgress > 1.0) transitionProgress = 1.0;
      
      // Interpolate brightness
      uint8_t interpBrightness = lerp8by8(current.brightness, next.brightness, uint8_t(transitionProgress * 255));
      FastLED.setBrightness(interpBrightness);
      
      // Interpolate base color
      interpColor = blend(current.baseColor, next.baseColor, uint8_t(transitionProgress * 255));
      
    } else {
      // Normal rendering at current stage settings
      FastLED.setBrightness(current.brightness);
      interpColor = current.baseColor;
      // Set LED colors based on current.baseColor and noiseStrength
      // (Implementation of noise effect omitted for brevity)
    }
    
    iter_noise++;
    if (iter_noise >= n_iters_noise) {
      iter_noise = 0;
      for (int i=0; i<NUM_LEDS; i++) {
        prev_noise_offsets[i][0] = next_noise_offsets[i][0];
        prev_noise_offsets[i][1] = next_noise_offsets[i][1];
        prev_noise_offsets[i][2] = next_noise_offsets[i][2];
        
        // Generate random offset (0 to noiseStrength*2), then subtract noiseStrength
        int8_t offset_r = random8(current.noiseStrength * 2) - current.noiseStrength;
        int8_t offset_g = random8(current.noiseStrength * 2) - current.noiseStrength;
        int8_t offset_b = random8(current.noiseStrength * 2) - current.noiseStrength;
        next_noise_offsets[i][0] = offset_r;
        next_noise_offsets[i][1] = offset_g;
        next_noise_offsets[i][2] = offset_b;
      }
    }

    // Apply noise offsets with interpolation
    if (current.noiseStrength == 0) {
      // No noise, set all to calculated color
      fill_solid(leds, NUM_LEDS, interpColor);
    } else {
      for (int i=0; i<NUM_LEDS; i++) {
        // Linear interpolation for signed offsets
        uint8_t blend_factor = (iter_noise * 256) / n_iters_noise;
        int r_offset = ((prev_noise_offsets[i][0] * (256 - blend_factor)) + (next_noise_offsets[i][0] * blend_factor)) >> 8;
        int g_offset = ((prev_noise_offsets[i][1] * (256 - blend_factor)) + (next_noise_offsets[i][1] * blend_factor)) >> 8;
        int b_offset = ((prev_noise_offsets[i][2] * (256 - blend_factor)) + (next_noise_offsets[i][2] * blend_factor)) >> 8;
        int r = constrain(int(interpColor.r) + r_offset, 0, 255);
        int g = constrain(int(interpColor.g) + g_offset, 0, 255);
        int b = constrain(int(interpColor.b) + b_offset, 0, 255);
        leds[i] = CRGB(r, g, b);
      }
    }

    // Add the effects if any
    switch (current.effectType) {
      case 0: // Normal
        // Do nothing extra. The final color is already set above.
        break;
      case 1: // Pulse partial
        displayPulsePartial();
        break;
      case 2: // Pulse total
        displayPulseTotal();
        break;
      case 3: // Explosion
        displayExplosion();
        break;
      case 4: // Fade
        displayFade();
        break;
    }
    return;
  }

  /**********
  * Effects
  ***********/
  void displayPulsePartial() {
    // Implement pulse effect with partial brightness modulation. Not full on/off.
    // Predefined frequency of 1 Hz for pulse effect
    const float w = 6.28; // 1 Hz
    int8_t pulse = sin(w * millis() / 1000.0) * FastLED.getBrightness() / 3; // +/- 1/3 of brightness
    // Take into account that int8_t can store from -128 to 127. 
    // There is no overflow problem because I am dividing by 3.

    FastLED.setBrightness(constrain(FastLED.getBrightness() + pulse, 0, 255));
  }

  void displayPulseTotal() {
    // Implement pulse effect with full brightness modulation (on/off)
    // Predefined frequency of 1 Hz for pulse effect
    const float w = 6.28; // 1 Hz
    uint8_t pulse = sin(w * millis() / 1000.0) * 50 + 50; // 0-brightness

    FastLED.setBrightness(pulse);
  }

  void displayExplosion() {
    // Implement explosion effect 
    // Quick bright flash in the first 1 s and then keep this value
    // The transition to the next color and brightness is handled by
    // the animation control.
    unsigned long elapsed = millis() - stageStartTime;
    if (elapsed <= 500) {
      float progress = float(elapsed) / 500.0;  // Fixed: progress from 0 to 1
      uint8_t brightness = lerp8by8(0, 200, uint8_t(progress * 255));
      FastLED.setBrightness(brightness);
    } else if (elapsed < 10000) { // Limit to 10 s for safety reasons
      FastLED.setBrightness(200);
    } else {
      FastLED.setBrightness(0);
    }
  }

  void displayFade() {
    FastLED.setBrightness(int(FastLED.getBrightness()*(1-stageProgress)));
  }
  
  /*****************
  * Query functions
  ******************/
  const char* getCurrentStageName() {
    AnimationKeyframe frame = getCurrentKeyframe();
    return frame.stageName;
  }
  
  float getProgress() const {
    return stageProgress;
  }
  /*
  bool isComplete() const {
    if (!getCurrentPath()) return true;
    return !isPlaying && currentStageIndex >= getCurrentPath()->numStages;
  }
  */
  bool getIsPlaying() const {
    return isPlaying;
  }
  
  uint8_t getCurrentStageIndex() const {
    return currentStageIndex;
  }
};
