// Functions


void ColorSensor() { // checks floor color sensor (up to 20 times per second)
  //uint16_t clear, red, green, blue;
  int clear, red, green, blue;

  tcs.setInterrupt(false);      // turn on LED
  tcs.getRawData(&red, &green, &blue, &clear);
  tcs.setInterrupt(true);  // turn off LED

  //  Serial.print("C:\t"); Serial.print(clear);
  //  Serial.print("\tR:\t"); Serial.print(red);
  //  Serial.print("\tG:\t"); Serial.print(green);
  //  Serial.print("\tB:\t"); Serial.println(blue);

  if (Cgray == 0) {
    ColorCalibration(clear, red, green, blue);
  }

  WhiteCheck(clear);
  Quadrant(red, green, blue);
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

  HomeQuadrantLED();
  //QuadrantLED();
}


void ColorCalibration(int c, int r, int g, int b) {
  Cgray = c;
  Rgray = r;
  Ggray = g;
  Bgray = b;

  Serial.print("Color calibration complete");
}


void WhiteCheck(int c) {
  if (c > (4 * Cgray)) {  // maybe 4*Cgray when white tape is used
    Serial.println("White Detected");

    DriveReverse();
    delay(1000);

    RightTurn();
    delay(1000);
  }
}


void HomeQuadrantLED() {
  if (homeQuad == 1) {
    //Serial.println("Red");
    digitalWrite(RED_F, HIGH);    // LED
    digitalWrite(YELLOW_F, LOW);    // LED
    digitalWrite(GREEN_F, LOW);    // LED
    digitalWrite(BLUE_F, LOW);    // LED
  }

  else if (homeQuad == 2) {
    //Serial.println("Yellow");
    digitalWrite(RED_F, LOW);    // LED
    digitalWrite(YELLOW_F, HIGH);    // LED
    digitalWrite(GREEN_F, LOW);    // LED
    digitalWrite(BLUE_F, LOW);    // LED
  }

  else if (homeQuad == 3) {
    //Serial.println("Green");
    digitalWrite(RED_F, LOW);    // LED
    digitalWrite(YELLOW_F, LOW);    // LED
    digitalWrite(GREEN_F, HIGH);    // LED
    digitalWrite(BLUE_F, LOW);    // LED
  }

  else if (homeQuad == 4) {
    //Serial.println("Blue");
    digitalWrite(RED_F, LOW);    // LED
    digitalWrite(YELLOW_F, LOW);    // LED
    digitalWrite(GREEN_F, LOW);    // LED
    digitalWrite(BLUE_F, HIGH);    // LED
  }
  else {
    digitalWrite(RED_F, LOW);    // LED
    digitalWrite(YELLOW_F, LOW);    // LED
    digitalWrite(GREEN_F, LOW);    // LED
    digitalWrite(BLUE_F, LOW);    // LED
  }
}

//void QuadrantLED() {
//  if (quadrant == 1) {
//    //Serial.println("Red");
//    digitalWrite(RED_F, HIGH);    // LED
//    digitalWrite(YELLOW_F, LOW);    // LED
//    digitalWrite(GREEN_F, LOW);    // LED
//    digitalWrite(BLUE_F, LOW);    // LED
//  }
//
//  else if (quadrant == 2) {
//    //Serial.println("Yellow");
//    digitalWrite(RED_F, LOW);    // LED
//    digitalWrite(YELLOW_F, HIGH);    // LED
//    digitalWrite(GREEN_F, LOW);    // LED
//    digitalWrite(BLUE_F, LOW);    // LED
//  }
//
//  else if (quadrant == 3) {
//    //Serial.println("Green");
//    digitalWrite(RED_F, LOW);    // LED
//    digitalWrite(YELLOW_F, LOW);    // LED
//    digitalWrite(GREEN_F, HIGH);    // LED
//    digitalWrite(BLUE_F, LOW);    // LED
//  }
//
//  else if (quadrant == 4) {
//    //Serial.println("Blue");
//    digitalWrite(RED_F, LOW);    // LED
//    digitalWrite(YELLOW_F, LOW);    // LED
//    digitalWrite(GREEN_F, LOW);    // LED
//    digitalWrite(BLUE_F, HIGH);    // LED
//  }
//  else {
//    digitalWrite(RED_F, LOW);    // LED
//    digitalWrite(YELLOW_F, LOW);    // LED
//    digitalWrite(GREEN_F, LOW);    // LED
//    digitalWrite(BLUE_F, LOW);    // LED
//  }
//}


//void BlockColor () {
//  if (maxSig == 1) {
//    //Serial.println("Red");
//    digitalWrite(RED, HIGH);    // LED
//    digitalWrite(YELLOW, LOW);    // LED
//    digitalWrite(GREEN, LOW);    // LED
//    digitalWrite(BLUE, LOW);    // LED
//  }
//
//  else if (maxSig == 2) {
//    //Serial.println("Yellow");
//    digitalWrite(RED, LOW);    // LED
//    digitalWrite(YELLOW, HIGH);    // LED
//    digitalWrite(GREEN, LOW);    // LED
//    digitalWrite(BLUE, LOW);    // LED
//  }
//
//  else if (maxSig == 3) {
//    //Serial.println("Green");
//    digitalWrite(RED, LOW);    // LED
//    digitalWrite(YELLOW, LOW);    // LED
//    digitalWrite(GREEN, HIGH);    // LED
//    digitalWrite(BLUE, LOW);    // LED
//  }
//
//  else if (maxSig == 4) {
//    //Serial.println("Blue");
//    digitalWrite(RED_B, LOW);    // LED
//    digitalWrite(YELLOW_B, LOW);    // LED
//    digitalWrite(GREEN_B, LOW);    // LED
//    digitalWrite(BLUE_B, HIGH);    // LED
//  }
//}


void DriveForward() {
  speedL = 50;
  speedR = 50;
  MotorUpdate(speedL, speedR);
}


void DriveReverse() {
  speedL = -50;
  speedR = -50;
  MotorUpdate(speedL, speedR);
}


void LeftTurn() {
  speedL = -50;
  speedR = 50;
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


void GrabBlock() {
  blockServoL.write(67);
  blockServoR.write(120);
  delay(2000);
}


void ReleaseBlock() {
  blockServoL.write(94);
  blockServoR.write(100);
  DriveReverse();
  delay(2000);
  RightTurn();
  delay(2000);
}


void HoldingBlock() {
  if (ShortIR <= CAPTURED)
    DriveForward();
}

//void BlockComplete() {
//  speedL = 0;
//  speedR = 0;
//  MotorUpdate(speedL, speedR);
//  BlockColor();
//  digitalWrite(WHITE, HIGH);    // LED
//  //  Serial.print("maxWidth: "); Serial.println(maxWidth);
//  //  Serial.print("PixyIR: "); Serial.println(PixyIR);
//  delay(5000);
//}


void MotorUpdate(double L, double R) {
  double servoL_speed = (L * (-0.9)) + 92;
  double servoR_speed = (R * 0.9 ) + 92;
  servoL.write(servoL_speed);
  servoR.write(servoR_speed);
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

  if (homeQuad != 5) {
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

      digitalWrite(WHITE, HIGH);    // LED
      FollowBlock();
    }

    else {
      digitalWrite(WHITE, LOW);
      DriveForward();
    }
  }
  else {
    digitalWrite(WHITE, LOW);
    DriveForward();
  }
}

void FollowBlock()
{
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
