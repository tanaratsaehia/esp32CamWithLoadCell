#include <WiFi.h>
#include "esp_camera.h"
#include <WiFiManager.h>
#include <TridentTD_LineNotify.h>

#define LINE_TOKEN  "xlPCf7LajMbAHq0E9xNLST6gZlxnhm4BdaVIi9VwwxE" // TOKEN
#define googleScriptID "https://script.google.com/macros/s/AKfycbwUHt-JbJW4xp3ubmpTMTsT3LXqvfvpHDDsUUgPyTGBO-lYmXIJhIjyjJyMFid4C-E/exec"

unsigned long millisCouting;
String record_file_name;
WiFiManager wm;

bool presentMode = false;

int nextHourRecord;
int nextMinuteRecord;
float previousUrineWeight;
bool firstTimeOpenDevice = true;
bool stateNotiEightHour = false;

String* dateTime;
int timeIntArr[3];

void setup() {
  Serial.begin(115200);
  wm.setDebugOutput(false);
  wm.setTimeout(300);

  while (!Serial.available()){}
  String command = Serial.readStringUntil('\n');
  if (command.startsWith("c_")){
    command = command.substring(2);
    command.trim();
    if (command == "present_mode"){
      presentMode = true;
    }
  }
  // Serial.println("hi");
  // delay(500);
  // Serial.println("c_config_wifi");
  Serial.println("c_config_wifi");
  // delay(500);
  // delay(00);
  wm.autoConnect("I Care Urine");
  if (WiFi.status() == WL_CONNECTED){
    Serial.println("c_wifi_connected");
  }else{
    Serial.println("c_wifi_fail");
  }
  // delay(3000);
  esp_cam_init();
  sd_card_init();
  date_time_init();

  LINE.setToken(LINE_TOKEN);
  dateTime = get_date_time();
  splitTimeString(dateTime[1], timeIntArr);
  nextHourRecord = timeIntArr[0]+1;
  nextMinuteRecord = timeIntArr[1];
  record_file_name = "/" + dateTime[0] + ".csv";
  // Serial.println(record_file_name);

  if (!is_file_exist(record_file_name)){
    add_initial_data(record_file_name.c_str());
    // add_initial_data("/testing.csv");
  }
  while (Serial.available()){
    String temp = Serial.readStringUntil('\n');
  }
  check_record(record_file_name);
  readAndResendCSVRecords(record_file_name.c_str());

  LINE.notify("เครื่องพร้อมใช้งาน");
}

void loop() {
  if (Serial.available()){
    String command = Serial.readStringUntil('\n');
    if (command.startsWith("c_")){
      command = command.substring(2);
      command.trim();
      if (command == "batt_loss"){
        LINE.notify("แบตเตอรี่น้อยกว่า 10% กรุณาชาร์จแบต");
        delay(60000);
        return;
      }
    }
  }

  if (firstTimeOpenDevice){
    firstTimeOpenDevice = false;
    Serial.println("c_wake_up");
    float weight = get_weight();
    previousUrineWeight = weight;
  }

  dateTime = get_date_time();
  splitTimeString(dateTime[1], timeIntArr);
  String file_name = getFileName(record_file_name);
  // Serial.println(file_name);
  if (dateTime[0] != file_name){
    // Serial.println("change file name");
    record_file_name = dateTime[0];
  }
  if (timeIntArr[0] == nextHourRecord & timeIntArr[1] == nextMinuteRecord){
    // nextHourRecord++;
    if (presentMode){
      if (nextMinuteRecord < 60){
        nextMinuteRecord++;
      }else if (nextMinuteRecord == 60){
        nextMinuteRecord = 0;
      }
    }else{
      if (nextHourRecord < 23){
        nextHourRecord++;
      }else if (nextHourRecord == 23){
        nextHourRecord = 0;
      }
    }
    
    Serial.println("c_wake_up");
    camera_fb_t* pic = get_picture(false);
    delay(250);
    float weight = get_weight();
    String pathImg = "/"+dateTime[0]+"_"+dateTime[1]+".jpg";
    String data = dateTime[0]+","+dateTime[1]+","+String(weight)+","+pathImg;
    if (!save_image_to_sd(pic, pathImg.c_str())) {
      if (!save_image_to_sd(pic, pathImg.c_str())){
        // Serial.println("can't save image");
        data = dateTime[0]+","+dateTime[1]+","+String(weight)+",N/A";
      }
    }
    String notiStr;
    if (weight - previousUrineWeight < 30){
      notiStr = "\n!!! ปริมาณปัสสาวะน้อยกว่า 30 มล. !!!\nชั่วโมงที่แล้ว: "+String(previousUrineWeight)+" มล.\nปัจจุบัน: "+String(weight)+" มล.";
    }else{
      notiStr = "\nปริมาณปัสสาวะปกติ\nชั่วโมงที่แล้ว: "+String(previousUrineWeight)+" มล.\nปัจจุบัน: "+String(weight)+" มล.";
    }
    bool gg_res = false;
    bool line_res = false;
    if (WiFi.status() == WL_CONNECTED){
      line_res = LINE.notifyPicture(notiStr, pic->buf, pic->len);
      // gg_res = sendToGoogleSheets(data);
    }
    if (gg_res){
      if (line_res){
        data = dateTime[0]+","+dateTime[1]+","+String(weight)+","+pathImg+",1,1";
      }else{
        data = dateTime[0]+","+dateTime[1]+","+String(weight)+","+pathImg+",0,1";
      }
    }else{
      if (line_res){
        data = dateTime[0]+","+dateTime[1]+","+String(weight)+","+pathImg+",1,1";
      }else{
        data = dateTime[0]+","+dateTime[1]+","+String(weight)+","+pathImg+",0,1";
      }
      // LINE.notify("can't upload data into google sheet");
    }
    // Serial.println("gg res"+String(gg_res));
    write_CSV(record_file_name.c_str(), data);
    previousUrineWeight = weight;
    esp_camera_fb_return(pic);
  }

  if ((timeIntArr[0] == 6 | timeIntArr[0] == 14 | timeIntArr[0] == 22) & timeIntArr[1] == 0 & !stateNotiEightHour){
    stateNotiEightHour = true;
    Serial.println("c_wake_up");
    camera_fb_t* pic = get_picture(false);
    delay(250);
    float weight = get_weight();
    String notiStr = "นี่คือข้อความรายงานผลของปริมาณปัสสาวะตามเวลาเปลี่ยนเวร\nปริมาณปัสสาวะปัจจุบัน: "+String(weight)+" มล.";
    bool line_res = false;
    if (WiFi.status() == WL_CONNECTED){
      line_res = LINE.notifyPicture(notiStr, pic->buf, pic->len);
    }
    esp_camera_fb_return(pic);
  }
  if (timeIntArr[1] == 58){
    stateNotiEightHour = false;
  }
  if (timeIntArr[1] % 5 ==0){
    readAndResendCSVRecords(record_file_name.c_str());
  }
}


String getFileName(String filePath) {
  int lastSlashIndex = filePath.lastIndexOf('/');
  int dotIndex = filePath.lastIndexOf('.');
  
  if (dotIndex == -1) {
    // No extension found
    dotIndex = filePath.length();
  }
  
  String fileName = filePath.substring(lastSlashIndex + 1, dotIndex);
  return fileName;
}


