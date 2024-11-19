/**
 * @file can_data.h
 *
 * @brief This message displayed in Doxygen Files index
 *
 * @ingroup PackageName
 * (note: this needs exactly one @defgroup somewhere)
 *
 * @date	2024-11-19-17-12
 * @author Nemausa
 *
 */
#ifndef CAN_DATA_H_
#define CAN_DATA_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*can_signal_callback_t)(float value, uint32_t timestamp);

typedef struct {
  uint8_t start_bit;
  uint8_t length;
  float scale;
  float offset;
  const char* name;
  can_signal_callback_t callback;
} can_signal_t;

typedef struct {
  uint32_t id;
  const can_signal_t* signals;
  size_t signal_count;
} can_message_t;

#ifdef __cplusplus
}
#endif

#endif  // CAN_DATA_H_
