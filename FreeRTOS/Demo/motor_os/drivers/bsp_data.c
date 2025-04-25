#include "bsp_data.h"
#include <string.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define CAN_SIM_BUF_SIZE 1024u
#define CAN_SIM_TASK_STACK (configMINIMAL_STACK_SIZE + 100)
#define CAN_SIM_TASK_PRIO (tskIDLE_PRIORITY + 1)
#define CAN_SIM_PERIOD_MS 100u

static uint8_t s_can_buf[CAN_SIM_BUF_SIZE];
static size_t s_can_head = 0, s_can_tail = 0;
static SemaphoreHandle_t s_can_mutex;

static const uint8_t s_sample_frame[] = {
    0x02, 0x55,              // 帧头
    0x10, 0x20, 0x30, 0x40,  // 示例载荷
    0xCC                     // 校验
};

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

static void _can_sim_task(void *params) {
  (void)params;
  for (;;) {
    xSemaphoreTake(s_can_mutex, portMAX_DELAY);
    _ringbuf_put(s_sample_frame, sizeof(s_sample_frame));
    xSemaphoreGive(s_can_mutex);

    printf("_can_sim_task running\n");
    vTaskDelay(pdMS_TO_TICKS(CAN_SIM_PERIOD_MS));
  }
}

bool bsp_can_init(const char *device, uint32_t baudrate) {
  (void)device;
  (void)baudrate;

  s_can_mutex = xSemaphoreCreateMutex();
  if (s_can_mutex == NULL) {
    return false;
  }

  BaseType_t ret = xTaskCreate(_can_sim_task, "CAN_Sim", CAN_SIM_TASK_STACK,
                               NULL, CAN_SIM_TASK_PRIO, NULL);
  return (ret == pdPASS);
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
