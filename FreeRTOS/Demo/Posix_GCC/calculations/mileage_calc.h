/**
 * @file mileage_calc.h
 *
 * @brief Odometer and trip mileage calculation module.
 *
 * This module is responsible for calculating the total odometer mileage and
 * trip mileage based on periodic speed updates. It integrates with FreeRTOS
 * to perform mileage calculations in a background task.
 *
 * @ingroup MileageCalculation
 *
 * @details
 * Features include:
 * - Simpson's rule-based mileage calculation for accuracy.
 * - Automatic trip reset when the mileage exceeds the maximum allowed value.
 * - Integration with FreeRTOS task management for periodic updates.
 *
 * @date 2024-10-18
 * @version 1.0
 * @author Nemausa
 */

#ifndef MILEAGE_CALC_H_
#define MILEAGE_CALC_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the mileage calculation task.
 *
 * This function creates a FreeRTOS task responsible for calculating mileage
 * based on speed data retrieved at fixed intervals. The task will periodically
 * update the odometer and trip mileage.
 *
 * @note This function must be called once during system initialization.
 */
void mileage_calc_init();

#ifdef __cplusplus
}
#endif

#endif  // MILEAGE_CALC_H_
