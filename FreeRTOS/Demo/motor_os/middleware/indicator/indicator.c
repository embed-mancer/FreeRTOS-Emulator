#include "indicator.h"
#include "../hal/can_manager.h"
#include "can_ids.h"
#include <stddef.h>

typedef struct {
  uint32_t id;
  uint8_t byte_index;
  uint8_t mask;
  uint8_t shift;
  indicator_type_t type;
} indicator_map_entry_s;

// static const indicator_map_entry_s indicator_map[] = {
//     /* ID 0x101 */
//     {CAN_ID_LIGHTS_101, 0, 0x01, 0, INDICATOR_TURN_LEFT},
//     {CAN_ID_LIGHTS_101, 0, 0x02, 1, INDICATOR_TURN_RIGHT},
//     {CAN_ID_LIGHTS_101, 0, 0x04, 2, INDICATOR_LOW_BEAM},
//     {CAN_ID_LIGHTS_101, 0, 0x08, 3, INDICATOR_HIGH_BEAM},
//     /* ID 0x345 */
//     {CAN_ID_LIGHTS_345, 0, 0x01, 0, INDICATOR_TPMS},
//     {CAN_ID_LIGHTS_345, 0, 0x02, 1, INDICATOR_FUEL_LEVEL},
//     {CAN_ID_LIGHTS_345, 0, 0x04, 2, INDICATOR_MOTOR_FAULT},
//     {CAN_ID_LIGHTS_345, 0, 0x08, 3, INDICATOR_WATER_TEMP_WARN},
//     /* ID 0x400: 全字节电池电量 */
//     {CAN_ID_LIGHTS_345, 1, 0xFF, 0, INDICATOR_BATTERY_LEVEL},
// };
//
// static const uint32_t indicator_ids[] = {
//     CAN_ID_LIGHTS_101,
//     CAN_ID_LIGHTS_345,
// };
//
// static void indicator_handler(const can_msg_t *msg) {
//   uint8_t raw = msg->data[0];
//   size_t i;
//
//   for (i = 0; i < COUNT_OF(indicator_map); ++i) {
//     if (msg->id == indicator_map[i].id) {
//       uint32_t value = (raw & indicator_map[i].mask) >> indicator_map[i].shift;
//       indicator_set(indicator_map[i].type, value);
//     }
//   }
// }
