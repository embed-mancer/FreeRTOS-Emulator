#include "vehicle_speed.h"

static double s_value = 0.0;

void vehicle_speed_set_value(double speed) {
  s_value = speed;
}

double vehicle_speed_get_value() {
  return s_value;
}
