
void DriveForward() {
  UpdateWheels(highSpeed, highSpeed);
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


void FWD_LeftTurn() {
  
current = millis();
    endTime = current + 2500;
    while (endTime > current) {
      ColorSensors();
      DriveForward();
      current = millis();
    }

    current = millis();
    endTime = current + 1000;
    while (endTime > current) {
      ColorSensors();
      LeftTurn();
      current = millis();
    }
    UpdateWheels(0, 0);
    delay(50);
}


void FWD_RightTurn() {
  
current = millis();
    endTime = current + 2500;
    while (endTime > current) {
      ColorSensors();
      DriveForward();
      current = millis();
    }

    current = millis();
    endTime = current + 1000;
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
    endTime = current + 2500;
    while (endTime > current) {
      ColorSensors();
      DriveReverse();
      current = millis();
    }

    current = millis();
    endTime = current + 1000;
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
    endTime = current + 2500;
    while (endTime > current) {
      ColorSensors();
      DriveReverse();
      current = millis();
    }

    current = millis();
    endTime = current + 1000;
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
  delay(700);
  UpdateArms(100, 100);
  delay(50);
}


void Release() {
  for (i = 0; i < 3; i++) {
    current = millis();
    if (i == 0) {
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
      current = millis();
    }
    UpdateWheels(0, 0);
    UpdateArms(0, 0);
    delay(50);
  }
  maxSig = 6;
  Status = 1;
}

