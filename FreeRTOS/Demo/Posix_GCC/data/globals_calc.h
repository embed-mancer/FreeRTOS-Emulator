/**
* @file globals_calc.h
*
* @brief This message displayed in Doxygen Files index
*
* @ingroup PackageName
* (note: this needs exactly one @defgroup somewhere)
*
* @date	2024-10-18-22-43
* @author Nemausa
*
*/
#ifndef GLOBALS_CALC_H_
#define GLOBALS_CALC_H_

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define LOG_DEBUG(fmt, ...) \
  printf("[DEBUG][%s@%d] " fmt "\n", __func__, __LINE__, ##__VA_ARGS__)

#define LOG_INFO(fmt, ...) \
  printf("[INFO][%s@%d] " fmt "\n", __func__, __LINE__, ##__VA_ARGS__)

#define LOG_WARNING(fmt, ...) \
  printf("[WARNING][%s@%d] " fmt "\n", __func__, __LINE__, ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...) \
  printf("[ERROR][%s@%d] " fmt "\n", __func__, __LINE__, ##__VA_ARGS__)

// Enum for calculation methods
typedef enum {
  METHOD_SIMPSON,
  METHOD_RK4,
  // Future methods can be added here
} CalculationMethod;

extern SemaphoreHandle_t  globals_mutex;
extern double globals_total_fuel;
extern double globals_total_distance;
extern CalculationMethod globals_current_method;

void GlobalsInitMutex();

#endif  // GLOBALS_CALC_H_
