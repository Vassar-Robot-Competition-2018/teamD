
void DriveForward() {
  UpdateWheels(baseSpeed, baseSpeed);
}

void DriveReverse() {
  UpdateWheels(-baseSpeed, -baseSpeed);
}


void LeftTurn() {
  UpdateWheels(-baseSpeed, baseSpeed);
}


void RightTurn() {
  UpdateWheels(baseSpeed, -baseSpeed);
}


void DriveStop() {
  UpdateWheels(0, 0);
}

void Fwd_LeftTurn() {
  for (int i = 0; i < 90; i++) {
    ColorSensors();
    DriveForward();
  }
  for (int i = 0; i < 50; i++) {
    ColorSensors();
    LeftTurn();
  }
  UpdateWheels(0, 0);
  delay(50);
}

void Fwd_RightTurn() {
  for (int i = 0; i < 90; i++) {
    ColorSensors();
    DriveForward();
  }
  for (int i = 0; i < 50; i++) {
    ColorSensors();
    RightTurn();
  }
  UpdateWheels(0, 0);
  delay(50);
}

void Back_LeftTurn() {
  for (int i = 0; i < 90; i++) {
    ColorSensors();
    DriveReverse();
  }
  for (int i = 0; i < 50; i++) {
    ColorSensors();
    LeftTurn();
  }
  UpdateWheels(0, 0);
  delay(50);
}


void Back_RightTurn() {
  for (int i = 0; i < 90; i++) {
    ColorSensors();
    DriveReverse();
  }
  for (int i = 0; i < 50; i++) {
    ColorSensors();
    RightTurn();
  }
  UpdateWheels(0, 0);
  delay(50);
}

void Grab() {
  UpdateWheels(baseSpeed, baseSpeed);
  delay(500);
  UpdateArms(100, 100);
  delay(50);
}

void Release() {
  digitalWrite(BLUE_F, HIGH);
  digitalWrite(YELLOW_F, HIGH);
  for (int i = 0; i < 100; i++) {
    ColorSensors();
    DriveForward();
  }
  UpdateWheels(0, 0);
  UpdateArms(0, 0);
  delay(50);
  for (int i = 0; i < 100; i++) {
    ColorSensors();
    DriveReverse();
  }
  for (int i = 0; i < 50; i++) {
    ColorSensors();
    RightTurn();
  }
  delay(50);
   maxSig = 6;
  captureB = 0;
  UpdateArms(0, 0);
  Status = 1;
  digitalWrite(BLUE_F, LOW);
  digitalWrite(YELLOW_F, LOW);
}

void CenterCam()
{
  if (servoPos > 3) {
    servoPos -= 3;
    UpdatePan(servoPos);
  }
  else if (servoPos < -3) {
    servoPos += 3;
    UpdatePan(servoPos);
  }
  else if (servoPos > 1) {
    servoPos -= 1;
    UpdatePan(servoPos);
  }
  else if (servoPos < -1) {
    servoPos += 1;
    UpdatePan(servoPos);
  }
  delay(20);
}
