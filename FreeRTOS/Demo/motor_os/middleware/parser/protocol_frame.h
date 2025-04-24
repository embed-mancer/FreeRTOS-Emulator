/**
 * @file protocol_frame.h
 *
 * @brief This message displayed in Doxygen Files index
 *
 * @ingroup PackageName
 * (note: this needs exactly one @defgroup somewhere)
 *
 * @date 2025-04-24-15-54
 * @author Nemausa
 *
 */
#ifndef PROTOCOL_FRAME_H_
#define PROTOCOL_FRAME_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MIN_FRAME_SIZE 5
#define UART_MCU_MSG_START_TAG 0x02

/**
 * @struct protocol_frame_t
 * @brief Parsed higher-level protocol frame.
 *
 * Frame layout:
 *   [0]             stag
 *   [1]             cmd_type
 *   [2]             cmd_code
 *   [3]             data_len
 *   [4 .. 4+data_len-1] payload
 *   [4+data_len]    crc (XOR of all previous bytes)
 *
 * @var stag      Start tag.
 * @var cmd_type  Command type.
 * @var cmd_code  Command code.
 * @var data_len  Length of payload in bytes.
 * @var payload   Pointer into raw buffer where payload begins.
 * @var crc       Checksum byte.
 */
typedef struct {
  uint8_t stag;
  uint8_t cmd_type;
  uint8_t cmd_code;
  uint8_t data_len;
  const uint8_t *payload;
  uint8_t crc;
} protocol_frame_t;

#ifdef __cplusplus
}
#endif

#endif  // PROTOCOL_FRAME_H_
