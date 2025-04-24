/**
 * @file can_manager.h
 *
 * @brief This message displayed in Doxygen Files index
 *
 * @ingroup PackageName
 * (note: this needs exactly one @defgroup somewhere)
 *
 * @date 2025-04-24-18-21
 * @author Nemausa
 *
 */
#ifndef CAN_MANAGER_H_
#define CAN_MANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif

bool can_manager_init(const char *device, uint32_t baudrate);
void can_manager_dispatch();

#ifdef __cplusplus
}
#endif

#endif  // CAN_MANAGER_H_
