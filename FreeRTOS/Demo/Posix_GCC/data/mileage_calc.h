/**
* @file mileage_calc.h
*
* @brief This message displayed in Doxygen Files index
*
* @ingroup PackageName
* (note: this needs exactly one @defgroup somewhere)
*
* @date	2024-10-18-02-40
* @author Nemausa
*
*/
#ifndef MILEAGE_CALC_H_
#define MILEAGE_CALC_H_

// Enum for calculation methods
typedef enum {
  METHOD_SIMPSON,
  METHOD_RK4,
  // Future methods can be added here
} CalculationMethod;

void MileageCalcInit();
float CalculateDistance();

#endif // MILEAGE_CALC_H_
