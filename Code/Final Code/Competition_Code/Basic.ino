
void DriveForward() {
  UpdateWheels(baseSpeedL, baseSpeedR);
}


void DriveForwardHigh() {
  UpdateWheels(highSpeedL, highSpeedR);
}


void DriveReverse() {
  UpdateWheels(-baseSpeedL, -baseSpeedR);
}


void LeftTurn() {
  UpdateWheels(-baseSpeedL, baseSpeedR);
}


void RightTurn() {
  UpdateWheels(baseSpeedL, -baseSpeedR);
}


void DriveStop() {
  UpdateWheels(0, 0);
}


void Fwd_LeftTurn() {
  for (int i = 0; i < 120; i++) {
    ColorSensors();
    DriveForward();
  }
  for (int i = 0; i < 45; i++) {
    ColorSensors();
    LeftTurn();
  }
  UpdateWheels(0, 0);
  delay(50);
}


void Fwd_RightTurn() {
  for (int i = 0; i < 120; i++) {
    ColorSensors();
    DriveForward();
  }
  for (int i = 0; i < 45; i++) {
    ColorSensors();
    RightTurn();
  }
  UpdateWheels(0, 0);
  delay(50);
}


void Back_LeftTurn() {
  for (int i = 0; i < 100; i++) {
    ColorSensors();
    DriveReverse();
  }
  for (int i = 0; i < 45; i++) {
    ColorSensors();
    LeftTurn();
  }
  UpdateWheels(0, 0);
  delay(50);
}


void Back_RightTurn() {
  for (int i = 0; i < 100; i++) {
    ColorSensors();
    DriveReverse();
  }
  for (int i = 0; i < 45; i++) {
    ColorSensors();
    RightTurn();
  }
  UpdateWheels(0, 0);
  delay(50);
}


void Grab() {
  for (int i = 0; i < 60; i++) {
    UpdateWheels((baseSpeedL - 10) , (baseSpeedR - 10));
    ColorSensors();
  }
  UpdateArms(100, 100);
  delay(50);
}


void Release() {
  for (int i = 0; i < 175; i++) {
    ColorSensors();
    DriveForward();
  }
  UpdateWheels(0, 0);
  UpdateArms(0, 0);
  delay(50);
  for (int i = 0; i < 25; i++) {
    ColorSensors();
    DriveReverse();
  }
  for (int i = 0; i < 25; i++) { // should be a 50 degree right turn. Used to be 80 for 180 degree turn
    ColorSensors();
    RightTurn();
  }
  maxSig = 6;
  captureB = 0;
  UpdateArms(0, 0);
  Status = 1;
}

