#include "mileage_calc.h"

#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

#define SPEED_INTERVAL_MS 50
#define CALC_INTERVAL_MS 1050
#define SPEED_INTERVAL pdMS_TO_TICKS(SPEED_INTERVAL_MS)
#define CALC_INTERVAL pdMS_TO_TICKS(CALC_INTERVAL_MS)

#define MAX_SPEEDS ((CALC_INTERVAL_MS / SPEED_INTERVAL_MS) | 1)
#define TRIP_MILEAGE_MAX 1000000.0f

float odo_mileage = 0.0f;
float trip_mileage = 0.0f;

static float speed_array[MAX_SPEEDS];
static int speed_count = 0;

static float get_speed() {
  // TODO
  return 20000.0f;
}

static float calculate_distance() {
  double h = SPEED_INTERVAL_MS / 1000.0;
  float distance = speed_array[0] + speed_array[MAX_SPEEDS - 1];

  for (int i = 1; i < MAX_SPEEDS - 1; ++i) {
    distance += (i % 2 == 0) ? 2 * speed_array[i] : 4 * speed_array[i];
  }
  speed_array[0] = speed_array[MAX_SPEEDS - 1];
  return (h / 3.0) * distance;
}

void reset_trip(void) {
  trip_mileage = 0.0f;
}

void check_and_reset_trip(void) {
  if (trip_mileage >= TRIP_MILEAGE_MAX) {
    reset_trip();
  }
}
static void calc_task(void *param) {
  (void)param;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    vTaskDelayUntil(&xLastWakeTime, SPEED_INTERVAL);

    float current_speed = get_speed();
    speed_array[speed_count++] = current_speed;

    if (speed_count >= MAX_SPEEDS) {
      float distance = calculate_distance();
      odo_mileage += distance;
      trip_mileage += distance;
      speed_count = 1;
      check_and_reset_trip();
    }
  }
}

void mileage_calc_init(void) {
  xTaskCreate(calc_task, "odo_calc", configMINIMAL_STACK_SIZE, NULL,
              tskIDLE_PRIORITY + 1, NULL);
}