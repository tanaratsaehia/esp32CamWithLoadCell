#define buzzerPin 12
#define buttonPin 8

bool sleepMode = false;
bool presentMode = false;
const float waitTimeToSleep = 3.5; // minute

unsigned long sleepModeMillis;
unsigned long displayMillis;
unsigned long timeOutSerialMillis;
unsigned long presentModeMillis;
unsigned long waitNotiBattLoss;

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  offBuzzer();
  presentModeMillis = millis();

  lcd_init();
  onBuzzer(0.05);

  clear_display();
  display_custom("Welcome to", 3, 0);
  display_custom("I Care Urine", 2, 1);

  while (buttonPressed()){
    if (millis() - presentModeMillis >= 5000){
      onBuzzer(0.05);
      onBuzzer(0.05);
      presentMode = true;
      Serial.println("c_present_mode");
      clear_display();
      display_custom("present mode", 2, 0);
      display_custom("ready now!", 3, 1);
      while (buttonPressed()){}
    }
  }
  load_cell_init();
  Serial.println("hi");
  // for display config wifi
  read_command("INF");
  // for check file is exist or not
  read_command("INF");

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
  sleepModeEvent();
  if (get_batt_percent() > 10){
    waitNotiBattLoss = millis();
  }else if (get_batt_percent() < 10 & millis() - waitNotiBattLoss >= 2000){
    Serial.println("c_batt_loss");
  }
  read_command("one");
}

void sleepModeEvent(){
  if (buttonPressed()){
    sleepModeMillis = millis();
  }
  if (millis() - sleepModeMillis >= (waitTimeToSleep*60)*1000 & !sleepMode & !presentMode){
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