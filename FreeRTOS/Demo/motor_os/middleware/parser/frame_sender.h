/**
 * @file frame_sender.h
 *
 * @brief FreeRTOS-based frame sender module.
 *
 * This module provides APIs to initialize a frame sender task
 * and send speed and RPM frames over CAN via a message queue.
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

/**
 * @brief Initialize the frame sender module and create its FreeRTOS task.
 *
 * This function creates a message queue and spawns a FreeRTOS task
 * that dequeues frames and sends them via CAN.
 *
 * @return 0 on success, -1 on failure
 */
int frame_sender_init();

/**
 * @brief Enqueue a speed frame for sending.
 *
 * @param speed_kmh Vehicle speed in km/h to pack into the frame payload.
 * @return true if enqueued successfully, false on queue full or allocation
 * error.
 */
bool frame_sender_speed(uint16_t speed_kmh);

/**
 * @brief Enqueue a speed frame for sending.
 *
 * @param speed_kmh Vehicle speed in km/h to pack into the frame payload.
 * @return true if enqueued successfully, false on queue full or allocation
 * error.
 */
bool frame_sender_rpm(uint16_t rpm);

#ifdef __cplusplus
}
#endif

#endif  // FRAME_SENDER_H_
