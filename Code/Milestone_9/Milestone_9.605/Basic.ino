
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
  current = millis();
  int endTime = current + 2500;
  while (endTime > current) {
    ColorSensors();
    DriveForward();
    current = millis();
  }
  current = millis();
  endTime = current + 1200;
  while (endTime > current) {
    ColorSensors();
    LeftTurn();
    current = millis();
  }
  UpdateWheels(0, 0);
  delay(50);
}

void Fwd_RightTurn() {
  current = millis();
  int endTime = current + 2500;
  while (endTime > current) {
    ColorSensors();
    DriveForward();
    current = millis();
  }
  current = millis();
  endTime = current + 1200;
  while (endTime > current) {
    ColorSensors();
    RightTurn();
    current = millis();
  }
  UpdateWheels(0, 0);
  delay(50);
}

void Back_LeftTurn() {
  current = millis();
  int endTime = current + 3000;
  while (endTime > current) {
    ColorSensors();
    DriveReverse();
    current = millis();
  }
  current = millis();
  endTime = current + 1200;
  while (endTime > current) {
    ColorSensors();
    LeftTurn();
    current = millis();
  }
  UpdateWheels(0, 0);
  delay(50);
}


void Back_RightTurn() {
  current = millis();
  int endTime = current + 3000;
  while (endTime > current) {
    ColorSensors();
    DriveReverse();
    current = millis();
  }
  current = millis();
  endTime = current + 1200;
  while (endTime > current) {
    ColorSensors();
    RightTurn();
    current = millis();
  }
  UpdateWheels(0, 0);
  delay(50);
}

void Grab() {
  UpdateWheels(baseSpeed, baseSpeed);
  delay(500);
  UpdateArms(100, 100);
  delay(50);
  //  UpdateWheels(0, 0);
  //  delay(100);


  //  digitalWrite(RED_R, HIGH);
  //  delay(1000);
  //  digitalWrite(RED_R, LOW);
}

void Release() {
  digitalWrite(BLUE_F, HIGH);
  digitalWrite(YELLOW_F, HIGH);
  current = millis();
  int endTime = current + releaseInterval;
  while (endTime > current) {
    ColorSensors();
    DriveForward();
    current = millis();
  }
  UpdateWheels(0, 0);
  UpdateArms(0, 0);
  delay(50);

  current = millis();
  endTime = current + backupInterval;
  while (endTime > current) {
    ColorSensors();
    DriveReverse();
    current = millis();
  }
  UpdateWheels(0, 0);
  UpdateArms(0, 0);
  delay(50);

  current = millis();
  endTime = current + turnInterval;
  while (endTime > current) {
    ColorSensors();
    RightTurn();
    current = millis();
  }
  UpdateWheels(0, 0);
  UpdateArms(0, 0);
  delay(50);
  digitalWrite(BLUE_F, LOW);
  maxSig = 6;
  captureB = 0;
  UpdateArms(0, 0);
  Status = 1;
  digitalWrite(BLUE_F, LOW);
  digitalWrite(YELLOW_F, LOW);
  digitalWrite(RED_F, LOW);
  digitalWrite(GREEN_F, LOW);
}

/*
  for (int i = 0; i < 3; i++) {
    int current = millis();
    int endTime;
    if (i == 0) {
      digitalWrite(BLUE_F, HIGH);
      endTime = current + releaseInterval;
      L_wheel = baseSpeed;
      R_wheel = baseSpeed;
    }
    else if (i == 1) {
      endTime = current + backupInterval;
      L_wheel = -baseSpeed;
      R_wheel = -baseSpeed;
    }
    else {
      endTime = current + turnInterval;
      L_wheel = baseSpeed;
      R_wheel = -baseSpeed;
    }
    while (endTime > current) {
      ColorSensors();
      UpdateWheels(L_wheel, R_wheel);
      current2 = millis();
    }
    UpdateWheels(0, 0);
    UpdateArms(0, 0);
    delay(50);
  }
  digitalWrite(BLUE_F, LOW);
  maxSig = 6;
  captureB = 0;
  UpdateArms(0, 0);
  Status = 1;
  }
*/

/*void Release() {
  UpdateWheels(highSpeed, highSpeed);
  delay(1500);
  UpdateWheels(0, 0);
  delay(100);
  UpdateArms(0, 0);
  UpdateWheels(-highSpeed, -highSpeed);
  delay(1000);
  UpdateArms(100, 100);
  UpdateWheels(-highSpeed, highSpeed);
  delay(1000);
  UpdateWheels(0, 0);
  delay(10);

  maxSig = 6;
  captureB = 0;
  UpdateArms(0, 0);
  Status = 1;
  }*/


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
