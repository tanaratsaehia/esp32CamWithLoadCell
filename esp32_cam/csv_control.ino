#include <FS.h>
#include <SD_MMC.h>
#include "esp_camera.h"

void sd_card_init(){
  if (!SD_MMC.begin("/sdcard", true)) {
    Serial.println("SD Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
}

void write_CSV(const char* filename, String data) {
  Serial.println("File : " + String(filename));
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

void record_data(const char* &filename, String data){
  
}

void check_record(const String &filename){
  if (is_file_exist(filename)){
    Serial.println("c_file_already_exist");

    while (true){
      if (Serial.available()){
        String command = Serial.readStringUntil('\n');
        if (command.startsWith("c_")){
          command = command.substring(2);
          command.trim();
          
          if (command == "delete_file"){
            SD_MMC.remove(filename);
            Serial.println("remove file");
            break;
          }else if (command == "keep_file"){
            // do nothing
            Serial.println("keep file");
            break;
          }
        }
      }
    }
  }else{
    Serial.println("c_file_created");
  }
}

bool is_file_exist(const String &filename) {
  // Check if the file exists
  if (SD_MMC.exists(filename)) {
    Serial.println("File exists.");

    // Check if the file extension is ".csv"
    if (filename.endsWith(".csv")) {
      Serial.println("File is a CSV file.");
      return true;
    } else {
      Serial.println("File is not a CSV file.");
      return false;
    }
  } else {
    Serial.println("File does not exist.");
    return false;
  }
}

bool delete_csv_file(const String filename){
  if (SD_MMC.exists(filename)) {
    // Serial.println("File exists.");
    // Attempt to delete the file
    if (SD_MMC.remove(filename)) {
      // Serial.println("File deleted.");
      return true;
    } else {
      // Serial.println("File deletion failed.");
      return false;
    }
  } else {
    // Serial.println("File does not exist.");
    return false;
  }
}

void add_initial_data(const char* filename) {
  // csv format date,time,weight,path_pic,upload_status
  write_CSV(filename, "'15-July-2024','11:36:44',330.5,'/picture5588.jpg',0");
  write_CSV(filename, "'15-July-2024','11:36:44',330.5,'/picture5588.jpg',0");
  write_CSV(filename, "'15-July-2024','11:36:44',330.5,'/picture5588.jpg',0");
  write_CSV(filename, "'15-July-2024','11:36:44',330.5,'/picture5588.jpg',0");
}