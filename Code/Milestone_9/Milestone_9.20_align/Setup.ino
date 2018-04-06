void setup() {
  servoL.attach(8);  // attaches left wheel servo on pin 8
  servoR.attach(9);  // attaches right wheel servo on pin 9
  //  blockServoL.attach(11); // attaches left block servo on pin 22
  //  blockServoR.attach(10); // attaches right block servo on pin 23

  pinMode(RED_F, OUTPUT);
  pinMode(YELLOW_F, OUTPUT);
  pinMode(GREEN_F, OUTPUT);
  pinMode(BLUE_F, OUTPUT);
  pinMode(RED_B, OUTPUT);
  pinMode(YELLOW_B, OUTPUT);
  pinMode(GREEN_B, OUTPUT);
  pinMode(BLUE_B, OUTPUT);

  Serial.begin(115200);
  tcsL.begin();  // initializes left color sensor
  tcsR.begin();  // initializes right color sensor
  pixy.init();

  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  delay(10);

  // sets servo initial positions
  MotorUpdate(0, 0);
  UpdatePan(0);
  UpdateArms(100, 100);
  delay(1000);

  digitalWrite(RED_B, HIGH);
  delay(100);
  digitalWrite(RED_B, LOW);
  digitalWrite(YELLOW_B, HIGH);
  delay(100);
  digitalWrite(YELLOW_B, LOW);
  digitalWrite(GREEN_B, HIGH);
  delay(100);
  digitalWrite(GREEN_B, LOW);
  digitalWrite(BLUE_B, HIGH);
  delay(100);
  digitalWrite(BLUE_B, LOW);
  digitalWrite(WHITE, HIGH);
  delay(100);
  digitalWrite(WHITE, LOW);
  digitalWrite(RED_F, HIGH);
  delay(100);
  digitalWrite(RED_F, LOW);
  digitalWrite(YELLOW_F, HIGH);
  delay(100);
  digitalWrite(YELLOW_F, LOW);
  digitalWrite(GREEN_F, HIGH);
  delay(100);
  digitalWrite(GREEN_F, LOW);
  digitalWrite(BLUE_F, HIGH);
  delay(100);
  digitalWrite(BLUE_F, LOW);
  delay(1000);

  ColorSensors();
}

