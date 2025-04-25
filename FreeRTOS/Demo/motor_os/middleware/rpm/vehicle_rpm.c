#include "vehicle_rpm.h"

static int s_value = 0;

void vehicle_rpm_set_value(int rpm) {
  s_value = rpm;
}

int vehicle_rpm_get_value() {
  return s_value;
}
