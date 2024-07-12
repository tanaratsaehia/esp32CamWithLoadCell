#include <WiFi.h>
#include "esp_camera.h"
#include <TridentTD_LineNotify.h>

#define SSID        "D"   // WiFi name
#define PASSWORD    "11223344"   // PASSWORD
#define LINE_TOKEN  "PEz5jDbZCiSVYNiXjMxA72OLiQupMjjmo4Bm3Xthl19" // TOKEN

unsigned long millisCouting;

void setup() {
  Serial.begin(115200);
  delay(2000);
  _cam_init();

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
}

void loop() {
  if (millis() - millisCouting >= 10000) {
    millisCouting = millis();
    camera_fb_t* pic = get_picture();
    delay(250);
    Serial.println("saving picture to SD card...");
    String path = "/sdcard/picture" + String(millis()) + ".jpg";
    if (save_image_to_sd(pic, path.c_str())) {
      Serial.println("Picture saved to SD card.");
    } else {
      Serial.println("Failed to save picture to SD card.");
    }
    Serial.println("sending picture...");
    LINE.notifyPicture("testing esp cam...", pic->buf, pic->len);
    Serial.println("picture sent!"); 
  }
}
