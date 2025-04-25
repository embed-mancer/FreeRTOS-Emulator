/**
 * @file vehicle_speed.h
 *
 * @brief This message displayed in Doxygen Files index
 *
 * @ingroup PackageName
 * (note: this needs exactly one @defgroup somewhere)
 *
 * @date 2025-04-25-10-03
 * @author Nemausa
 *
 */
#ifndef VEHICLE_SPEED_H_
#define VEHICLE_SPEED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

void vehicle_speed_set_value(double speed);
double vehicle_speed_get_value();

#ifdef __cplusplus
}
#endif

#endif  // VEHICLE_SPEED_H_
