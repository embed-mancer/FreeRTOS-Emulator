#include "globals_calc.h"

double globals_total_fuel = 0.0;
double globals_total_distance = 0.0;
SemaphoreHandle_t  globals_mutex;
CalculationMethod globals_current_method = METHOD_RK4;

void GlobalsInitMutex() {
  globals_mutex = xSemaphoreCreateMutex();
  if (globals_mutex == NULL) {
    
  }
}