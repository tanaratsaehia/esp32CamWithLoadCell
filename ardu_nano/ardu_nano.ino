#define buzzerPin 12
#define buttonPin 8

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  offBuzzer();
  lcd_init();

  clear_lcd();
  display_custom("Welcome to", 3, 0);
  display_custom("I Care Urine", 2, 1);

  load_cell_init();
  delay(3000);

  clear_lcd();
  display_custom("press button to", 0, 0);
  display_custom("start & set zero", 0, 1);
  while (!buttonPressed()){}
  load_cell_set_zero();
}

void loop() {
  // Serial.println("Weight : " + String(get_weight()));
  // Serial.println("Battery : " + String(get_batt_percent()));
  // Serial.println("test");
  if (millis() % 1000 == 0){
    clear_lcd();
    display_battery(1, 0);
    display_weight(1, 1);
  }
}

void onBuzzer(){
  digitalWrite(buzzerPin, HIGH);
}

void offBuzzer(){
  digitalWrite(buzzerPin, LOW);
}

bool buttonPressed(){
  return digitalRead(buttonPin);
}