#pragma once

#ifdef CONSOLE_USE_SERIAL1
  #define CONSOLE_SERIAL Serial1
  // Serial1 shares D6/D7 with I2C (display), so disable display
  #ifdef DISPLAY_CLASS
    #undef DISPLAY_CLASS
  #endif
#else
  #define CONSOLE_SERIAL Serial
#endif
