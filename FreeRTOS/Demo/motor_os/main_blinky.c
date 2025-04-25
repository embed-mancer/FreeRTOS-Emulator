#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"

#include "app_callbacks.h"
#include "can_manager.h"

void MainApp(void) {
  can_manager_init("", 115200);
  app_register_handlers();

  vTaskStartScheduler();

  for (;;) {
  }
}
