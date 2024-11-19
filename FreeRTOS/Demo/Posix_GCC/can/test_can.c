#include "can_message.h"

void test_process_can_message() {
  uint8_t buffer_123[] = {0x10, 0x27};
  uint8_t buffer_124[] = {0x40, 0x00, 0x80, 0x20};

  process_can_message(0x123, buffer_123);
  process_can_message(0x124, buffer_124);

  debug_can_message(0x123, buffer_123);
  debug_can_message(0x124, buffer_124);
}

void test_can() {
  test_process_can_message();
}
