#include "can_data.h"
#include <stdio.h>

void vehicle_speed_callback(float value, uint32_t timestamp) {
  printf("Vehicle Speed Updated: %.2f, Timestamp: %u\n", value, timestamp);
}

void engine_temp_callback(float value, uint32_t timestamp) {
  printf("Engine Temp Updated: %.2f, Timestamp: %u\n", value, timestamp);
}

void engine_rpm_callback(float value, uint32_t timestamp) {
  printf("Engine RPM Updated: %.2f, Timestamp: %u\n", value, timestamp);
}

static const can_signal_t signals_123[] = {
    {0, 16, 0.1, 0.0, "Vehicle Speed", vehicle_speed_callback},
};

static const can_signal_t signals_124[] = {
    {0, 16, 1.0, -40.0, "Engine Temp", engine_temp_callback},
    {16, 16, 0.5, 0.0, "Engine RPM", engine_rpm_callback},
};

static const can_message_t can_message_table[] = {
    {0x123, signals_123, sizeof(signals_123) / sizeof(signals_123[0])},
    {0x124, signals_124, sizeof(signals_124) / sizeof(signals_124[0])},
};

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


void process_can_message(uint32_t id, const uint8_t* buffer) {
  for (size_t i = 0;
       i < sizeof(can_message_table) / sizeof(can_message_table[0]); i++) {
    if (can_message_table[i].id == id) {
      const can_message_t* message = &can_message_table[i];
      for (size_t j = 0; j < message->signal_count; j++) {
        const can_signal_t* signal = &message->signals[j];

        float value = can_extract_signal(buffer, signal);

        uint32_t timestamp;

        if (signal->callback) {
          signal->callback(value, timestamp);
        }
      }
    }
  }
}

void test_process_can_message() {
    uint8_t buffer_123[] = {0x10, 0x27};
    uint8_t buffer_124[] = {0x40, 0x00, 0x80, 0x20};

    process_can_message(0x123, buffer_123);
    process_can_message(0x124, buffer_124);
}
