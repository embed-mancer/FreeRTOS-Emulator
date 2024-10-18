#include "mileage_calc.h"

#include <stdio.h>
#include <stdlib.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

#define SPEED_INTERVAL pdMS_TO_TICKS(20)
#define CALC_INTERVAL pdMS_TO_TICKS(120)
// Requirement for even
#define MAX_SPEEDS (CALC_INTERVAL / SPEED_INTERVAL)

static float speed_array[MAX_SPEEDS];
static int count = 0;
static float last_speed = 0.0;
static float total_ditance = 0.0;
static CalculationMethod current_method =
    METHOD_RK4;  // Default calculation method

extern float avg_fuel_l_100;
// Function to simulate vehicle speed
float GetVehicleSpeed() {
  float speed = (float)(rand() % 100);
  return 27.778;
  return speed;
}

// Simpson's rule for distance calculation
float CalculateDistanceSimpson() {
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

// Fourth-order Runge-Kutta method for distance calculation
float CalculateDistanceRK4() {
  float distance = 0.0;
  double h = SPEED_INTERVAL / 1000.0;

  for (int i = 0; i < MAX_SPEEDS; i++) {
    double k1 = speed_array[i];
    double k2 = (i < MAX_SPEEDS - 1) ? speed_array[i + 1] : k1;
    double k3 = k2;
    double k4 = k3;

    distance += (h / 6.0) * (k1 + 2 * k2 + 2 * k3 + k4);
  }
  return distance;  // Return calculated distance
}

// Function to choose the calculation method
float CalculateDistance() {
  switch (current_method) {
    case METHOD_SIMPSON:
      return CalculateDistanceSimpson();
    case METHOD_RK4:
      return CalculateDistanceRK4();
    default:
      return 0.0;
  }
}

static void SpeedCalcTask(void *param) {
  (void)param;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  TickType_t xStartTime, xEndTime;
  xStartTime = xTaskGetTickCount();

  for (;;) {
    float current_speed = GetVehicleSpeed();
    speed_array[count++] = current_speed;
    last_speed = current_speed;

    if (count >= MAX_SPEEDS) {
      float distance = CalculateDistance();
      total_ditance += distance;
      xEndTime = xTaskGetTickCount();
      float total = total_ditance;
      FuelCalcUpdate(&total_ditance);
      printf("total distance = %f m in %fs \n", total_ditance,
             (xEndTime - xStartTime) / 1000.0);
      printf("Avg =%f L/100km\n", avg_fuel_l_100);
      count = 0;
    }
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SPEED_INTERVAL));
  }
}

void MileageCalcInit() {
  srand(time(NULL));
  xTaskCreate(SpeedCalcTask, "MileageCalc", configMINIMAL_STACK_SIZE, NULL,
              configMAX_PRIORITIES / 3, NULL);
}
