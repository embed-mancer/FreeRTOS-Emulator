#include "fuel_calc.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "mileage_calc.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

#include "globals_calc.h"

#define FUEL_INTERVAL_MS 20
#define FUEL_CALC_INTERVAL_MS 120
#define FUEL_INTERVAL pdMS_TO_TICKS(FUEL_INTERVAL_MS)
#define FUEL_CALC_INTERVAL pdMS_TO_TICKS(FUEL_CALC_INTERVAL_MS)
// Requirement for even
#define MAX_FUELS (FUEL_CALC_INTERVAL_MS / FUEL_INTERVAL_MS)

static float fuel_array[MAX_FUELS];
static int fuel_count = 0;
static float avg_100 = 0.0;

// Function to simulate instant fuel consumption
static float FuelCalcInstant(void) {
  return 27.778f;  // Fixed value for consistent simulation
}

// Function to calculate average fuel consumption
float FuelCalcAvg(void) {
  if (globals_total_distance == 0.0f) {
    return 0.0f;
  }
  return (globals_total_fuel * 100) / globals_total_distance;
}

// Simpson's rule for fuel calculation
static float CalculateFuelSimpson(void) {
  float fuel = fuel_array[0] + fuel_array[MAX_FUELS - 1];
  double h = FUEL_INTERVAL_MS / 1000.0;

  for (int i = 1; i < MAX_FUELS - 1; ++i) {
    fuel += (i % 2 == 0) ? 2 * fuel_array[i] : 4 * fuel_array[i];
  }

  return (h / 3) * fuel;
}

// Fourth-order Runge-Kutta method for fuel calculation
static float CalculateFuelRK4(void) {
  float fuel = 0.0f;
  double h = FUEL_INTERVAL_MS / 1000.0;

  for (int i = 0; i < MAX_FUELS; ++i) {
    double k1 = fuel_array[i];
    double k2 = (i < MAX_FUELS - 1) ? fuel_array[i + 1] : k1;
    double k3 = k2;
    double k4 = k3;

    fuel += (h / 6.0) * (k1 + 2 * k2 + 2 * k3 + k4);
  }

  return fuel;
}

// Function to choose the calculation method
float CalculateFuel(void) {
  switch (globals_current_method) {
    case METHOD_SIMPSON:
      return CalculateFuelSimpson();
    case METHOD_RK4:
      return CalculateFuelRK4();
    default:
      return 0.0f;
  }
}

// Task to calculate fuel consumption
static void FuelCalcTask(void *param) {
  (void)param;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  TickType_t xStartTime = xLastWakeTime;

  for (;;) {
    // Get the current fuel consumption and store it in the array
    float instant_fuel = FuelCalcInstant();
    fuel_array[fuel_count++] = instant_fuel;

    // If array is full, calculate fuel consumption and log results
    if (fuel_count >= MAX_FUELS) {
      float fuel = CalculateFuel();

      // Update global fuel with mutual exclusion
      if (xSemaphoreTake(globals_mutex, portMAX_DELAY) == pdTRUE) {
        globals_total_fuel += fuel;
        TickType_t xEndTime = xTaskGetTickCount();

        LOG_DEBUG("Total fuel = %.1f ml in %.3fs", globals_total_fuel,
                  (xEndTime - xStartTime) / 1000.0);

        xSemaphoreGive(globals_mutex);
      }

      // Reset fuel count for the next calculation
      fuel_count = 0;
    }

    // Delay until the next fuel measurement
    vTaskDelayUntil(&xLastWakeTime, FUEL_INTERVAL);
  }
}

// Initialization function for fuel calculation
void FuelCalcInit(void) {
  globals_total_fuel = 0.0f;
  GlobalsInitMutex();
  srand((unsigned int)time(NULL));
  xTaskCreate(FuelCalcTask, "FuelCalc", configMINIMAL_STACK_SIZE, NULL,
              tskIDLE_PRIORITY + 1, NULL);
}

void CalcInit(void) {
  FuelCalcInit();
  MileageCalcInit();
}
