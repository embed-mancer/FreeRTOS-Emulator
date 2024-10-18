#include "data_receive.h"
#include "tool/constrant.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <pthread.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. */
#include "console.h"

#if __linux__
#define SEND_TASK
#endif

static QueueHandle_t xQueue = NULL;
static int count = 0;

static void QueueReceiveTask(void *param) {
  (void)param;
  uint8_t *msg;

  for (;;) {
    if (xQueueReceive(xQueue, &msg, portMAX_DELAY) != pdPASS) {
      printf("xQueueReceive error\n");
      continue;
    }
    DataTaskProcess(msg);
    vPortFree(msg);
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

#ifdef SEND_TASK
static void QueueSendTask(void *param) {
  (void)param;

  for (;;) {
    uint8_t *msg = pvPortMalloc(10);
    if (msg) {
      msg[0] = count++;
      if (xQueueSend(xQueue, &msg, 100) != pdPASS) {
        printf("xQueueSend error\n");
        vPortFree(msg);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
#endif

void DataTaskInit() {
  xQueue = xQueueCreate(10, sizeof(uint32_t));
  if (xQueue == NULL) {
    printf("xQueue create error.\n");
    return;
  }

  xTaskCreate(QueueReceiveTask,
              "Rx",
              configMINIMAL_STACK_SIZE,
              NULL,
              configMAX_PRIORITIES/3,
              NULL);
#ifdef SEND_TASK
  xTaskCreate(QueueSendTask,
              "TX",
              configMINIMAL_STACK_SIZE,
              NULL,
              configMAX_PRIORITIES/3,
              NULL);
#endif
}

void DataTaskAdd(uint8_t *_msg) {
  uint8_t *msg = malloc(kMaxMsgLen);
  if (!msg) {
    memcpy(msg, _msg, 256);
  }
}

void DataTaskProcess(uint8_t *msg) {
  printf("msg[0] = %d\n", msg[0]);
}

