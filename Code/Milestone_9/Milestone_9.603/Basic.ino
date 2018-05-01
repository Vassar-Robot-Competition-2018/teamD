
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
  DriveForward();
  delay(1500);
  LeftTurn();
  delay(1000);
  DriveStop();
  delay(50);
}

void Fwd_RightTurn() {
  DriveForward();
  delay(1500);
  RightTurn();
  delay(1000);
  DriveStop();
  delay(50);
}

void Back_LeftTurn() {
  DriveReverse();
  delay(1500);
  LeftTurn();
  delay(1000);
  DriveStop();
  delay(50);
}


void Back_RightTurn() {
  DriveReverse();
  delay(1500);
  RightTurn();
  delay(1000);
  DriveStop();
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
