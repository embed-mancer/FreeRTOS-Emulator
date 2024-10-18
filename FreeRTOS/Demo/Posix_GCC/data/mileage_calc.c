#include "mileage_calc.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

#include "globals_calc.h"
#include "fuel_calc.h"

#define SPEED_INTERVAL_MS 20
#define CALC_INTERVAL_MS 120
#define SPEED_INTERVAL pdMS_TO_TICKS(SPEED_INTERVAL_MS)
#define CALC_INTERVAL pdMS_TO_TICKS(CALC_INTERVAL_MS)
// Requirement for even
#define MAX_SPEEDS (CALC_INTERVAL_MS / SPEED_INTERVAL_MS)

static float speed_array[MAX_SPEEDS];
static int speed_count = 0;
static float last_speed = 0.0;

extern float avg_fuel_l_100;

// Function to simulate vehicle speed
static float GetVehicleSpeed(void) {
  return 27.778f;  // Fixed value for consistent simulation
}

// Simpson's rule for distance calculation
static float CalculateDistanceSimpson(void) {
  float distance = speed_array[0] + speed_array[MAX_SPEEDS - 1];
  double h = SPEED_INTERVAL_MS / 1000.0;

  for (int i = 1; i < MAX_SPEEDS - 1; ++i) {
    distance += (i % 2 == 0) ? 2 * speed_array[i] : 4 * speed_array[i];
  }

  return (h / 3) * distance;
}

// Fourth-order Runge-Kutta method for distance calculation
static float CalculateDistanceRK4(void) {
  float distance = 0.0f;
  double h = SPEED_INTERVAL_MS / 1000.0;

  for (int i = 0; i < MAX_SPEEDS; ++i) {
    double k1 = speed_array[i];
    double k2 = (i < MAX_SPEEDS - 1) ? speed_array[i + 1] : k1;
    double k3 = k2;
    double k4 = k3;

    distance += (h / 6.0) * (k1 + 2 * k2 + 2 * k3 + k4);
  }

  return distance;
}

// Function to choose the calculation method
float CalculateDistance(void) {
  switch (globals_current_method) {
    case METHOD_SIMPSON:
      return CalculateDistanceSimpson();
    case METHOD_RK4:
      return CalculateDistanceRK4();
    default:
      return 0.0f;
  }
}

// Task to calculate speed and distance
static void SpeedCalcTask(void *param) {
  (void)param;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  TickType_t xStartTime = xLastWakeTime;

  for (;;) {
    // Get the current speed and store it in the array
    float current_speed = GetVehicleSpeed();
    speed_array[speed_count++] = current_speed;
    last_speed = current_speed;

    // If array is full, calculate distance and log results
    if (speed_count >= MAX_SPEEDS) {
      float distance = CalculateDistance();

      // Update global distance with mutual exclusion
      if (xSemaphoreTake(globals_mutex, portMAX_DELAY) == pdTRUE) {
        globals_total_distance += distance;
        TickType_t xEndTime = xTaskGetTickCount();

        LOG_DEBUG("Total distance = %.2f m in %.3fs", globals_total_distance,
                  (xEndTime - xStartTime) / 1000.0);
        LOG_DEBUG("%.3f L/100km", FuelCalcAvg());

        xSemaphoreGive(globals_mutex);
      }

      // Reset speed count for the next calculation
      speed_count = 0;
    }

    // Delay until the next speed measurement
    vTaskDelayUntil(&xLastWakeTime, SPEED_INTERVAL);
  }
}

// Initialization function for mileage calculation
void MileageCalcInit(void) {
  srand((unsigned int)time(NULL));
  xTaskCreate(SpeedCalcTask, "MileageCalc", configMINIMAL_STACK_SIZE, NULL,
              tskIDLE_PRIORITY + 1, NULL);
}
