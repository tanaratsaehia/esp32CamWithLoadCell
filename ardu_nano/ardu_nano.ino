#define buzzerPin 12
#define buttonPin 8

bool sleepMode = false;
const float waitTimeToSleep = 2; // minute

unsigned long sleepMillis;
unsigned long displayMillis;

void setup() {
  Serial.begin(9600);
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
  display_custom("Press button to", 0, 0);
  display_custom("start & set zero", 0, 1);
  while (!buttonPressed()){}
  onBuzzer(0.05);
  load_cell_set_zero();

  sleepMillis = millis();
}

void loop() {
  if (millis() - displayMillis >= 1000 & !sleepMode){
    displayMillis = millis();
    clear_lcd();
    display_battery(1, 0);
    display_weight(1, 1);
  }

  // serial format batt,weight,button
  if (millis() % 20 == 0 & !sleepMode){
    Serial.println(String(int(get_batt_percent())) + "," + String(get_weight(), 1) + "," + String(buttonPressed()));
  }

  if (buttonPressed()){
    sleepMillis = millis();
  }

  if (millis() - sleepMillis >= (waitTimeToSleep*60)*1000 & !sleepMode){
    sleepMode = true;
    lcd_sleep();
    load_cell_sleep();
  }else if (millis() - sleepMillis < (waitTimeToSleep*60)*1000 & sleepMode){
    sleepMode = false;
    lcd_wake();
    load_cell_wake();
  }else if (Serial.available() & sleepMode){
    char temp = Serial.read();
    sleepMillis = millis();
    sleepMode = false;
    lcd_wake();
    load_cell_wake();
  }

}

void onBuzzer(float second){
  digitalWrite(buzzerPin, HIGH);
  delay(second*1000);
  digitalWrite(buzzerPin, LOW);
}

void offBuzzer(){
  digitalWrite(buzzerPin, LOW);
}

bool buttonPressed(){
  return digitalRead(buttonPin);
}