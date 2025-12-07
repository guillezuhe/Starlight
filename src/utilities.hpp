#include <FastLED.h>

// Helper: Convert X,Y to LED index (for serpentine wiring)
uint16_t XY(uint8_t x, uint8_t y) {
  if (y & 1) {  // Odd rows run backwards
    return (y * MATRIX_WIDTH) + (MATRIX_WIDTH - 1 - x);
  } else {
    return (y * MATRIX_WIDTH) + x;
  }
}