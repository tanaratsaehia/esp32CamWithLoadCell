#define buzzerPin 12

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  offBuzzer();
  load_cell_init();
}

void loop() {
  // Serial.println("Weight : " + String(get_weight()));
  // Serial.println("Battery : " + String(get_batt_percent()));
  // Serial.println("test");
}

void onBuzzer(){
  digitalWrite(buzzerPin, HIGH);
}

void offBuzzer(){
  digitalWrite(buzzerPin, LOW);
}