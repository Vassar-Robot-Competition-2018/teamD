void setup() {
  pinMode(RED_F, OUTPUT);
  pinMode(GREEN_F, OUTPUT);
  pinMode(YELLOW_F, OUTPUT);
  pinMode(BLUE_F, OUTPUT);
  pinMode(RED_B, OUTPUT);
  pinMode(GREEN_B, OUTPUT);
  pinMode(YELLOW_B, OUTPUT);
  pinMode(BLUE_B, OUTPUT);

  Serial.begin(115200);
  tcsL.begin();  // initializes left color sensor
  tcsR.begin();  // initializes right color sensor
  pixy.init();

  Serial.println("Adafruit VL6180x test!");
  if (! vl.begin()) {
    Serial.println("Failed to find sensor");
    while (1);
  }
  Serial.println("Sensor found!");

  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  delay(10);

  // sets servo initial positions
  RightTurn();
  UpdatePan(0);
  UpdateArms(100, 100);
  delay(380);
  UpdateWheels(0, 0);
  delay(100);

  digitalWrite(RED_B, HIGH);
  delay(100);
  digitalWrite(RED_B, LOW);
  digitalWrite(GREEN_B, HIGH);
  delay(100);
  digitalWrite(GREEN_B, LOW);
  digitalWrite(YELLOW_B, HIGH);
  delay(100);
  digitalWrite(YELLOW_B, LOW);
  digitalWrite(BLUE_B, HIGH);
  delay(100);
  digitalWrite(BLUE_B, LOW);
  digitalWrite(WHITE, HIGH);
  delay(100);
  digitalWrite(WHITE, LOW);
  digitalWrite(RED_F, HIGH);
  delay(100);
  digitalWrite(RED_F, LOW);
  digitalWrite(GREEN_F, HIGH);
  delay(100);
  digitalWrite(GREEN_F, LOW);
  digitalWrite(YELLOW_F, HIGH);
  delay(100);
  digitalWrite(YELLOW_F, LOW);
  digitalWrite(BLUE_F, HIGH);
  delay(100);
  digitalWrite(BLUE_F, LOW);
  delay(1000);
  ColorSensors();
}
