/**
 * @file frame_parser.h
 *
 * @brief This message displayed in Doxygen Files index
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

int frame_parser_init();
void frame_parser_feed(const uint8_t *data, size_t length);
void frame_parser_register(const uint32_t id, frame_handle_t handler);

#ifdef __cplusplus
}
#endif

#endif  // FRAME_PARSER_H_
