/**
 * @file frame_parser.h
 *
 * @brief Protocol frame parser with FreeRTOS integration.
 *
 * This module provides routines to feed raw byte streams, parse
 * protocol frames, and dispatch complete frames via a FreeRTOS queue
 * to a parser task for handler callbacks.
 *
 * @ingroup PackageName
 * (note: this needs exactly one @defgroup somewhere)
 *
 * @date 2025-04-24-15-37
 * @author Nemausa
 *
 */

#ifndef FRAME_PARSER_H_
#define FRAME_PARSER_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "protocol_frame.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*frame_handle_t)(const protocol_frame_t *frame);

/**
 * @brief Initialize the frame parser module and start parser task.
 *
 * @return 0 on success, -1 on error
 */
int frame_parser_init();

/**
 * @brief Feed incoming bytes into the parser.
 *
 * Bytes are appended to an internal buffer and parser_try() is called.
 * If buffer overflows, it is reset.
 *
 * @param data   Pointer to incoming byte array
 * @param length Number of bytes to feed
 */
void frame_parser_feed(const uint8_t *data, size_t length);

/**
 * @brief Register a callback for a specific frame cmd_code.
 *
 * @param id      Frame identifier (cmd_code) to match
 * @param handler Function to call when matching frame is parsed
 */
void frame_parser_register(const uint32_t id, frame_handle_t handler);

#ifdef __cplusplus
}
#endif

#endif  // FRAME_PARSER_H_
