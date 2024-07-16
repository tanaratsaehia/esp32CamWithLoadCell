#include <WiFi.h>
#include "esp_camera.h"
#include <WiFiManager.h>
#include <TridentTD_LineNotify.h>

// #define SSID        "D"   // WiFi name
// #define PASSWORD    "11223344"   // PASSWORD
#define LINE_TOKEN  "PEz5jDbZCiSVYNiXjMxA72OLiQupMjjmo4Bm3Xthl19" // TOKEN
#define googleScriptID "https://script.google.com/macros/s/AKfycbzqyLveC_ZVLk_1Jw68CJ-QbS9_JxL7tpTrXkZSc4MDCms5dAxBzHFssmwPOQud6L-a/exec"

unsigned long millisCouting;
String record_file_name;
WiFiManager wm;

int nextHourRecord;
int nextMinuteRecord;

String* dateTime;
int timeIntArr[3];

void setup() {
  Serial.begin(115200);
  wm.setDebugOutput(false);
  wm.setTimeout(300);

  while (!Serial.available()){}
  String temp = Serial.readStringUntil('\n');
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
  nextHourRecord = timeIntArr[0];
  nextMinuteRecord = timeIntArr[1]+1;
  record_file_name = "/" + dateTime[0] + ".csv";
  // Serial.println(record_file_name);

  if (!is_file_exist(record_file_name)){
    add_initial_data(record_file_name.c_str());
    // add_initial_data("/testing.csv");
  }
  while (Serial.available()){
    temp = Serial.readStringUntil('\n');
  }
  check_record(record_file_name);
}

void loop() {
  // String* dateTime = get_date_time();
  // Serial.println(dateTime[0] + " " + dateTime[1]);

  // if (millis() - millisCouting >= 5000) {
  //   camera_fb_t* pic = get_picture(true);
  //   delay(250);
  //   data = "'15-July-2024','11:36:44',330.5,'/picture5588.jpg',0,"+String(millis());
  //   String path = "/picture" + String(millis()) + ".jpg";
  //   write_CSV(dateTime[0].c_str(), data);
  //   Serial.println("saving picture to SD card...");
  //   if (save_image_to_sd(pic, path.c_str())) {
  //     Serial.println("Picture saved to SD card.");
  //   } else {
  //     Serial.println("Failed to save picture to SD card.");
  //   }
  //   Serial.println("sending picture...");
  //   LINE.notifyPicture("testing esp cam...", pic->buf, pic->len);
  //   Serial.println("picture sent!"); 
  //   read_CSV(dateTime[0].c_str());
  //   millisCouting = millis();
  // }
  dateTime = get_date_time();
  splitTimeString(dateTime[1], timeIntArr);
  if (timeIntArr[0] == nextHourRecord & timeIntArr[1] == nextMinuteRecord){
    nextHourRecord++;
    // nextMinuteRecord++;
    Serial.println("c_wake_up");
    camera_fb_t* pic = get_picture(false);
    delay(250);
    float weight = get_weight();
    String pathImg = "/"+dateTime[0]+"_"+dateTime[1]+".jpg";
    String data = dateTime[0]+","+dateTime[1]+","+String(weight)+","+pathImg+",0";
    if (!save_image_to_sd(pic, pathImg.c_str())) {
      if (!save_image_to_sd(pic, pathImg.c_str())){
        // Serial.println("can't save image");
        data = dateTime[0]+","+dateTime[1]+","+String(weight)+",N/A,0";
      }
    }
    // Serial.println("weight : " + String(weight));
    // Serial.println(data);
    bool gg_res = sendToGoogleSheets(data);
    bool line_res = LINE.notifyPicture("testing esp cam...", pic->buf, pic->len);

    if (!gg_res){
      LINE.notify("can't upload data into google sheet");
    }

    write_CSV(record_file_name.c_str(), data);
  }
}

bool sendToGoogleSheets(String data) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(googleScriptID);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String httpRequestData = "data=" + data;
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode); // Print return code
      Serial.println(response);         // Print request answer
      http.end();
      return true;
    } else {
      // Serial.print("Error on sending POST: ");
      // Serial.println(httpResponseCode);
      http.end();
      return false;
    }
  } else {
    // Serial.println("Error in WiFi connection");
    return false;
  }
}
