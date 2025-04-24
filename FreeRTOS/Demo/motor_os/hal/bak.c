
/**
 * @file can_manager.h
 *
 * @brief This message displayed in Doxygen Files index
 *
 * @ingroup PackageName
 * (note: this needs exactly one @defgroup somewhere)
 *
 * @date 2025-04-24-15-25
 * @author Nemausa
 *
 */
#ifndef CAN_MANAGER_H_
#define CAN_MANAGER_H_

#include "../middleware/parser/protocol_frame.h"

typedef void (*frame_handler_t)(const protocol_frame_t *frame);

/**
 * @brief 初始化 CAN 管理器，打开接口
 * @param ifname CAN 接口名称，如 "can0"
 */
void can_manager_init(const char *ifname);

/**
 * @brief 注册指定 ID 的回调
 */
void can_manager_register(uint32_t id, frame_handler_t handler);

/**
 * @brief 调度：读取一帧并分发给对应回调
 */
void can_manager_dispatch(const protocol_frame_t *msg);

#endif  // CAN_MANAGER_H_
//
//


#include "can_manager.h"
#include <stdlib.h>

#include "FreeRTOS.h"
#include "queue.h"

#define MAX_HANDLERS 32

static struct {
  uint32_t id;
  frame_handler_t cb;
} s_handlers[MAX_HANDLERS];

static size_t s_hander_count = 0;

static QueueHandle_t s_frame_queue;

static void frame_processing_task(void *pvParameters);

void can_manager_init(const char *ifname) {
  s_frame_queue = xQueueCreate(16, sizeof(protocol_frame_t));
  xTaskCreate(frame_processing_task, "frm_prc", 256, NULL, 2, NULL);
}

void can_manager_register(uint32_t id, frame_handler_t handler) {
  if (s_hander_count < MAX_HANDLERS) {
    s_handlers[s_hander_count++] = (typeof(s_handlers[0])){id, handler};
  }
}

static void dispatch_frame_to_queue(const protocol_frame_t *frame) {
  protocol_frame_t local = *frame;
  if (xQueueSend(s_frame_queue, &local, 0) != pdPASS) {
  }
}

static void frame_processing_task(void *pvParameters) {
  protocol_frame_t frm;
  for (;;) {
    if (xQueueReceive(s_frame_queue, &frm, portMAX_DELAY) == pdPASS) {
      can_manager_dispatch(&frm);
    }
  }
}

void can_manager_dispatch(const protocol_frame_t *msg) {
  for (size_t i = 0; i < s_hander_count; ++i) {
    if (s_handlers[i].id == msg->cmd_code) {
      s_handlers[i].cb(msg);
    }
  }
}
