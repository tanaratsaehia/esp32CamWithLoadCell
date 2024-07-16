#define buzzerPin 12
#define buttonPin 8

bool sleepMode = false;
const float waitTimeToSleep = 3.5; // minute

unsigned long sleepModeMillis;
unsigned long displayMillis;
unsigned long timeOutSerialMillis;

void setup() {
  Serial.begin(115200);
  Serial.println("hi");
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  lcd_init();
  onBuzzer(0.05);

  clear_display();
  display_custom("Welcome to", 3, 0);
  display_custom("I Care Urine", 2, 1);

  load_cell_init();
  // for display config wifi
  read_command("INF");
  // for check file is exist or not
  read_command("INF");

  Serial.println("outnow");
  // delay(3000);

  clear_display();
  display_custom("Press button for", 0, 0);
  display_custom("set zero & start", 0, 1);
  while (!buttonPressed()){}
  onBuzzer(0.05);
  load_cell_set_zero();

  sleepModeMillis = millis();
}

void loop() {
  if (millis() - displayMillis >= 1000 & !sleepMode){
    displayMillis = millis();
    clear_display();
    display_batt_and_weight();
  }
  if (buttonPressed()){
    sleepModeMillis = millis();
  }
  if (millis() - sleepModeMillis >= (waitTimeToSleep*60)*1000 & !sleepMode){
    sleepMode = true;
    lcd_sleep();
    load_cell_sleep();
  }else if (millis() - sleepModeMillis < (waitTimeToSleep*60)*1000 & sleepMode){
    sleepMode = false;
    lcd_wake();
    load_cell_wake();
  }
}

void onBuzzer(float second){
  digitalWrite(buzzerPin, HIGH);
  delay(second*1000);
  digitalWrite(buzzerPin, LOW);
  delay(second*1000);
}

void offBuzzer(){
  digitalWrite(buzzerPin, LOW);
}

bool buttonPressed(){
  return digitalRead(buttonPin);
}