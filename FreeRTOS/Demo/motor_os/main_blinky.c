#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"

#include "app_callbacks.h"
#include "can_manager.h"
#include "parser/frame_sender.h"

void MainApp(void) {
  can_manager_init("", 115200);
  app_register_handlers();
  frame_sender_rpm(12345);

  vTaskStartScheduler();

  for (;;) {
  }
}
