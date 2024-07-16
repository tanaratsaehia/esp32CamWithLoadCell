void read_command(String times){
  while (true){
    if (Serial.available()){
      String command = Serial.readStringUntil("\n");

      if (command.startsWith("c_")){
        command = command.substring(2);
        command.trim();

        if (command == "file_already_exist"){
          // file_already_exist
          clear_display();
          display_custom("select option", 2, 0);
          display_custom("keep record", 2, 1);

          bool btnState = false;
          bool disCase  = false;
          unsigned long pressBtnMillis;

          while (true){
            if (buttonPressed() & !btnState){
              btnState = true;
              pressBtnMillis = millis();
              // Serial.println("reset time counter");
            }else if (!buttonPressed() & btnState & millis() - pressBtnMillis>=100){
              btnState = false;
              if (!disCase & !buttonPressed()){
                disCase = true;
                clear_display();
                display_custom("select option", 2, 0);
                display_custom("delete record", 2, 1);
                while (buttonPressed()){}
              }else if (disCase & !buttonPressed()){
                disCase = false;
                clear_display();
                display_custom("select option", 2, 0);
                display_custom("keep record", 2, 1);
                while (buttonPressed()){}
              }
            }else if (buttonPressed()&millis()-pressBtnMillis>=3000&btnState){
              if (disCase){
                clear_display();
                display_custom("record deleted", 1, 0);
                onBuzzer(0.5);
                Serial.println("c_delete_file");
                while (buttonPressed()){}
                return;
              }else if (!disCase){
                clear_display();
                display_custom("keep record", 2, 0);
                onBuzzer(0.5);
                Serial.println("c_keep_file");
                while (buttonPressed()){}
                return;
              }
            }
          }
          return;
        }else if (command == "file_created"){
          return;
        }else if (command == "wifi_connected"){
          clear_display();
          display_custom("WiFi connected", 1, 0);
          onBuzzer(0.05);
          onBuzzer(0.05);
          delay(2500);
          return;
        }else if (command == "wifi_fail"){
          clear_display();
          display_custom("connect fail", 2, 0);
          display_custom("pls try again", 1, 1);
          onBuzzer(0.5);
          // reset mcu here
          return;
        }
      }else if (command.startsWith("*wm:")){
        command = command.substring(4);
        command.trim();

        if (command == "Starting Web Portal"){
          clear_display();
          display_custom("pls config wifi", 1, 0);
          display_custom("N:I_Care_Urine", 1, 1);
          // return;
        }
      }
    }

    if (times == "one"){
      return;
    }
  }
}