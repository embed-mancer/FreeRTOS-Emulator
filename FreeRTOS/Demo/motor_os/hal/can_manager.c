#include "can_manager.h"

#include <stdlib.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "bsp_data.h"
#include "parser/frame_parser.h"

#define HAL_PARSER_READ_BUF_SIZE 256u

static uint8_t s_read_buf[HAL_PARSER_READ_BUF_SIZE];

void can_manager_dispatch() {
  size_t n = bsp_can_read(s_read_buf, sizeof(s_read_buf));
  if (n > 0) {
    frame_parser_feed(s_read_buf, n);
  }
}

static void can_manager_task(void *params) {
  (void)params;
  for (;;) {
    printf("CAN manager task running\n");
    can_manager_dispatch();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

bool can_manager_init(const char *device, uint32_t baudrate) {
  if (!bsp_can_init(device, baudrate)) {
    return false;
  }

  BaseType_t ret =
      xTaskCreate(can_manager_task, "can_task", configMINIMAL_STACK_SIZE, NULL,
                  tskIDLE_PRIORITY, NULL);
  if (ret != pdPASS) {
    return false;
  }

  return true;
}
