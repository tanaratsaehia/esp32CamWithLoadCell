#include <FS.h>
#include <SD_MMC.h>
#include "esp_camera.h"

void write_CSV(const char* filename, String data) {
  File file = SD_MMC.open(filename, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.println(data)) {
    Serial.println("Data written successfully");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void read_CSV(const char* filename) {
  File file = SD_MMC.open(filename);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }
  while (file.available()) {
    String line = file.readStringUntil('\n');
    Serial.println(line);
  }
  file.close();
}

void edit_CSV(const char* filename, String condition, String newValue) {
  File file = SD_MMC.open(filename, FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  // Read file into memory
  String fileContent = "";
  while (file.available()) {
    String line = file.readStringUntil('\n');
    String timestamp = line.substring(0, line.indexOf(',')); // Get the timestamp
    if (timestamp == condition) {
      int lastCommaIndex = line.lastIndexOf(',');
      line = line.substring(0, lastCommaIndex + 1) + newValue; // Modify the last column
    }
    fileContent += line + "\n";
  }
  file.close();

  // Write modified content back to file
  file = SD_MMC.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  file.print(fileContent);
  file.close();

  Serial.println("Modified CSV file and updated the last column based on the condition.");
}

void add_initial_data() {
  write_CSV("/data.csv", "2024-07-11,Example Data,123,4336");
  write_CSV("/data.csv", "2024-07-14,Example Data,123,10357");
  write_CSV("/data.csv", "2024-07-15,Example Data,123,24829");
  write_CSV("/data.csv", "2024-07-10,Example Data,123,39221");
  write_CSV("/data.csv", "2024-07-13,Example Data,123,53373");
  write_CSV("/data.csv", "2024-07-18,Example Data,123,67285");
  write_CSV("/data.csv", "2024-07-19,Example Data,123,80958");
}