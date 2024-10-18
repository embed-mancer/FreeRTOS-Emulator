#include "mileage_calculator.h"

#include <stdlib.h>
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

#define SPEED_INTERVAL pdMS_TO_TICKS(20)     
#define CALC_INTERVAL pdMS_TO_TICKS(120)
// Requirement for even
#define MAX_SPEEDS (CALC_INTERVAL / SPEED_INTERVAL)

static float speed_array[MAX_SPEEDS];
static int speed_count = 0;
static float last_speed = 0.0;
static float last_distance = 0.0;

float get_vehicle_speed() {
  float speed = (float)(rand() % 100);
  float speed_ms = speed * 1000 / 3600.0;
  return 1.0;
  // return speed_ms;
}

static void SpeedSamplingTask(void *param) {
  (void)param;
  TickType_t xLastWakeTime = xTaskGetTickCount(); 
  
  for (;;) {
    float current_speed = get_vehicle_speed();
    speed_array[speed_count++] = current_speed;
    last_speed = current_speed;

    if (speed_count >= MAX_SPEEDS) {
      float distance = CalculateDistance();
      last_distance += distance;
      speed_count = 0;
    }
    vTaskDelayUntil(&xLastWakeTime ,pdMS_TO_TICKS(SPEED_INTERVAL));
  }
}

float CalculateDistance() {
  float distance = 0.0;
  double h = SPEED_INTERVAL / 1000.0;

  distance += speed_array[0];
  distance += speed_array[MAX_SPEEDS - 1];

  for (int i = 1; i < MAX_SPEEDS; i++) {
    if (i % 2 == 0) {
        distance += 2 * speed_array[i];  
    } else {
        distance += 4 * speed_array[i]; 
    }
  }
  return (h / 3) * distance;
}

void MileageCalculatorInit() {
  srand(time(NULL));
  xTaskCreate(SpeedSamplingTask,
              "Mileage",
              configMINIMAL_STACK_SIZE,
              NULL,
              configMAX_PRIORITIES/3,
              NULL);
}
