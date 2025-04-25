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
#include <stddef.h>

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

/**
 * @brief  Calculate an 8-bit CRC using a simple XOR algorithm.
 * @param  data  Pointer to the data buffer to compute the CRC over.
 * @param  len   Number of bytes in the data buffer.
 * @return       The computed 8-bit CRC value.
 */
uint8_t protocol_frame_calc_crc8(const uint8_t *data, size_t len);

/**
 * @brief  Pack a protocol_frame_t into a byte buffer.
 *
 * Frame layout:
 *   [ stag      ] 1 byte
 *   [ cmd_type  ] 1 byte
 *   [ cmd_code  ] 1 byte
 *   [ data_len  ] 1 byte
 *   [ payload   ] data_len bytes (optional)
 *   [ crc       ] 1 byte (CRC8 over all previous bytes)
 *
 * @param  frame      Pointer to the frame to pack.
 * @param  buf        Destination buffer for packed bytes.
 * @param  buf_size   Size of the destination buffer.
 * @return            Number of bytes written on success, or 0 on failure
 *                    (e.g., insufficient buffer space or invalid payload).
 */
size_t protocol_frame_pack(const protocol_frame_t *frame, uint8_t *buf,
                           size_t buf_size);

#ifdef __cplusplus
}
#endif

#endif  // PROTOCOL_FRAME_H_
