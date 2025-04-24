#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"

void ledTask(void* pvParameters) {
  TickType_t lastWakeUp = xTaskGetTickCount();
  bool ledState         = false;

  while (1) {
    vTaskDelayUntil(&lastWakeUp, pdMS_TO_TICKS(1000));
  }
}

void MainApp(void) {
  xTaskCreate(ledTask, "LED", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY,
              NULL);

  vTaskStartScheduler();

  for (;;) {
  }
}
