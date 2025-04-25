#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"

#include "app_callbacks.h"
#include "can_manager.h"
#include "parser/frame_sender.h"

static int rpm = 0;
static void send_rpm_task(void *params) {
  for (;;) {
    frame_sender_rpm(rpm++);
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void MainApp(void) {
  can_manager_init("", 115200);
  app_register_handlers();
  xTaskCreate(send_rpm_task, "send_rpm_task", configMINIMAL_STACK_SIZE, NULL,
              configMAX_PRIORITIES / 3, NULL);
  vTaskStartScheduler();

  for (;;) {
  }
}
