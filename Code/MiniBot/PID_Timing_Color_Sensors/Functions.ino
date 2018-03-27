// Functions

void ColorSensorL() { // checks floor color sensor (up to 20 times per second)
  //uint16_t clear, red, green, blue;
  int clear, red, green, blue;

  tcsL.setInterrupt(false);      // turn on LED
  tcsL.getRawData(&red, &green, &blue, &clear);
  tcsL.setInterrupt(true);  // turn off LED

  Serial.print("Left: C = "); Serial.print(clear);
  Serial.print("; R = "); Serial.print(red);
  Serial.print("; G = "); Serial.print(green);
  Serial.print("; B = "); Serial.print(blue);

  if (LC_gray == 0) {
    L_ColorCalibration(clear, red, green, blue);
  }

  WhiteCheck(clear, LC_gray, LEFT);
  Quadrant(red, green, blue);
}


void ColorSensorR() { // checks floor color sensor (up to 20 times per second)
  //uint16_t clear, red, green, blue;
  int clear, red, green, blue;

  tcsR.setInterrupt(false);      // turn on LED
  //delay(60);  // takes 50ms to read
  tcsR.getRawData(&red, &green, &blue, &clear);
  tcsR.setInterrupt(true);  // turn off LED

  Serial.print("     Right: C = "); Serial.print(clear);
  Serial.print("; R = "); Serial.print(red);
  Serial.print("; G = "); Serial.print(green);
  Serial.print("; B = "); Serial.println(blue);

  if (RC_gray == 0) {
    R_ColorCalibration(clear, red, green, blue);
  }
  WhiteCheck(clear, RC_gray, RIGHT);
  Quadrant(red, green, blue);
}


void ColorSensor() { // checks floor color sensor (up to 20 times per second)
  ColorSensorL();
  ColorSensorR();
}


void SetHome(int r, int g, int b) { // sets home quadrant
  if ((homeQuad == 0) && (quadrant != 0)) {
    homeQuad = quadrant;
  }
}


void Quadrant(int r, int g, int b) {
  if (r > (g + b))  //red
    quadrant = 1;

  else if ((r + g) > (4 * b))  //yellow
    quadrant = 2;

  else if ((3 * g) > ((r + b) * 2))  //green
    quadrant = 3;

  else if (b > (2 * r))  //blue
    quadrant = 4;

  SetHome(r, g, b);

  if (quadrant != tempQuad) {
    prevQuad = tempQuad;
    tempQuad = quadrant;
  }

  //  Serial.print("Home Quadrant: ");
  //  Serial.print(homeQuad);
  //  Serial.print("Quadrant color: ");
  //  Serial.println(quadrant);

  QuadrantLED();
  HomeLED();
}


void L_ColorCalibration(int c, int r, int g, int b) {
  LC_gray = c;
  LR_gray = r;
  LG_gray = g;
  LB_gray = b;

  Serial.print("Left Color calibration complete");
}


void R_ColorCalibration(int c, int r, int g, int b) {
  RC_gray = c;
  RR_gray = r;
  RG_gray = g;
  RB_gray = b;

  Serial.print("Right Color calibration complete");
}


void WhiteCheck(int c, int C_gray, int side) {
  if (c > (4 * C_gray)) {  // maybe 4*Cgray when white tape is used
    Serial.println("White Detected");
    digitalWrite(WHITE, HIGH);    // LED

    UpdateWheels(0, 0);
    delay(100);
    UpdateWheels(-baseSpeedL, -baseSpeedR);
    delay(1000);

    if (side == 1) {
      UpdateWheels(baseSpeedL, -baseSpeedR);
      delay(1000);
    }
    else if (side == 2) {
      UpdateWheels(-baseSpeedL, baseSpeedR);
      delay(1000);
    }

    UpdateWheels(0, 0);
    delay(100);
  }

  else {
    digitalWrite(WHITE, LOW);    // LED
  }
}


void HomeLED() {
  if (homeQuad == 1) {
    //Serial.println("Red");
    digitalWrite(RED_L, HIGH);    // LED
    digitalWrite(YELLOW_L, LOW);    // LED
    digitalWrite(GREEN_L, LOW);    // LED
    digitalWrite(BLUE_L, LOW);    // LED
  }

  else if (homeQuad == 2) {
    //Serial.println("Yellow");
    digitalWrite(RED_L, LOW);    // LED
    digitalWrite(YELLOW_L, HIGH);    // LED
    digitalWrite(GREEN_L, LOW);    // LED
    digitalWrite(BLUE_L, LOW);    // LED
  }

  else if (homeQuad == 3) {
    //Serial.println("Green");
    digitalWrite(RED_L, LOW);    // LED
    digitalWrite(YELLOW_L, LOW);    // LED
    digitalWrite(GREEN_L, HIGH);    // LED
    digitalWrite(BLUE_L, LOW);    // LED
  }

  else if (homeQuad == 4) {
    //Serial.println("Blue");
    digitalWrite(RED_L, LOW);    // LED
    digitalWrite(YELLOW_L, LOW);    // LED
    digitalWrite(GREEN_L, LOW);    // LED
    digitalWrite(BLUE_L, HIGH);    // LED
  }

  else {
    digitalWrite(RED_L, LOW);    // LED
    digitalWrite(YELLOW_L, LOW);    // LED
    digitalWrite(GREEN_L, LOW);    // LED
    digitalWrite(BLUE_L, LOW);    // LED
  }
}


void QuadrantLED() {
  if (quadrant == 1) {
    //Serial.println("Red");
    digitalWrite(RED_R, HIGH);    // LED
    digitalWrite(YELLOW_R, LOW);    // LED
    digitalWrite(GREEN_R, LOW);    // LED
    digitalWrite(BLUE_R, LOW);    // LED
  }

  else if (quadrant == 2) {
    //Serial.println("Yellow");
    digitalWrite(RED_R, LOW);    // LED
    digitalWrite(YELLOW_R, HIGH);    // LED
    digitalWrite(GREEN_R, LOW);    // LED
    digitalWrite(BLUE_R, LOW);    // LED
  }

  else if (quadrant == 3) {
    //Serial.println("Green");
    digitalWrite(RED_R, LOW);    // LED
    digitalWrite(YELLOW_R, LOW);    // LED
    digitalWrite(GREEN_R, HIGH);    // LED
    digitalWrite(BLUE_R, LOW);    // LED
  }

  else if (quadrant == 4) {
    //Serial.println("Blue");
    digitalWrite(RED_R, LOW);    // LED
    digitalWrite(YELLOW_R, LOW);    // LED
    digitalWrite(GREEN_R, LOW);    // LED
    digitalWrite(BLUE_R, HIGH);    // LED
  }

  else {
    digitalWrite(RED_R, LOW);    // LED
    digitalWrite(YELLOW_R, LOW);    // LED
    digitalWrite(GREEN_R, LOW);    // LED
    digitalWrite(BLUE_R, LOW);    // LED
  }
}


void BlockColor () {
  if (maxSig == 1) {
    //Serial.println("Red");
    digitalWrite(RED_R, HIGH);    // LED
    digitalWrite(YELLOW_R, LOW);    // LED
    digitalWrite(GREEN_R, LOW);    // LED
    digitalWrite(BLUE_R, LOW);    // LED
  }

  else if (maxSig == 2) {
    //Serial.println("Yellow");
    digitalWrite(RED_R, LOW);    // LED
    digitalWrite(YELLOW_R, HIGH);    // LED
    digitalWrite(GREEN_R, LOW);    // LED
    digitalWrite(BLUE_R, LOW);    // LED
  }

  else if (maxSig == 3) {
    //Serial.println("Green");
    digitalWrite(RED_R, LOW);    // LED
    digitalWrite(YELLOW_R, LOW);    // LED
    digitalWrite(GREEN_R, HIGH);    // LED
    digitalWrite(BLUE_R, LOW);    // LED
  }

  else if (maxSig == 4) {
    //Serial.println("Blue");
    digitalWrite(RED_R, LOW);    // LED
    digitalWrite(YELLOW_R, LOW);    // LED
    digitalWrite(GREEN_R, LOW);    // LED
    digitalWrite(BLUE_R, HIGH);    // LED
  }
  else {
    digitalWrite(RED_R, LOW);    // LED
    digitalWrite(YELLOW_R, LOW);    // LED
    digitalWrite(GREEN_R, LOW);    // LED
    digitalWrite(BLUE_R, LOW);    // LED
  }
}


void CheckBlocks() {
  // for (int k = 0; k < 10; k++) {
  static int i = 0;
  uint16_t blocks;
  char buf[32];

  maxJ = 0;
  maxSig = 6;
  max_X = camSP;
  max_Y = 0;
  maxWidth = 0;
  maxHeight = 0;
  prod = 0;
  maxProd = 1000;

  //  if (homeQuad != 0) {
  blocks = pixy.getBlocks();

  if (blocks) { //if a color sig is detected by pixy cam
    for (j = 0; j < blocks; j++) { //find the largest signature
      //        if (pixy.blocks[j].signature == homeQuad) {
      //    digitalWrite(WHITE, HIGH);
      prod = pixy.blocks[j].width * pixy.blocks[j].height;
      if (prod > maxProd) {
        maxProd = prod;
        maxJ = j;

        maxSig = pixy.blocks[maxJ].signature;
        max_X = pixy.blocks[maxJ].x;
        max_Y = pixy.blocks[maxJ].y;
        maxHeight = pixy.blocks[maxJ].height;
        maxWidth = pixy.blocks[maxJ].width;
        Serial.println(maxSig);
      }
      //        }
    }
    // Serial.println(maxJ);

    //    Serial.print("Block Position: ");
    //    Serial.print(max_X);
    //    Serial.print("     ");
    //    Serial.print("Block Width: ");
    //    Serial.print(maxWidth);
    //    Serial.print(" x ");
    //    Serial.println(maxWidth);

    if (maxSig != 6) {
      digitalWrite(WHITE, HIGH);    // LED

      TrackBlock();
      FollowBlock();
    }

    else {
      digitalWrite(WHITE, LOW);    // LED
      //CenterCam();
      Scan();
      DriveForward();
      //Serial.println("None");

      //    digitalWrite(YELLOW, LOW);    // LED
      //    digitalWrite(GREEN, LOW);    // LED
      //    digitalWrite(BLUE, LOW);    // LED

      //DriveForward();
      //      digitalWrite(GREEN, HIGH);
      //      digitalWrite(YELLOW, LOW);
    }
  }

  else {
    digitalWrite(WHITE, LOW);    // LED
    //CenterCam();
    Scan();
    DriveForward();
  }
}


void Grab() {
  UpdateWheels(baseSpeedL, baseSpeedR);
  delay(500);
  UpdateArms(100, 100);
  delay(50);
  UpdateWheels(0, 0);
  delay(500);
  UpdateWheels(baseSpeedL, -baseSpeedR);
  delay(1000);
  UpdateWheels(0, 0);
  delay(100);


  //  digitalWrite(RED_R, HIGH);
  //  delay(1000);
  //  digitalWrite(RED_R, LOW);
}


void Release() {
  UpdateWheels(0, 0);
  delay(100);
  UpdateArms(0, 0);
  delay(50);
  UpdateWheels(-baseSpeedL, -baseSpeedR);
  delay(1000);
  UpdateWheels(-baseSpeedL, baseSpeedR);
  delay(1000);
  UpdateWheels(0, 0);
  delay(100);

  //  digitalWrite(BLUE_R, HIGH);
  //  delay(1000);
  //  digitalWrite(BLUE_R, LOW);
}


void DriveForward() {
  speedL = baseSpeedL;
  speedR = baseSpeedR;
  UpdateWheels(speedL, speedR);
  //delay(20);
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

void TrackBlock()
{
  /*How long since we last calculated*/
  current = millis();
  trackTimeChange = (double)(current - trackPrevTime);
  trackError = camSP - max_X;
  if (trackTimeChange > 100) {
    trackErrSum = 0;
    trackdErr = 0;
  }
  else {
    /*Compute all the working error variables*/
    trackErrSum += (trackError * trackTimeChange);
    trackdErr = (trackError - trackPrevErr) / trackTimeChange;
  }
  /*Compute PID Output*/
  servoAdj = trackKp * trackError + trackKi * trackErrSum + trackKd * trackdErr;

  /*Remember some variables for next time*/
  trackPrevErr = trackError;
  trackPrevTime = current;

  servoPos = (int)constrain(servoPos + servoAdj, servoMin, servoMax);

  current = millis();
  if ((current - lastCamUpdate) > camInterval) // time to update
  {
    lastCamUpdate = millis();
    UpdatePan(servoPos);
  }



  //UpdatePan(servoPos);
  //  delay(20);    // 20ms works well
}


void FollowBlock()
{
  /*How long since we last calculated*/
  current = millis();
  followTimeChange = (double)(current - followPrevTime);
  followError = servoSP - servoPos;
  if (followTimeChange > 100) {
    followErrSum = 0;
    followdErr = 0;
  }
  else {
    /*Compute all the working error variables*/
    followErrSum += (followError * followTimeChange);
    followdErr = (followError - followPrevErr) / followTimeChange;
  }
  /*Compute PID Output*/
  wheelAdj = followKp * followError + followKi * followErrSum + followKd * followdErr;

  /*Remember some variables for next time*/
  followPrevErr = followError;
  followPrevTime = current;
  //if(wheelAdj > 0) {
  //    speedL = (int)constrain(baseSpeedL + 0.5*wheelAdj, minSpeed, maxSpeed);
  //  speedR = (int)constrain(baseSpeedR - wheelAdj, minSpeed, maxSpeed);
  //}
  //else if(wheelAdj < 0) {
  //    speedL = (int)constrain(baseSpeedL + wheelAdj, minSpeed, maxSpeed);
  //  speedR = (int)constrain(baseSpeedR - 0.5*wheelAdj, minSpeed, maxSpeed);
  //}
  speedL = (int)constrain(baseSpeedL + wheelAdj, minSpeed, maxSpeed);
  speedR = (int)constrain(baseSpeedR - wheelAdj, minSpeed, maxSpeed);

  //    Serial.print("Follow Error: ");
  //    Serial.println(followError);

  Serial.print("L speed: ");
  Serial.print(speedL);
  Serial.print(",    R speed: ");
  Serial.println(speedR);

  current = millis();
  if ((current - lastWheelUpdate) > wheelInterval) // time to update
  {
    lastWheelUpdate = millis();
    UpdateWheels(speedL, speedR);
  }


  //  UpdateWheels(speedL, speedR);
  //  delay(20);
}


void Scan() {
  current = millis();
  if ((current - lastScanUpdate) > scanInterval) // time to update
  {
    lastScanUpdate = millis();
    servoPos += increment;
    UpdatePan(servoPos);
    //delay(20);

    //Serial.println(servoPos);
    if ((servoPos >= servoMax) || (servoPos <= servoMin)) // end of sweep
    {
      // reverse direction
      increment = -increment;
    }
  }
}


void UpdatePan(double pos) {
  double pan_pulse = map(pos, -100, 100, PAN_MIN, PAN_MAX); // straight
  pwm.setPWM(PAN_SERVO, 0, pan_pulse);
  // delay(20);
}


void UpdateArms(int L, int R) {
  int l_arm_pulse = map(L, 0, 100, L_ARM_MIN, L_ARM_MAX); // 0 is open, 100 is closed
  int r_arm_pulse = map(R, 100, 0, R_ARM_MIN, R_ARM_MAX); // 0 is open, 100 is closed
  pwm.setPWM(L_ARM, 0, l_arm_pulse);
  pwm.setPWM(R_ARM, 0, r_arm_pulse);
}


void UpdateWheels(double L, double R) {
  double l_wheel_pulse = map(L, -100, 100, L_WHEEL_MAX, L_WHEEL_MIN); // -100 is full reverse, 100 is full forward
  double r_wheel_pulse = map(R, -100, 100, R_WHEEL_MIN, R_WHEEL_MAX); // 0 is stopped
  pwm.setPWM(L_WHEEL, 0, l_wheel_pulse);
  pwm.setPWM(R_WHEEL, 0, r_wheel_pulse);
  //  delay(20);
}


//void TOF() {
//  unsigned long sumTOF;
//  int j;
//
//  for (int i = 0; i < 1; i++) {
//    VL53L0X_RangingMeasurementData_t measure;
//    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
//
//    // phase failures have incorrect data
//    if ((measure.RangeStatus != 4) && ((measure.RangeMilliMeter > 50) && (measure.RangeMilliMeter < 1200))) {
//      sumTOF += measure.RangeMilliMeter;
//      j++;
//    }
//
//    //    else {
//    //      Serial.println(" out of range ");
//    //    }
//
//   // delay(40);
//  }
//
//  //if ((sumTOF > 50) && (sumTOF < 15000)) {
//  if (j > 0) {
//    TOF_distance = sumTOF / j;
//
//    Serial.print(TOF_distance);
//    Serial.print("        ");
//    Serial.println(j);
//  }
//  else {
//    TOF_distance = 20000;
//
//    Serial.println(TOF_distance);
//  }
//}

void TOF() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if ((measure.RangeStatus != 4) && ((measure.RangeMilliMeter > 50) && (measure.RangeMilliMeter < 1200))) {
    TOF_distance = measure.RangeMilliMeter;

    if (abs(raw_dist - prev_dist) > 70) {
      temp_dist = TOF_distance;
      TOF_distance = prev_dist;
      prev_dist = (temp_dist + prev_dist) / 2;
    }
    else {
      prev_dist = TOF_distance;
    }
  }

  else {
    TOF_distance = 1200;
  }
  Serial.println(TOF_distance);
}

