#include "can_message.h"
#include <stdio.h>

extern can_message_t message_123;
extern can_message_t message_124;

can_message_t* can_message_table[] = {
    &message_123,
    &message_124,
};

size_t can_message_count = ARRAY_SIZE(can_message_table);

float can_extract_signal(const uint8_t* buffer, const can_signal_t* signal) {
  uint64_t raw_data = 0;
  uint8_t byte_index = signal->start_bit / 8;
  uint8_t bit_index = signal->start_bit % 8;
  uint8_t bit_length = signal->length;

  for (uint8_t i = 0; i < (bit_length + 7) / 8; i++) {
    raw_data |= ((uint64_t)buffer[byte_index + i]) << (i * 8);
  }

  raw_data >>= bit_index;
  raw_data &= ((1ULL << bit_length) - 1);

  return raw_data * signal->scale + signal->offset;
}

const can_message_t* get_message_by_id(uint32_t id) {
  for (size_t i = 0; i < ARRAY_SIZE(can_message_table); i++) {
    if (can_message_table[i]->id == id) {
      return can_message_table[i];
    }
  }
  return NULL;
}

void process_can_message(uint32_t id, const uint8_t* buffer) {
  uint32_t timestamp = 123456;
  const can_message_t* message = get_message_by_id(id);
  if (!message) return;

  for (size_t i = 0; i < message->signal_count; i++) {
    const can_signal_t* signal = &message->signals[i];
    float value = can_extract_signal(buffer, signal);

    if (signal->callback) {
      signal->callback(value, timestamp);
    }
  }
}

void debug_can_message(uint32_t id, const uint8_t* buffer) {
  const can_message_t* message = get_message_by_id(id);
  if (!message) return;

  printf("CAN Message ID: 0x%X\n", id);
  printf("can count = %d\n", message->signal_count);
  for (size_t i = 0; i < message->signal_count; i++) {
    const can_signal_t* signal = &message->signals[i];
    float value = can_extract_signal(buffer, signal);
    printf("  Signal: %s, Value: %.2f\n", signal->name, value);
  }
}

void debug_all_messages() {
  for (size_t i = 0; i < can_message_count; i++) {
    const can_message_t* message = can_message_table[i];
    printf("Message ID: 0x%X, Signal Count: %zu\n", message->id, message->signal_count);
    for (size_t j = 0; j < message->signal_count; j++) {
      const can_signal_t* signal = &message->signals[j];
      printf("  Signal: %s, Start Bit: %u, Length: %u, Scale: %.2f, Offset: %.2f\n",
             signal->name, signal->start_bit, signal->length, signal->scale, signal->offset);
    }
  }
}
