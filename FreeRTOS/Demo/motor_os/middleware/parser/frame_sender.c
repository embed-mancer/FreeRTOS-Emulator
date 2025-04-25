#include "frame_sender.h"

#include <stdio.h>

#include "protocol_frame.h"
#include "can_manager.h"
#include "tool/tool.h"
#include "can_ids.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

static QueueHandle_t send_queue = NULL;

bool frame_sender_msg(uint8_t *msg) {
  if (send_queue == NULL ||
      xQueueSend(send_queue, &msg, pdMS_TO_TICKS(100)) != pdPASS) {
    vPortFree(msg);
    printf("[%s:%d] error\n", __FUNCTION__, __LINE__);
    return false;
  }
  return true;
}

void frame_sender_task(void *params) {
  uint8_t *msg;
  for (;;) {
    if (xQueueReceive(send_queue, &msg, portMAX_DELAY) != pdPASS) {
      printf("[%s:%d] error\n", __FUNCTION__, __LINE__);
      continue;
    }
    can_manager_send(msg, 0);
    vPortFree(msg);
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

bool frame_sender_speed(uint16_t speed_kmh) {
  protocol_frame_t frame = {
      .stag     = 0x02,
      .cmd_type = 0x01,
      .cmd_code = CAN_ID_VEHICLE_SPEED,
      .data_len = sizeof(speed_kmh),
      .payload  = (const uint8_t *)&speed_kmh,
  };

  uint8_t *buf = pvPortMalloc(sizeof(uint8_t) * 8);
  if (!buf)
    false;
  size_t len = protocol_frame_pack(&frame, buf, sizeof(buf));
  if (len == 0) {
    return false;
  }
  return frame_sender_msg(buf);
}

bool frame_sender_rpm(uint16_t rpm) {
  protocol_frame_t frame = {
      .stag     = 0x02,
      .cmd_type = 0x02,
      .cmd_code = CAN_ID_ENGINE_RPM,
      .data_len = sizeof(rpm),
      .payload  = (uint8_t[]){(uint8_t)(rpm >> 8), (uint8_t)(rpm & 0xFF)},
  };

  uint8_t *buf = pvPortMalloc(4 + frame.data_len + 1);
  if (!buf)
    false;
  size_t len = protocol_frame_pack(&frame, buf, sizeof(buf));
  if (len == 0) {
    vPortFree(buf);
    return false;
  }
    // printf("send data\n");
    // printf_data(buf);
  return frame_sender_msg(buf);
}

int frame_sender_init() {
  send_queue = xQueueCreate(10, sizeof(uint8_t *));
  if (send_queue == NULL) {
    printf("[%s:%d] error\n", __FUNCTION__, __LINE__);
    return -1;
  }
  int ret = xTaskCreate(frame_sender_task, "frame_sender_task",
                        configMINIMAL_STACK_SIZE, NULL,
                        configMAX_PRIORITIES / 3, NULL);
  if (ret != pdPASS) {
    vQueueDelete(send_queue);
    send_queue = NULL;
    printf("[%s:%d] error\n", __FUNCTION__, __LINE__);
    return -1;
  }

  return 0;
}
