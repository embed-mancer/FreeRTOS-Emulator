/**
 * @file can_message.h
 *
 * @brief This message displayed in Doxygen Files index
 *
 * @ingroup PackageName
 * (note: this needs exactly one @defgroup somewhere)
 *
 * @date	2024-11-19-19-42
 * @author Nemausa
 *
 */
#ifndef CAN_MESSAGE_H_
#define CAN_MESSAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

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
  can_signal_t* signals;
  size_t signal_count;
} can_message_t;

extern can_message_t* can_message_table[];
extern size_t can_message_count;

void process_can_message(uint32_t id, const uint8_t* buffer);
void debug_can_message(uint32_t id, const uint8_t* buffer);
void debug_all_messages();

#ifdef __cplusplus
}
#endif

#endif  // CAN_MESSAGE_H_
