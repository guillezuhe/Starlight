#pragma once
#include <Preferences.h>

/**
 * State Manager - Handles persistent storage of lamp configuration
 * 
 * Strategy for minimizing flash wear:
 * 1. Only saves when values actually change
 * 2. Batches writes with a debounce timer (prevents multiple saves per adjustment)
 * 3. Uses Preferences library (handles wear leveling automatically)
 * 4. Restores state on power-up
 */

class StateManager {
private:
  Preferences preferences;
  
  // Current state
  int currentStarType;
  int brightness;
  int noiseStr;
  
  // Previous saved state (to detect changes)
  int savedStarType;
  int savedBrightness;
  int savedNoiseStr;
  
  // Debounce timer to batch writes
  uint32_t lastSaveTime;
  const uint32_t SAVE_DEBOUNCE_MS = 10000;  // Wait 10 seconds after last change before saving
  
  bool pendingSave;

  // Timer state
  bool timerOn;
  unsigned long lastTimerActivation;
  unsigned long sleepTimer;
  bool pendingDeepSleep;
  
  // Animation state
  bool animationOn;
  int animationType;

public:
  StateManager() 
    : currentStarType(0), brightness(10), noiseStr(0),
      savedStarType(-1), savedBrightness(-1), savedNoiseStr(-1),
      lastSaveTime(0), pendingSave(false), timerOn(false),
      lastTimerActivation(0), sleepTimer(60000), pendingDeepSleep(false),
      animationOn(false), animationType(0) {}
  
  /**
   * Initialize preferences storage and load saved state
   * Call this once in setup()
   */
  void begin() {
    // Open preferences namespace for StarLamp
    preferences.begin("StarLamp", false);  // false = read/write mode
    
    // Load saved values (with defaults)
    currentStarType = preferences.getInt("starType", 0);
    brightness = preferences.getInt("brightness", 10);
    noiseStr = preferences.getInt("noiseStr", 0);
    
    // Validate ranges
    if (currentStarType < 0 || currentStarType > 7) currentStarType = 0;
    if (brightness < 10 || brightness > 100) brightness = 10;
    if (noiseStr < 0 || noiseStr > 75) noiseStr = 0;

    // Initialize timer parameters
    timerOn = false;
    lastTimerActivation = 0;
    sleepTimer = 60000; // Default 1 minute
    pendingDeepSleep = false;

    // Animation mode
    animationOn = false;
    animationType = 0;
    
    // Initialize saved state
    savedStarType = currentStarType;
    savedBrightness = brightness;
    savedNoiseStr = noiseStr;
    /*
    Serial.print("[StateManager] Loaded state: starType=");
    Serial.print(currentStarType);
    Serial.print(" brightness=");
    Serial.print(brightness);
    Serial.print(" noiseStr=");
    Serial.println(noiseStr);
    */
  }
  
  /**
   * Call this in loop() to handle deferred saves
   * Saves to flash only when 2 seconds have passed since last change
   */
  void update() {
    if (pendingSave) {
      uint32_t timeSinceChange = millis() - lastSaveTime;
      if (timeSinceChange >= SAVE_DEBOUNCE_MS) {
        _performSave();
        pendingSave = false;
      }
    }

    // Check for timer-based sleep
    if (timerOn && !pendingDeepSleep) {
      if (millis() - lastTimerActivation >= sleepTimer) {
        // Time to go to sleep
        pendingDeepSleep = true;
        // Save state before sleeping
        saveImmediately();
        // Enter deep sleep (implementation depends on hardware)
        // For ESP32:
        //ESP.deepSleep(0);  // Sleep indefinitely //TODO: enable it in the main code turning off the leds and the button first. Also set a wake up source (button).
      }
    }
  }
  
  // Getters
  int getStarType() const { return currentStarType; }
  int getBrightness() const { return brightness; }
  int getNoiseStr() const { return noiseStr; }
  bool getTimerState() const { return timerOn; }
  int getSleepTimer() const { return sleepTimer; }
  int getLastTimerActivation() const { return lastTimerActivation; }
  bool getPendingDeepSleep() const { return pendingDeepSleep; }
  
  // Setters - Mark for save if changed
  void setStarType(int value) {
    if (value >= 0 && value <= 7 && currentStarType != value) {
      currentStarType = value;
      _markForSave();
    }
  }
  
  void setBrightness(int value) {
    if (value >= 10 && value <= 100 && brightness != value) {
      brightness = value;
      _markForSave();
    }
  }
  
  void setNoiseStr(int value) {
    if (value >= 0 && value <= 125 && noiseStr != value) {
      noiseStr = value;
      _markForSave();
    }
  }

  void setTimerState(bool state) {
    if (timerOn != state) {
      timerOn = state;
      //_markForSave();
    }
  }

  void setTimerSleep(unsigned long duration_ms) {
    if (sleepTimer != duration_ms) {
      sleepTimer = duration_ms;
      //_markForSave();
    }
  }
  
  void setPendingDeepSleep(bool state) {
    pendingDeepSleep = state;
  }

  void activateTimer(unsigned long duration_ms) {
    timerOn = true;
    sleepTimer = duration_ms;
    lastTimerActivation = millis();
  }

  void deactivateTimer() {
    timerOn = false;
    pendingDeepSleep = false;
  }
  
  /**
   * Force immediate save (useful for graceful shutdown)
   */
  void saveImmediately() {
    if (pendingSave || _hasChanges()) {
      _performSave();
      pendingSave = false;
    }
  }

  void clearPreferences() {
    preferences.clear();  // Clears all keys in the namespace
  }
  
private:
  bool _hasChanges() const {
    return (currentStarType != savedStarType) ||
           (brightness != savedBrightness) ||
           (noiseStr != savedNoiseStr);
  }
  
  void _markForSave() {
    lastSaveTime = millis();
    pendingSave = true;
    //pendingSave = false; //TODO: disable deferred save for testing
  }
  
  void _performSave() {
    if (!_hasChanges()) return;  // No changes to save
    
    // Write to preferences
    preferences.putInt("starType", currentStarType);
    preferences.putInt("brightness", brightness);
    preferences.putInt("noiseStr", noiseStr);
    
    // Update saved state
    savedStarType = currentStarType;
    savedBrightness = brightness;
    savedNoiseStr = noiseStr;
    /*
    Serial.print("[StateManager] Saved state: starType=");
    Serial.print(currentStarType);
    Serial.print(" brightness=");
    Serial.print(brightness);
    Serial.print(" noiseStr=");
    Serial.println(noiseStr);
    */
  }
};
