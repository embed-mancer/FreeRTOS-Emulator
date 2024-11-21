#include "can_message.h"

static void vehicle_speed_callback(float value, uint32_t timestamp) {
  printf("Vehicle Speed Updated: %.2f, Timestamp: %u\n", value, timestamp);
}

static can_signal_t signals_123[] = {
    {0, 16, 0.1, 0.0, "Vehicle Speed", vehicle_speed_callback},
};

can_message_t message_123 = {0x123, signals_123, ARRAY_SIZE(signals_123)};
