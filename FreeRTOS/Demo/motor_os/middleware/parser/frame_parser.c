#include "frame_parser.h"
#include <string.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "queue.h"

#define PARSER_BUF_SIZE 256U /**< Maximum buffer size for incoming bytes */
#define MIN_FRAME_OVERHEAD \
  5U /**< Minimum bytes for a frame: STAG + Type + Code + Len + CRC */
#define MAX_HANDLERS 32U /**< Maximum number of registered frame handlers */

/**
 * @brief Registered frame handler entry.
 */
static struct {
  uint32_t id;       /**< Frame identifier (cmd_code) */
  frame_handle_t cb; /**< Callback when frame matches id */
} s_handlers[MAX_HANDLERS];

static size_t s_handler_count = 0; /**< Number of registered handlers */
static QueueHandle_t queue_handle =
    NULL; /**< FreeRTOS queue for parsed frames */

/**
 * @brief Internal byte buffer for accumulating incoming data.
 */
static uint8_t s_buf[PARSER_BUF_SIZE];
static size_t s_buf_len = 0; /**< Number of valid bytes in s_buf */

static void frame_parser_thread(void *para) {
  (void)para;
  protocol_frame_t *msg;

  for (;;) {
    if (xQueueReceive(queue_handle, &msg, portMAX_DELAY) != pdPASS) {
      printf("[frame_parser] receive error\n");
      continue;
    }
    /* Dispatch to matching handlers */
    for (size_t i = 0; i < s_handler_count; ++i) {
      if (s_handlers[i].id == msg->cmd_code) {
        s_handlers[i].cb(msg);
      }
    }

    vPortFree(msg);
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

int frame_parser_init(void) {
  if (queue_handle != NULL) {
    printf("[frame_parser] already initialized\n");
    return 0;
  }

  /* Create queue to hold pointers to parsed frames */
  queue_handle = xQueueCreate(10, sizeof(protocol_frame_t *));
  if (queue_handle == NULL) {
    printf("[frame_parser] queue creation failed!\n");
    return -1;
  }

  /* Spawn the parser thread */
  if (xTaskCreate(frame_parser_thread, "frame_parser", configMINIMAL_STACK_SIZE,
                  NULL, configMAX_PRIORITIES / 3, NULL) != pdPASS) {
    vQueueDelete(queue_handle);
    queue_handle = NULL;
    printf("[frame_parser:%d] task creation failed\n", __LINE__);
    return -1;
  }

  return 0;
}

static void frame_parser_add(const protocol_frame_t *_msg) {
  size_t frame_len = 4 /* header */ + _msg->data_len + 1 /* CRC */;
  // Allocate memory: structure itself + payload data area
  size_t alloc_size     = sizeof(protocol_frame_t) + _msg->data_len;
  protocol_frame_t *msg = pvPortMalloc(alloc_size);
  if (!msg)
    return;

  // 1) Copy fixed fields
  msg->stag     = _msg->stag;
  msg->cmd_type = _msg->cmd_type;
  msg->cmd_code = _msg->cmd_code;
  msg->data_len = _msg->data_len;

  // 2) Payload area follows immediately after the structure
  uint8_t *payload_dst = (uint8_t *)(msg + 1);
  memcpy(payload_dst, _msg->payload, _msg->data_len);
  msg->payload = payload_dst;

  // 3) If you need to include the CRC field, store it separately in msg

  // 4) Enqueue the message
  if (xQueueSend(queue_handle, &msg, pdMS_TO_TICKS(100)) != pdPASS) {
    vPortFree(msg);
    printf("[%s:%d] queue full\n", __FUNCTION__, __LINE__);
  }
}

static void parser_try() {
  size_t idx = 0;

  while (s_buf_len - idx >= MIN_FRAME_OVERHEAD) {
    /* 1) Search for start tag */
    if (s_buf[idx] != UART_MCU_MSG_START_TAG) {
      ++idx;
      continue;
    }
    /* 2) Ensure length field is available */
    if (idx + 4 >= s_buf_len) {
      break;
    }
    uint8_t payload_len = s_buf[idx + 3];
    size_t frame_len    = 4U + payload_len + 1U;
    /* 3) Check full frame availability */
    if (idx + frame_len > s_buf_len) {
      break;
    }
    /* 4) CRC check */
    uint8_t crc = 0;
    for (size_t i = 0; i < frame_len - 1; ++i) {
      crc ^= s_buf[idx + i];
    }
    if (crc != s_buf[idx + frame_len - 1]) {
      /* CRC mismatch: skip this start tag */
      ++idx;
      continue;
    }
    /* 5) Populate a temporary frame struct */
    protocol_frame_t frame;
    frame.stag     = s_buf[idx];
    frame.cmd_type = s_buf[idx + 1];
    frame.cmd_code = s_buf[idx + 2];
    frame.data_len = payload_len;
    frame.payload  = &s_buf[idx + 4];
    /* Note: CRC can be accessed via frame.payload[frame.data_len] */

    /* 6) Add complete frame to queue */
    frame_parser_add(&frame);

    /* Advance index past this frame */
    idx += frame_len;
  }

  /* Remove parsed bytes from buffer */
  if (idx > 0) {
    memmove(s_buf, &s_buf[idx], s_buf_len - idx);
    s_buf_len -= idx;
  }
}

void frame_parser_feed(const uint8_t *data, size_t length) {
  if (s_buf_len + length > PARSER_BUF_SIZE) {
    s_buf_len = 0;
  }
  memcpy(&s_buf[s_buf_len], data, length);
  s_buf_len += length;
  parser_try();
}

void frame_parser_register(uint32_t id, frame_handle_t handler) {
  if (s_handler_count < MAX_HANDLERS) {
    s_handlers[s_handler_count++] = (typeof(s_handlers[0])){id, handler};
  }
}
