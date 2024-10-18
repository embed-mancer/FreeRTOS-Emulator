/**
* @file fuel_calc.h
*
* @brief This message displayed in Doxygen Files index
*
* @ingroup PackageName
* (note: this needs exactly one @defgroup somewhere)
*
* @date	2024-10-18-02-36
* @author Nemausa
*
*/
#ifndef FUEL_CALC_H_
#define FUEL_CALC_H_

void FuelCalcInit();
void FuelCalcUpdate(float *distance);
float FuelCalcAvg();
void CalcInit(void);

#endif // FUEL_CALC_H_
