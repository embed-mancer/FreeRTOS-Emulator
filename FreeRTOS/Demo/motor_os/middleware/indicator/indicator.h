/**
 * @file indicator.h
 *
 * @brief This message displayed in Doxygen Files index
 *
 * @ingroup PackageName
 * (note: this needs exactly one @defgroup somewhere)
 *
 * @date 2025-04-24-14-46
 * @author Nemausa
 *
 */
#ifndef INDICATOR_H_
#define INDICATOR_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @enum indicator_type_t
 * @brief Enumeration of all indicator types used by the system.
 *
 * Each entry represents a distinct LED or gauge that can be updated
 * via CAN messages.
 */
typedef enum indicator_type {
  INDICATOR_TYPE_TURN_LEFT,   /**< Left turn indicator light */
  INDICATOR_TYPE_TURN_RIGHT,  /**< Right turn indicator light */
  INDICATOR_TYPE_LOW_BEAM,    /**< Low-beam headlight indicator */
  INDICATOR_TYPE_HIGH_BEAM,   /**< High-beam headlight indicator */
  INDICATOR_TYPE_TPMS,        /**< Tire Pressure Monitoring System warning */
  INDICATOR_TYPE_FUEL_LEVEL,  /**< Low fuel level warning */
  INDICATOR_TYPE_MOTOR_FAULT, /**< Motor fault indicator light */
  INDICATOR_TYPE_WATER_TEMP_WARN, /**< Water temperature warning */
  INDICATOR_TYPE_BATTERY_LEVEL,   /**< Battery level gauge (0–255) */
  INDICATOR_TYPE_COUNT            /**< Number of indicator types */
} indicator_type_t;

/**
 * @brief Initialize the indicator module.
 *
 * Registers all necessary CAN message callbacks for handling
 * indicator updates. Should be called once at startup.
 */
void indicator_init(void);

/**
 * @brief Update the state or value of an indicator.
 *
 * @param type  The indicator type to update.
 * @param value For boolean indicators: 0 = off, non-zero = on.
 *              For value-based indicators (e.g., battery level): 0–255.
 */
void indicator_set(indicator_type_t type, uint32_t value);

#endif  // INDICATOR_H_
