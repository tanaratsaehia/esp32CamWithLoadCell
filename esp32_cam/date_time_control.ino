#include <time.h>

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7 * 3600;
const int   daylightOffset_sec = 0;

void date_time_init(){
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

String* get_date_time() {
  static String result[2]; // Static array to hold the date and time

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    result[0] = "Failed to obtain date";
    result[1] = "Failed to obtain time";
    date_time_init();
    return result;
  }

  char dateStringBuff[20]; // Buffer to hold the date
  strftime(dateStringBuff, sizeof(dateStringBuff), "%d-%B-%Y", &timeinfo);
  
  char timeStringBuff[10]; // Buffer to hold the time
  strftime(timeStringBuff, sizeof(timeStringBuff), "%H:%M:%S", &timeinfo);
  
  result[0] = String(dateStringBuff);
  result[1] = String(timeStringBuff);
  
  return result;
}