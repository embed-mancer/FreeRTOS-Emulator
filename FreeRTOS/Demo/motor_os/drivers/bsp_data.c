#include "bsp_data.h"
#include <string.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "tool/tool.h"

#define CAN_SIM_BUF_SIZE 1024u
#define CAN_SIM_TASK_STACK (configMINIMAL_STACK_SIZE + 100)
#define CAN_SIM_TASK_PRIO (tskIDLE_PRIORITY + 1)
#define CAN_SIM_PERIOD_MS 100u

static uint8_t s_can_buf[CAN_SIM_BUF_SIZE];
static size_t s_can_head = 0, s_can_tail = 0;
static SemaphoreHandle_t s_can_mutex;

static void _ringbuf_put(const uint8_t *data, size_t len) {
  for (size_t i = 0; i < len; i++) {
    size_t next = (s_can_head + 1) % CAN_SIM_BUF_SIZE;
    if (next == s_can_tail) {
      break;
    }
    s_can_buf[s_can_head] = data[i];
    s_can_head            = next;
  }
}

bool bsp_can_init(const char *device, uint32_t baudrate) {
  (void)device;
  (void)baudrate;

  s_can_mutex = xSemaphoreCreateMutex();
  if (s_can_mutex == NULL) {
    return false;
  }
  return true;
}

size_t bsp_can_read(uint8_t *buf, size_t buf_size) {
  size_t count = 0;
  xSemaphoreTake(s_can_mutex, portMAX_DELAY);
  while ((s_can_tail != s_can_head) && (count < buf_size)) {
    buf[count++] = s_can_buf[s_can_tail];
    s_can_tail   = (s_can_tail + 1) % CAN_SIM_BUF_SIZE;
  }
  xSemaphoreGive(s_can_mutex);
  return count;
}

void bsp_can_write(uint8_t *buf) {
  int len = buf[3] + 5;
  _ringbuf_put(buf, len);
}
