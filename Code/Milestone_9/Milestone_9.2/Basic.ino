
void DriveForward() {
  speedL = baseSpeedL;
  speedR = baseSpeedR;
  MotorUpdate(speedL, speedR);
}

void DriveReverse() {
  speedL = -baseSpeedL;
  speedR = -baseSpeedR;
  MotorUpdate(speedL, speedR);
}


void LeftTurn() {
  speedL = -baseSpeedL;
  speedR = baseSpeedR;
  MotorUpdate(speedL, speedR);
}


void RightTurn() {
  speedL = 50;
  speedR = -50;
  MotorUpdate(speedL, speedR);
}


void DriveStop() {
  speedL = 0;
  speedR = 0;
  MotorUpdate(speedL, speedR);
}


void Grab() {
  MotorUpdate(baseSpeedL, baseSpeedR);
  delay(500);
  UpdateArms(100, 100);
  delay(50);
  MotorUpdate(0, 0);
  delay(500);
  MotorUpdate(baseSpeedL, -baseSpeedR);
  delay(1000);
  MotorUpdate(0, 0);
  delay(100);


  //  digitalWrite(RED_R, HIGH);
  //  delay(1000);
  //  digitalWrite(RED_R, LOW);
}


void Release() {
  MotorUpdate(baseSpeedL, baseSpeedR);
  delay(3000);
  MotorUpdate(0, 0);
  delay(100);
  UpdateArms(0, 0);
  delay(50);
  MotorUpdate(-baseSpeedL, -baseSpeedR);
  delay(1000);
  MotorUpdate(-baseSpeedL, baseSpeedR);
  delay(1000);
  MotorUpdate(0, 0);
  delay(500);
  UpdateArms(100, 100);
  maxSig = 6;

  //  digitalWrite(BLUE_R, HIGH);
  //  delay(1000);
  //  digitalWrite(BLUE_R, LOW);
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
