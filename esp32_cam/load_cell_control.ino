#include "HX711.h"

// Pin definitions
const int LOADCELL_DOUT_PIN = 16;
const int LOADCELL_SCK_PIN = 0;

// Create an HX711 instance
HX711 scale;

void load_cell_init(){
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  // Calibrate the scale
  // Assuming you have a known weight to calibrate the scale
  Serial.println("Place a known weight on the scale...");
  delay(5000); // Give some time to place the weight

  long zeroFactor = scale.read_average(); // Get a baseline reading
  Serial.print("Zero factor: "); // This can be used to tare the scale
  Serial.println(zeroFactor);

  float calibration_factor = 2280.0; // Adjust this value based on your calibration weight
  scale.set_scale(calibration_factor);
  scale.tare(); // Reset the scale to 0 with no weight

  Serial.println("Scale calibrated. Ready to read weights.");
}

float read_load_cell(){
  if (scale.is_ready()) {
    return scale.get_units(10); // Read average of 10 readings
  } else {
    return -1;
  }
}