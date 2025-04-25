/**
 * @file frame_sender.h
 *
 * @brief This message displayed in Doxygen Files index
 *
 * @ingroup PackageName
 * (note: this needs exactly one @defgroup somewhere)
 *
 * @date 2025-04-25-11-35
 * @author Nemausa
 *
 */
#ifndef FRAME_SENDER_H_
#define FRAME_SENDER_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int frame_sender_init();
bool frame_sender_speed(uint16_t speed_kmh);
bool frame_sender_rpm(uint16_t rpm);

#ifdef __cplusplus
}
#endif

#endif  // FRAME_SENDER_H_
