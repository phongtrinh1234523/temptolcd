#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#define DISPLAY_ROWS 2U
#define DISPLAY_COLUMNS 16U

typedef struct {
  uint8_t int_Hum;
  uint8_t float_Hum;
  uint8_t int_temp;
  uint8_t float_temp;
} DisplayReading;

void Display_FormatRows(const DisplayReading *reading, uint8_t reading_ok,
                        char rows[DISPLAY_ROWS][DISPLAY_COLUMNS + 1U]);

#endif
