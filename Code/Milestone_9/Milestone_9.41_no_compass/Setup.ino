void setup() {
  //  servoL.attach(8);  // attaches left wheel servo on pin 8
  //  servoR.attach(9);  // attaches right wheel servo on pin 9

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

  Wire.begin();
  compass.init();
  compass.enableDefault();

  compass.m_min = (LSM303::vector<int16_t>) {
    -518, -23, -97
  };
  compass.m_max = (LSM303::vector<int16_t>) {
    -150, +382, -49
  };

  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  // power
  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));

  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  delay(10);

  // sets servo initial positions
  UpdateWheels(-baseSpeedL, baseSpeedR);
  MotorUpdate(0, 0);
  UpdatePan(0);
  UpdateArms(100, 100);
  delay(500);
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
