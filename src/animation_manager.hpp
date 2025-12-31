class AnimationController {
private:
  bool isPlaying;
  uint8_t currentPathIndex;  // Which mass category
  uint8_t currentStageIndex;
  unsigned long stageStartTime;
  float stageProgress;  // 0.0 to 1.0
  
  // Interpolation state (reuse existing prev/next_state vectors)
  
public:
  void selectMass(float mass);
  void start();
  void pause();
  void stop();
  void update();  // Call in loop()
  void render();  // Render current frame
  
  // Query functions
  const char* getCurrentStageName();
  float getProgress();
  bool isComplete();
};

struct AnimationKeyframe {
  const char* stageName;
  CRGB baseColor;
  uint8_t noiseStrength;
  uint8_t brightness;
  unsigned long durationMs;
  uint8_t effectType;  // 0=normal, 1=pulse, 2=explosion, 3=fade
};

struct EvolutionPath {
  float stellarMass;  // in solar masses
  uint8_t numStages;
  const AnimationKeyframe* stages;
};


// Example evolution paths (simplified)
const AnimationKeyframe path_1Ms_stages[] = {
  {"Protostar", CRGB::Blue, 20, 50, 5000, 0},
  {"Main Sequence", CRGB::White, 10, 100, 20000, 0},
  {"Red Giant", CRGB::Red, 30, 80, 10000, 1},
  {"White Dwarf", CRGB::White, 5, 60, 8000, 3}
};

const EvolutionPath evolutionPaths[] = {
  {1.0, 4, path_1Ms_stages},
  // Add more paths for different masses
};