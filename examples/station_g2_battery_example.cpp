/*
 * Station G2 Battery Percentage Example
 *
 * This example shows how to calculate battery percentage from INA219 voltage readings.
 * The INA219 voltage is automatically updated in the board's getBattMilliVolts() method.
 */

#include <Arduino.h>
#include "target.h"

// Battery voltage ranges for different battery types
// Adjust these values based on your specific battery
const uint16_t BATTERY_MIN_MV = 2500*5;  // 2.5V - minimum voltage before cutoff
const uint16_t BATTERY_MAX_MV = 4200*5;  // 4.2V - maximum voltage when fully charged

// For LiFePO4 batteries, use these values instead:
// const uint16_t BATTERY_MIN_MV = 2800;  // 2.8V
// const uint16_t BATTERY_MAX_MV = 3600;  // 3.6V

uint8_t calculateBatteryPercentage(uint16_t voltage_mv) {
  if (voltage_mv <= BATTERY_MIN_MV) {
    return 0;
  }
  if (voltage_mv >= BATTERY_MAX_MV) {
    return 100;
  }

  // Linear interpolation between min and max voltage
  uint16_t voltage_range = BATTERY_MAX_MV - BATTERY_MIN_MV;
  uint16_t voltage_above_min = voltage_mv - BATTERY_MIN_MV;

  return (voltage_above_min * 100) / voltage_range;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Station G2 Battery Percentage Example");
  Serial.println("=====================================");

  // Initialize the board and sensors
  board.begin();
  radio_init();

  Serial.println("INA219 sensor initialized for battery monitoring");
  Serial.printf("Battery voltage range: %d mV - %d mV\n", BATTERY_MIN_MV, BATTERY_MAX_MV);
  Serial.println();
}

void loop() {
  // Get battery voltage from INA219 (automatically updated every 5 seconds)
  uint16_t battery_mv = board.getBattMilliVolts();
  uint8_t battery_percent = calculateBatteryPercentage(battery_mv);

  // Display battery information
  Serial.printf("Battery: %d mV (%.2f V) - %d%%\n",
                battery_mv,
                battery_mv / 1000.0f,
                battery_percent);

  // Battery status indicators
  if (battery_percent > 75) {
    Serial.println("Status: Good");
  } else if (battery_percent > 25) {
    Serial.println("Status: Low");
  } else if (battery_percent > 10) {
    Serial.println("Status: Critical");
  } else {
    Serial.println("Status: Shutdown imminent!");
  }

  Serial.println("---");

  delay(10000); // Update every 10 seconds
}
