#include "can_message.h"

static void engine_temp_callback(float value, uint32_t timestamp) {
  printf("Engine Temp Updated: %.2f, Timestamp: %u\n", value, timestamp);
}

static void engine_rpm_callback(float value, uint32_t timestamp) {
  printf("Engine RPM Updated: %.2f, Timestamp: %u\n", value, timestamp);
}

static can_signal_t signals_124[] = {
    {0, 16, 1.0, -40.0, "Engine temp", engine_temp_callback},
    {16, 16, 0.5, 0.0, "Engine rpm", engine_rpm_callback},
};

can_message_t message_124 = {0x124, signals_124, ARRAY_SIZE(signals_124)};
