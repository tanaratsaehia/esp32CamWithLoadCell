#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

void lcd_init(){
  lcd.init();
  lcd.backlight();
}

void clear_lcd(){
  lcd.clear();
}

void display_battery(uint8_t column, uint8_t row){
  lcd.setCursor(column, row);
  lcd.print("Battery " + String(int(get_batt_percent())) + " %");
}

void display_weight(uint8_t column, uint8_t row){
  lcd.setCursor(column, row);
  lcd.print("Urine " + String(get_weight(), 1) + " ml");
}

void display_time(uint8_t column, uint8_t row){
  // not finish yet
  lcd.setCursor(column, row);
  // lcd.print();
}

void display_custom(String word, uint8_t column, uint8_t row){
  lcd.setCursor(column, row);
  lcd.print(word);
}

