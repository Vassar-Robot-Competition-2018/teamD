// Functions


void ColorSensor() { // checks floor color sensor (up to 20 times per second)
  //uint16_t clear, red, green, blue;
  int clear, red, green, blue;

  tcs.setInterrupt(false);      // turn on LED
  //delay(60);  // takes 50ms to read
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

  HomQuadrantLED();
  QuadrantLED();
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


void HomQuadrantLED() {
  if (homeQuad == 1) {
    //Serial.println("Red");
    digitalWrite(RED, HIGH);    // LED
    digitalWrite(YELLOW, LOW);    // LED
    digitalWrite(GREEN, LOW);    // LED
    digitalWrite(BLUE, LOW);    // LED
  }

  else if (homeQuad == 2) {
    //Serial.println("Yellow");
    digitalWrite(RED, LOW);    // LED
    digitalWrite(YELLOW, HIGH);    // LED
    digitalWrite(GREEN, LOW);    // LED
    digitalWrite(BLUE, LOW);    // LED
  }

  else if (homeQuad == 3) {
    //Serial.println("Green");
    digitalWrite(RED, LOW);    // LED
    digitalWrite(YELLOW, LOW);    // LED
    digitalWrite(GREEN, HIGH);    // LED
    digitalWrite(BLUE, LOW);    // LED
  }

  else if (homeQuad == 4) {
    //Serial.println("Blue");
    digitalWrite(RED, LOW);    // LED
    digitalWrite(YELLOW, LOW);    // LED
    digitalWrite(GREEN, LOW);    // LED
    digitalWrite(BLUE, HIGH);    // LED
  }
  else {
    digitalWrite(RED, LOW);    // LED
    digitalWrite(YELLOW, LOW);    // LED
    digitalWrite(GREEN, LOW);    // LED
    digitalWrite(BLUE, LOW);    // LED
  }
}
void QuadrantLED() {
  if (quadrant == 1) {
    //Serial.println("Red");
    digitalWrite(RED, HIGH);    // LED
    digitalWrite(YELLOW, LOW);    // LED
    digitalWrite(GREEN, LOW);    // LED
    digitalWrite(BLUE, LOW);    // LED
  }

  else if (quadrant == 2) {
    //Serial.println("Yellow");
    digitalWrite(RED, LOW);    // LED
    digitalWrite(YELLOW, HIGH);    // LED
    digitalWrite(GREEN, LOW);    // LED
    digitalWrite(BLUE, LOW);    // LED
  }

  else if (quadrant == 3) {
    //Serial.println("Green");
    digitalWrite(RED, LOW);    // LED
    digitalWrite(YELLOW, LOW);    // LED
    digitalWrite(GREEN, HIGH);    // LED
    digitalWrite(BLUE, LOW);    // LED
  }

  else if (quadrant == 4) {
    //Serial.println("Blue");
    digitalWrite(RED, LOW);    // LED
    digitalWrite(YELLOW, LOW);    // LED
    digitalWrite(GREEN, LOW);    // LED
    digitalWrite(BLUE, HIGH);    // LED
  }
  else {
    digitalWrite(RED, LOW);    // LED
    digitalWrite(YELLOW, LOW);    // LED
    digitalWrite(GREEN, LOW);    // LED
    digitalWrite(BLUE, LOW);    // LED
  }

}


void BlockColor () {
  if (maxSig == 1) {
    //Serial.println("Red");
    digitalWrite(RED, HIGH);    // LED
    digitalWrite(YELLOW, LOW);    // LED
    digitalWrite(GREEN, LOW);    // LED
    digitalWrite(BLUE, LOW);    // LED
  }

  else if (maxSig == 2) {
    //Serial.println("Yellow");
    digitalWrite(RED, LOW);    // LED
    digitalWrite(YELLOW, HIGH);    // LED
    digitalWrite(GREEN, LOW);    // LED
    digitalWrite(BLUE, LOW);    // LED
  }

  else if (maxSig == 3) {
    //Serial.println("Green");
    digitalWrite(RED, LOW);    // LED
    digitalWrite(YELLOW, LOW);    // LED
    digitalWrite(GREEN, HIGH);    // LED
    digitalWrite(BLUE, LOW);    // LED
  }

  else if (maxSig == 4) {
    //Serial.println("Blue");
    digitalWrite(RED, LOW);    // LED
    digitalWrite(YELLOW, LOW);    // LED
    digitalWrite(GREEN, LOW);    // LED
    digitalWrite(BLUE, HIGH);    // LED
  }
}


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


void MotorUpdate(double L, double R) {
  double servoL_speed = (L * (-0.9)) + 92;
  double servoR_speed = (R * 0.9 ) + 92;
  servoL.write(servoL_speed);
  servoR.write(servoR_speed);
}


void IR_Check()
{
  IR_Short();
  IR_Pixy();
  IR_Left();
  IR_Right();
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

void IR_Pixy() {
  double PixyPrev;
  double PixySum;

  for (int i = 0; i <= 10; i++) {
    double PixyRaw = analogRead(IR_PIXY);
    if (abs(PixyRaw - PixyPrev) > 100) {
      double PixyTemp = PixyRaw;
      PixyRaw = PixyPrev;
      PixyPrev = PixyTemp;
    }
    else {
      PixyPrev = PixyRaw;
    }
    PixySum += PixyRaw;

    if (i == 9) {
      double PixyAvg = PixySum / 10;
      //Serial.println(avg);
      PixyIR = 7032.25 / PixyAvg - 5.55;
      if (PixyIR > 36) {
        PixyIR = 100;
      }

      Serial.print("Pixy IR: ");
      Serial.println();
      //   delay(100);
      PixySum = 0;
    }
  }
}

void IR_Left() {
  double LeftPrev;
  double LeftSum;

  for (int i = 0; i <= 10; i++) {
    double LeftRaw = analogRead(IR_LEFT);
    if (abs(LeftRaw - LeftPrev) > 100) {
      double LeftTemp = LeftRaw;
      LeftRaw = LeftPrev;
      LeftPrev = LeftTemp;
    }
    else {
      LeftPrev = LeftRaw;
    }
    LeftSum += LeftRaw;

    if (i == 9) {
      double LeftAvg = LeftSum / 10;
      //Serial.println(avg);
      LeftIR = 7032.25 / LeftAvg - 5.55;
      if (LeftIR > 36) {
        LeftIR = 100;
      }

      Serial.print("Left IR: ");
      Serial.println();
      //   delay(100);
      LeftSum = 0;
    }
  }
}

void IR_Right() {
  double RightPrev;
  double RightSum;

  for (int i = 0; i <= 10; i++) {
    double RightRaw = analogRead(IR_RIGHT);
    if (abs(RightRaw - RightPrev) > 100) {
      double RightTemp = RightRaw;
      RightRaw = RightPrev;
      RightPrev = RightTemp;
    }
    else {
      RightPrev = RightRaw;
    }
    RightSum += RightRaw;

    if (i == 9) {
      double RightAvg = RightSum / 10;
      //Serial.println(avg);
      RightIR = 7032.25 / RightAvg - 5.55;
      if (RightIR > 36) {
        RightIR = 100;
      }

      Serial.print("Right IR: ");
      Serial.println();
      //   delay(100);
      RightSum = 0;
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

  if (homeQuad != 0) {
    blocks = pixy.getBlocks();

    if (blocks) { //if a color sig is detected by pixy cam
      for (j = 0; j < blocks; j++) { //find the largest signature
        if (pixy.blocks[j].signature == homeQuad) {
          prod = pixy.blocks[j].width * pixy.blocks[j].height;
          if (prod > maxProd) {
            maxProd = prod;
            maxJ = j;

            maxSig = pixy.blocks[maxJ].signature;
            max_X = pixy.blocks[maxJ].x;
            max_Y = pixy.blocks[maxJ].y;
            maxHeight = pixy.blocks[maxJ].height;
            maxWidth = pixy.blocks[maxJ].width;

          }
        }
      }

      FollowBlock();
      digitalWrite(WHITE, HIGH);
    }

    else {

      DriveForward();
    }
  }
  else {
    DriveForward();
    //   digitalWrite(WHITE, LOW);
    //digitalWrite(RED, LOW);    // LED
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

  speedL = (int)constrain(baseSpeedL + wheelAdj, minSpeed, maxSpeed);   // not sure yet if it's L minus adjust or R minus adjust
  speedR = (int)constrain(baseSpeedR - wheelAdj, minSpeed, maxSpeed);

  //  Serial.print("L speed: ");
  //  Serial.print(speedL);
  //  Serial.print(",    R speed: ");
  //  Serial.println(speedR);

  MotorUpdate(speedL, speedR);
  delay(20);
}


