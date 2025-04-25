#include "frame_parser.h"
#include <string.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "queue.h"

#define PARSER_BUF_SIZE 256  /* 可根据最大帧长调整 */
#define MIN_FRAME_OVERHEAD 5 /* STAG+Type+Code+Len+CRC */

#define MAX_HANDLERS 32

static struct {
  uint32_t id;
  frame_handle_t cb;
} s_handlers[MAX_HANDLERS];
static size_t s_handler_count = 0;

static QueueHandle_t queue_handle = NULL;

/* 内部缓冲区 */
static uint8_t s_buf[PARSER_BUF_SIZE];
static size_t s_buf_len;

static void frame_parser_thread(void *para) {
  protocol_frame_t *msg;
  for (;;) {
    if (xQueueReceive(queue_handle, &msg, portMAX_DELAY) != pdPASS) {
      printf("receive error\n");
      continue;
    }
    for (size_t i = 0; i < s_handler_count; ++i) {
      if (s_handlers[i].id == msg->cmd_code) {
        s_handlers[i].cb(msg);
      }
    }
    vPortFree(msg);
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

int frame_parser_init() {
  s_buf_len = 0;
  if (queue_handle) {
    printf("already init");
    return 0;
  }

  queue_handle = xQueueCreate(10, sizeof(uint32_t));
  if (queue_handle == NULL) {
    printf("create error!");
    return -1;
  }

  if (xTaskCreate(frame_parser_thread, "fp_th", configMINIMAL_STACK_SIZE, NULL,
                  configMAX_PRIORITIES / 3, NULL) != pdPASS) {
    vQueueDelete(queue_handle);
    printf("[%s:%d] error\n", __func__, __LINE__);
    return -1;
  }

  return 0;
}

static void frame_parser_add(const protocol_frame_t *_msg) {
  if (queue_handle == NULL) {
    printf("[%s:%d] error\n", __FUNCTION__, __LINE__);
    return;
  }
  protocol_frame_t *msg = pvPortMalloc(PARSER_BUF_SIZE);
  if (msg != NULL) {
    memcpy(msg, _msg, _msg->data_len + 5);
    if (xQueueSend(queue_handle, &msg, pdMS_TO_TICKS(100)) != pdPASS) {
      vPortFree(msg);
      printf("[%s:%d] error\n", __FUNCTION__, __LINE__);
    }
  }
}

/**
 * @brief 尝试从 s_buf 中提取并回调所有完整帧。
 */
static void parser_try(void) {
  size_t idx = 0;
  while (s_buf_len - idx >= MIN_FRAME_OVERHEAD) {
    /* 1) 找 STAG */
    if (s_buf[idx] != UART_MCU_MSG_START_TAG) {
      ++idx;
      continue;
    }
    /* 2) 够拿 Len 字段吗？ */
    if (idx + 4 >= s_buf_len)
      break;
    uint8_t payload_len = s_buf[idx + 3];
    size_t frame_len    = (size_t)4 + payload_len + 1; /* header+data+crc */
    /* 3) 整帧够了吗？ */
    if (idx + frame_len > s_buf_len)
      break;

    /* 4) CRC 校验 */
    uint8_t crc = 0;
    for (size_t i = 0; i < frame_len - 1; ++i) {
      crc ^= s_buf[idx + i];
    }
    if (crc != s_buf[idx + frame_len - 1]) {
      /* CRC 错误，丢掉这个 STAG，继续搜索 */
      ++idx;
      continue;
    }

    /* 5) 填充 ProtocolFrame 并回调 */
    protocol_frame_t frame;
    frame.stag     = s_buf[idx];
    frame.cmd_type = s_buf[idx + 1];
    frame.cmd_code = s_buf[idx + 2];
    frame.data_len = payload_len;
    frame.payload  = &s_buf[idx + 4];
    frame.crc      = s_buf[idx + 3 + payload_len + 1]; /* 或 idx+frame_len-1 */
    frame_parser_add(&frame);

    /* 6) 丢弃已解析字节，重置 idx */
    idx += frame_len;
  }

  /* 将剩余未解析字节移到缓冲区前端 */
  if (idx > 0) {
    memmove(s_buf, &s_buf[idx], s_buf_len - idx);
    s_buf_len -= idx;
  }
}

void frame_parser_feed(const uint8_t *data, size_t length) {
  /* 防止溢出：如果新数据超长，则丢弃旧数据 */
  if (s_buf_len + length > PARSER_BUF_SIZE) {
    s_buf_len = 0;
  }
  /* 拷贝进缓冲并尝试解析 */
  memcpy(&s_buf[s_buf_len], data, length);
  s_buf_len += length;
  parser_try();
}

void frame_parser_register(const uint32_t id, frame_handle_t handler) {
  if (s_handler_count < MAX_HANDLERS) {
    s_handlers[s_handler_count++] = (typeof(s_handlers[0])){id, handler};
  }
}
