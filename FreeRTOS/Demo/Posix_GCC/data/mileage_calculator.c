#include "mileage_calculator.h"

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

// Enum for calculation methods
typedef enum {
  METHOD_SIMPSON,
  METHOD_RK4,
  // Future methods can be added here
} CalculationMethod;

static float speed_array[MAX_SPEEDS];
static int speed_count = 0;
static float last_speed = 0.0;
static float last_distance = 0.0;
static CalculationMethod current_method =
    METHOD_RK4;  // Default calculation method

// Function to simulate vehicle speed
float get_vehicle_speed() {
  float speed = (float)(rand() % 100);
  return speed;  // Return random speed
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
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SPEED_INTERVAL));
  }
}

void MileageCalculatorInit() {
  srand(time(NULL));
  xTaskCreate(SpeedSamplingTask, "Mileage", configMINIMAL_STACK_SIZE, NULL,
              configMAX_PRIORITIES / 3, NULL);
}
