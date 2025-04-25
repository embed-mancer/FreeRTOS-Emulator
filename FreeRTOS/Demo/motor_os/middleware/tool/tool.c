#include "tool.h"

#include <stdio.h>

void printf_data(uint8_t *data) {
  for (int i = 0; i < data[3] + 5; ++i) {
    printf("%02X ", data[i]);
  }
  printf("\n");
}
