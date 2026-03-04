#pragma once

#ifdef CONSOLE_USE_SERIAL1
  #define CONSOLE_SERIAL Serial1
#else
  #define CONSOLE_SERIAL Serial
#endif
