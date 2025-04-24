#include "app_callbacks.h"
#include <stdio.h>
#include "can_ids.h"
#include "../hal/can_manager.h"
#include "parser/protocol_frame.h"
#include "vehicle_speed.h"
#include "vehicle_rpm.h"
#include "indicator.h"

static void speed_handler(const protocol_frame_t *msg) {
  // vehicle_speed_set_status(kmh, true);
}

static void rpm_handler(const protocol_frame_t *msg) {
}

static void indicator_handler(const protocol_frame_t *msg) {
}
//
//   static void indicator_handler(const can_msg_t *msg) {
//     /* Mapping table: CAN ID, byte index, mask, shift, indicator type */
//     static const struct {
//       uint32_t id;
//       uint8_t byte_index;
//       uint8_t mask;
//       uint8_t shift;
//       indicator_type_t type;
//     } map[] = {
//         /* ID 0x101, bits in data[0] */
//         {CAN_ID_LIGHTS_101, 0, 0x01, 0, INDICATOR_TYPE_TURN_LEFT},
//         {CAN_ID_LIGHTS_101, 0, 0x02, 1, INDICATOR_TYPE_TURN_RIGHT},
//         {CAN_ID_LIGHTS_101, 0, 0x04, 2, INDICATOR_TYPE_LOW_BEAM},
//         {CAN_ID_LIGHTS_101, 0, 0x08, 3, INDICATOR_TYPE_HIGH_BEAM},
//         /* ID 0x345, bits in data[0] */
//         {CAN_ID_LIGHTS_345, 0, 0x02, 1, INDICATOR_TYPE_FUEL_LEVEL},
//         {CAN_ID_LIGHTS_345, 0, 0x04, 2, INDICATOR_TYPE_MOTOR_FAULT},
//         {CAN_ID_LIGHTS_345, 0, 0x08, 3, INDICATOR_TYPE_WATER_TEMP_WARN},
//         /* ID 0x345, full byte in data[1] */
//         {CAN_ID_LIGHTS_345, 1, 0xFF, 0, INDICATOR_TYPE_TPMS},
//         /* add more entries as needed */
//     };
//
//     size_t i;
//     for (i = 0; i < COUNT_OF(map); ++i) {
//       if (msg->id != map[i].id) {
//         continue;
//       }
//       if (msg->dlc <= map[i].byte_index) {
//         continue;
//       }
//       uint8_t raw    = msg->data[map[i].byte_index];
//       uint32_t value = (raw & map[i].mask) >> map[i].shift;
//       indicator_set(map[i].type, value);
//     }
//   }
//
void app_register_handlers() {
  can_manager_register(CAN_ID_VEHICLE_SPEED, speed_handler);
  can_manager_register(CAN_ID_ENGINE_RPM, rpm_handler);
  can_manager_register(CAN_ID_LIGHTS_101, indicator_handler);
  can_manager_register(CAN_ID_LIGHTS_345, indicator_handler);
}

void app_test() {
  printf("app test\n");
}
