#include "fuel_calc.h"

#include <stdio.h>
#include <stdlib.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "mileage_calc.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

#include "globals_calc.h"

#define FUEL_INTERVAL pdMS_TO_TICKS(20)
#define FUEL_CALC_INTERVAL pdMS_TO_TICKS(120)
// Requirement for even
#define MAX_FUELS (FUEL_CALC_INTERVAL / FUEL_INTERVAL)

static float fuel_array[MAX_FUELS];
static int count = 0;
static float avg_100 = 0.0;
static CalculationMethod current_method = METHOD_RK4;

float FuelCalcInstant() {
  float fuel = (float)(rand() % 100);
  // return 1;
  return fuel;
}

float FuelCalcAvg() { return globals_total_fuel * 100 / globals_total_distance; }

// Simpson's rule for fuel calculation
float CalculateFuelSimpson() {
  float fuel = 0.0;
  double h = FUEL_INTERVAL / 1000.0;

  fuel += fuel_array[0];
  fuel += fuel_array[MAX_FUELS - 1];

  for (int i = 1; i < MAX_FUELS; i++) {
    if (i % 2 == 0) {
      fuel += 2 * fuel_array[i];
    } else {
      fuel += 4 * fuel_array[i];
    }
  }
  return (h / 3) * fuel;
}

// Fourth-order Runge-Kutta method for fuel calculation
float CalculateFuelRK4() {
  float fuel = 0.0;
  double h = FUEL_INTERVAL / 1000.0;

  for (int i = 0; i < MAX_FUELS; i++) {
    double k1 = fuel_array[i];
    double k2 = (i < MAX_FUELS - 1) ? fuel_array[i + 1] : k1;
    double k3 = k2;
    double k4 = k3;

    fuel += (h / 6.0) * (k1 + 2 * k2 + 2 * k3 + k4);
  }
  return fuel;
}

// Function to choose the calculation method
float CalculateFuel() {
  switch (current_method) {
    case METHOD_SIMPSON:
      return CalculateFuelSimpson();
    case METHOD_RK4:
      return CalculateFuelRK4();
    default:
      return 0.0;
  }
}

static void FuelCalcTask(void *param) {
  (void)param;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  TickType_t xStartTime, xEndTime;
  xStartTime = xTaskGetTickCount();

  for (;;) {
    float instant_fuel = FuelCalcInstant();
    fuel_array[count++] = instant_fuel;

    if (count >= MAX_FUELS) {
      float fuel = CalculateFuel();
      globals_total_fuel += fuel;
      xEndTime = xTaskGetTickCount();
      LOG_DEBUG("total fuel = %.1f ml in %.3fs", globals_total_fuel,
                (xEndTime - xStartTime) / 1000.0);
      count = 0;
    }
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(FUEL_INTERVAL));
  }
}

void FuelCalcInit() {
  globals_total_fuel = 0.0;
  srand(time(NULL));
  xTaskCreate(FuelCalcTask, "FuelCalc", configMINIMAL_STACK_SIZE, NULL,
              configMAX_PRIORITIES / 3, NULL);
}
