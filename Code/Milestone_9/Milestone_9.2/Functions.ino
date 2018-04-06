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

  if (clear == 0) {
    tcsL.begin();  // initializes left color sensor
    Serial.print("RESET");
  }

  if (LC_gray == 0) {
    L_ColorCalibration(clear, red, green, blue);
  }
  else {
    WhiteCheck(clear, LC_gray);
    Quadrant(red, green, blue);
  }
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

  if (clear == 0) {
    tcsR.begin();  // initializes left color sensor
    Serial.print("RESET");
  }

  if (RC_gray == 0) {
    R_ColorCalibration(clear, red, green, blue);
  }
  else {
    WhiteCheck(clear, RC_gray);
    Quadrant(red, green, blue);
  }
}

void ColorSensor() { // checks floor color sensor (up to 20 times per second)
  ColorSensorL();
  ColorSensorR();
}


void SetHome(int r, int g, int b) { // sets home quadrant
  if ((homeQuad == 5) && (quadrant != 0)) {
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


void WhiteCheck(int c, int C_gray) {
  if (c > (4 * C_gray)) {  // maybe 4*Cgray when white tape is used
    Serial.println("White Detected");
    digitalWrite(WHITE, HIGH);    // LED

    DriveReverse();
    delay(1000);

    RightTurn();
    delay(1000);
  }
  else {
    digitalWrite(WHITE, LOW);    // LED
  }
}


void MotorUpdate(double L, double R) {
  double servoL_speed = (L * (-0.9)) + 92;
  double servoR_speed = (R * 0.9 ) + 92;
  servoL.write(servoL_speed);
  servoR.write(servoR_speed);
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


void IR_Short() {
  double ShortPrev;
  double ShortSum;

  for (int i = 0; i <= 10; i++) {
    double ShortRaw = analogRead(IR_SHORT);
    if (abs(ShortRaw - ShortPrev) > 100) {
      double ShortTemp = ShortRaw;
      ShortRaw = ShortPrev;
      ShortPrev = ShortTemp;
    }
    else {
      ShortPrev = ShortRaw;
    }
    ShortSum += ShortRaw;

    if (i == 9) {
      double ShortAvg = ShortSum / 10;
      //Serial.println(avg);
      ShortIR = 3040.4 / ShortAvg - 1.18;
      if (ShortIR > 25) {
        ShortIR = 100;
      }

      Serial.print("Short IR: ");
      Serial.println();
      //   delay(100);
      ShortSum = 0;
    }
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
  maxProd = 0;

  if (homeQuad != quadrant) {
    blocks = pixy.getBlocks();

    if (blocks) { //if a color sig is detected by pixy cam
      for (j = 0; j < blocks; j++) { //find the largest signature
        if (pixy.blocks[j].signature == homeQuad) {
          prod = pixy.blocks[j].width * pixy.blocks[j].height;
          // if (prod > maxProd) {
          if ((pixy.blocks[j].height > 20) && (pixy.blocks[j].height < 125)) {
            if ((pixy.blocks[j].height > 20) && (pixy.blocks[j].height < 125)) {
              //  maxProd = prod;
              maxJ = j;
              maxSig = pixy.blocks[maxJ].signature;
              max_X = pixy.blocks[maxJ].x;
              max_Y = pixy.blocks[maxJ].y;
              maxHeight = pixy.blocks[maxJ].height;
              maxWidth = pixy.blocks[maxJ].width;
            }
          }
        }
      }
      //    Serial.print("Block Position: ");
      //    Serial.print(max_X);
      //    Serial.print("     ");
      //    Serial.print("Block Width: ");
      //    Serial.print(maxWidth);
      //    Serial.print(" x ");
      //    Serial.println(maxWidth);

      //    digitalWrite(WHITE, HIGH);    // LED
      FollowBlock();
      if(maxSig == homeQuad){
        UpdateArms(0, 0);
      }
    }

    else {
      //   digitalWrite(WHITE, LOW);
      DriveForward();
      if (maxSig != homeQuad) {
        UpdateArms(100, 100);
      }
    }
  }
  else {
    //   digitalWrite(WHITE, LOW);
    DriveForward();
  }
}

void FollowBlock()
{
  //  if (maxSig == homeQuad) {
  //    camSP = 160;
  //  }
  //  else {
  //    camSP = 0;
  //  }


  /*How long since we last calculated*/
  current = millis();
  timeChange = (double)(current - prevTime);
  camError = camSP - max_X;
  if (timeChange > 100) {
    errSum = 0;
    dErr = 0;
  }
  else {
    /*Compute all the working error variables*/
    errSum += (camError * timeChange);
    dErr = (camError - prevErr) / timeChange;
  }
  /*Compute PID Output*/
  wheelAdj = kp * camError + ki * errSum + kd * dErr;

  /*Remember some variables for next time*/
  prevErr = camError;
  prevTime = current;

  speedL = (int)constrain(baseSpeedL + wheelAdj, minSpeed, maxSpeed);
  speedR = (int)constrain(baseSpeedR - wheelAdj, minSpeed, maxSpeed);

  //  Serial.print("L speed: ");
  //  Serial.print(speedL);
  //  Serial.print(",    R speed: ");
  //  Serial.println(speedR);

  current = millis();
  if ((current - lastWheelUpdate) >= wheelInterval) // time to update
  {
    lastWheelUpdate = millis();
    MotorUpdate(speedL, speedR);
  }
}


boolean Captured() {
  if (maxSig != 6) {
    //   if (((maxHeight > 80)  && (maxHeight < 125)) && ((maxWidth > 105)  && (maxWidth <  150))) {
    if ((maxHeight > 80)  && (maxWidth > 105)) {
      return true;
    }
    else return false;
  }
  else return false;
}


