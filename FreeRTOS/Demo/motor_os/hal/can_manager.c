#include "can_manager.h"

#include <stdlib.h>

#include "FreeRTOS.h"
#include "queue.h"

#include "bsp_data.h"
#include "parser/frame_parser.h"

#define HAL_PARSER_READ_BUF_SIZE 256u

static uint8_t s_read_buf[HAL_PARSER_READ_BUF_SIZE];

bool can_manager_init(const char *device, uint32_t baudrate) {
}

void can_manager_dispatch() {
  size_t n = bsp_can_read(s_read_buf, sizeof(s_read_buf));
  if (n > 0) {
    /* 2) Feed all received bytes to the frame parser */
    frame_parser_feed(s_read_buf, n);
  }
}
