#include "protocol_frame.h"

uint8_t protocol_frame_calc_crc8(const uint8_t *data, size_t len) {
  uint8_t crc = 0;
  for (size_t i = 0; i < len; ++i) {
    crc ^= data[i];
  }
  return crc;
}

size_t protocol_frame_pack(const protocol_frame_t *frame, uint8_t *buf,
                           size_t buf_size) {
  // Required: 4 header bytes + payload + 1 CRC byte
  size_t required = 4 + frame->data_len + 1;
  if (buf_size < required) {
    return 0;  // Buffer too small
  }
  if (frame->data_len > 0 && frame->payload == NULL) {
    return 0;  // Payload pointer invalid
  }

  size_t idx = 0;
  buf[idx++] = frame->stag;
  buf[idx++] = frame->cmd_type;
  buf[idx++] = frame->cmd_code;
  buf[idx++] = frame->data_len;

  // Copy payload if present
  if (frame->data_len > 0) {
    memcpy(&buf[idx], frame->payload, frame->data_len);
    idx += frame->data_len;
  }

  // Compute and append CRC8 over header + payload
  buf[idx] = protocol_frame_calc_crc8(buf, idx);
  ++idx;

  return idx;
}
