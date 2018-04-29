
void DriveForward() {
  speedL = highSpeed;
  speedR = highSpeed;
  UpdateWheels(speedL, speedR);
}

void DriveReverse() {
  speedL = -highSpeed;
  speedR = -highSpeed;
  UpdateWheels(speedL, speedR);
}


void LeftTurn() {
  speedL = -highSpeed;
  speedR = highSpeed;
  UpdateWheels(speedL, speedR);
}


void RightTurn() {
  speedL = highSpeed;
  speedR = -highSpeed;
  UpdateWheels(speedL, speedR);
}


void DriveStop() {
  speedL = 0;
  speedR = 0;
  UpdateWheels(speedL, speedR);
}


void Grab() {
  UpdateWheels(baseSpeedL, baseSpeedR);
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
