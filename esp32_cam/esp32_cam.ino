#include <WiFi.h>
#include "esp_camera.h"
#include <TridentTD_LineNotify.h>

#define SSID        "D"   // WiFi name
#define PASSWORD    "11223344"   // PASSWORD
#define LINE_TOKEN  "PEz5jDbZCiSVYNiXjMxA72OLiQupMjjmo4Bm3Xthl19" // TOKEN

unsigned long millisCouting;
String data;

void setup() {
  Serial.begin(115200);
  // delay(3000);
  // load_cell_init();
  esp_cam_init();

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  LINE.setToken(LINE_TOKEN);

   // Add initial data to the CSV file
  add_initial_data();

  // Read and modify a specific record in the CSV file
  edit_CSV("/data.csv", "2024-07-13", "99999");

  // Read the CSV file to verify the changes
  read_CSV("/data.csv");
}

void loop() {
  if (millis() - millisCouting >= 10000) {
    camera_fb_t* pic = get_picture(true);
    delay(250);
    data = "2024-07-15,Example Data,123,"+String(millis());
    write_CSV("/data.csv", data);
    Serial.println("saving picture to SD card...");
    String path = "/picture" + String(millis()) + ".jpg";
    if (save_image_to_sd(pic, path.c_str())) {
      Serial.println("Picture saved to SD card.");
    } else {
      Serial.println("Failed to save picture to SD card.");
    }
    Serial.println("sending picture...");
    LINE.notifyPicture("testing esp cam...", pic->buf, pic->len);
    Serial.println("picture sent!"); 
    read_CSV("/data.csv");
    millisCouting = millis();
  }
  // if (millis() % 100 == 0){
  //   Serial.println("load cell : " + String(read_load_cell()));
  // }
}
