#include <WiFi.h>
#include "esp_camera.h"
#include <WiFiManager.h>
#include <TridentTD_LineNotify.h>

#define SSID        "D"   // WiFi name
#define PASSWORD    "11223344"   // PASSWORD
#define LINE_TOKEN  "PEz5jDbZCiSVYNiXjMxA72OLiQupMjjmo4Bm3Xthl19" // TOKEN

unsigned long millisCouting;
String record_file_name;

WiFiManager wm;

void setup() {
  Serial.begin(115200);
  wm.setTimeout(300);
  wm.autoConnect("I_Care_Urine");
  if (WiFi.status() == WL_CONNECTED){
    Serial.println("c_wifi_connected");
  }else{
    Serial.println("c_wifi_fail");
  }
  // delay(3000);
  // esp_cam_init();
  sd_card_init();
  date_time_init();

  // WiFi.begin(SSID, PASSWORD);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("");
  // Serial.println("WiFi connected");
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());
  LINE.setToken(LINE_TOKEN);

  // // Add initial data to the CSV file
  // add_initial_data();
  // // Read and modify a specific record in the CSV file
  // edit_CSV("/data.csv", "2024-07-13", "99999");
  // // Read the CSV file to verify the changes
  // read_CSV("/data.csv");
  String* dateTime = get_date_time();
  record_file_name = "/" + dateTime[0] + ".csv";
  Serial.println(record_file_name);

  if (!is_file_exist(record_file_name)){
    add_initial_data(record_file_name.c_str());
    // add_initial_data("/testing.csv");
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

}
