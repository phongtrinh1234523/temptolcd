#include "display.h"

#include <stdio.h>
#include <string.h>

void Display_FormatRows(const DisplayReading *reading, uint8_t reading_ok,
                        char rows[DISPLAY_ROWS][DISPLAY_COLUMNS + 1U]) {
  memset(rows, ' ', DISPLAY_ROWS * (DISPLAY_COLUMNS + 1U));
  rows[0][DISPLAY_COLUMNS] = '\0';
  rows[1][DISPLAY_COLUMNS] = '\0';

  memcpy(rows[0], "TEMP: ", 6U);
  memcpy(rows[1], "HUM: ", 5U);

  if (reading_ok && reading != NULL) {
    (void)snprintf(&rows[0][6], DISPLAY_COLUMNS - 6U + 1U, " %u.%u",
                   (unsigned int)reading->int_temp,
                   (unsigned int)reading->float_temp);
    (void)snprintf(&rows[1][5], DISPLAY_COLUMNS - 5U + 1U, " %u.%u",
                   (unsigned int)reading->int_Hum,
                   (unsigned int)reading->float_Hum);
  } else {
    memcpy(&rows[0][6], "ERROR", 5U);
    memcpy(&rows[1][5], "ERROR", 5U);
  }
}
