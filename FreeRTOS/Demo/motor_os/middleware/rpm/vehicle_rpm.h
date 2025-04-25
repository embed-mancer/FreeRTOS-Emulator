/**
 * @file vehicle_rpm.h
 *
 * @brief This message displayed in Doxygen Files index
 *
 * @ingroup PackageName
 * (note: this needs exactly one @defgroup somewhere)
 *
 * @date 2025-04-25-10-06
 * @author Nemausa
 *
 */
#ifndef VEHICLE_RPM_H_
#define VEHICLE_RPM_H_

#ifdef __cplusplus
extern "C" {
#endif

void vehicle_rpm_set_value(int rpm);
int vehicle_rpm_get_value();

#ifdef __cplusplus
}
#endif

#endif  // VEHICLE_RPM_H_
