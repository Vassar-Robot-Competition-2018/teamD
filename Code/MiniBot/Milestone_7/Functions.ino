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


void QuadrantLED() {
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
  blockServoL.write(135);
  blockServoR.write(45);
  delay(1000);
}


void ReleaseBlock() {
  blockServoL.write(135);
  blockServoR.write(45);
  DriveReverse();
  delay(2000);
  RightTurn();
  delay(2000);
}


void HoldingBlock() {
  if (irShortDist <= CAPTURED)
    DriveForward();
}


//void BlockComplete() {
//  speedL = 0;
//  speedR = 0;
//  MotorUpdate(speedL, speedR);
//  BlockColor();
//  digitalWrite(WHITE, HIGH);    // LED
//  //  Serial.print("maxWidth: "); Serial.println(maxWidth);
//  //  Serial.print("irPixyDist: "); Serial.println(irPixyDist);
//  delay(5000);
//}


void MotorUpdate(double L, double R) {
  double servoL_speed = (L * (-0.9)) + 92;
  double servoR_speed = (R * 0.9 ) + 92;
  servoL.write(servoL_speed);
  servoR.write(servoR_speed);
}


void IR_Check()
{
  IR_Front();
  IR_Left();
  IR_Right();
}


void IR_Front() {
  double frontPrev;
  double frontSum;

  for (int i = 0; i <= 10; i++) {
    double frontRaw = analogRead(IR_FRONT);
    if (abs(frontRaw - frontPrev) > 100) {
      double frontTemp = frontRaw;
      frontRaw = frontPrev;
      frontPrev = frontTemp;
    }
    else {
      frontPrev = frontRaw;
    }
    frontSum += frontRaw;

    if (i == 9) {
      double frontAvg = frontSum / 10;
      //Serial.println(avg);
      frontIR = (5996.74 / frontAvg) - 2.253;
      if (frontIR > 36) {
        frontIR = 100;
      }

      Serial.print("Front IR: ");
      Serial.println();
      //   delay(100);
      frontSum = 0;
    }
  }
}

void IR_Left() {
  double leftPrev;
  double leftSum;

  for (int j = 0; j <= 10; j++) {
    double leftRaw = analogRead(IR_LEFT);
    if (abs(leftRaw - leftPrev) > 100) {
      double leftTemp = leftRaw;
      leftRaw = leftPrev;
      leftPrev = leftTemp;
    }
    else {
      leftPrev = leftRaw;
    }
    leftSum += leftRaw;

    if (j == 9) {
      double leftAvg = leftSum / 10;
      //Serial.println(avg);
      leftIR = (5996.74 / leftAvg) - 2.253;
      if (leftIR > 36) {
        leftIR = 100;
      }
      Serial.print("                    Left IR: ");
      Serial.println(leftIR);
      //  delay(100);
      leftSum = 0;
    }
  }
}

void IR_Right() {
  double rightPrev;
  double rightSum;

  for (int k = 0; k <= 10; k++) {
    double rightRaw = analogRead(IR_RIGHT);
    if (abs(rightRaw - rightPrev) > 100) {
      double rightTemp = rightRaw;
      rightRaw = rightPrev;
      rightPrev = rightTemp;
    }
    else {
      rightPrev = rightRaw;
    }
    rightSum += rightRaw;

    if (k == 9) {
      double rightAvg = rightSum / 10;
      //Serial.println(avg);
      rightIR = (5996.74 / rightAvg) - 2.253;
      if (rightIR > 36) {
        rightIR = 100;
      }

      Serial.print("                                        Right IR: ");
      Serial.println(rightIR);
      // delay(100);
      rightSum = 0;
    }
  }
}

void BlockLeft() {
  DriveForward();
  delay(1000);

  speedL = -75;
  speedR = -20;
  MotorUpdate(speedL, speedR);
  delay(500);

  DriveStop();
  delay(100);
}

void BlockRight() {
  DriveForward();
  delay(1000);

  speedL = -20;
  speedR = -75;
  MotorUpdate(speedL, speedR);
  delay(500);

  DriveStop();
  delay(100);
}

void CheckBlocks() {
  // for (int k = 0; k < 10; k++) {
  static int i = 0;
  uint16_t blocks;
  char buf[32];

  maxJ = 0;
  maxSig = 6;
  max_X = sp;
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

      //int checkDist = 1500 / irShortDist;
      //if (((800 / irShortDist) < pixy.blocks[maxJ].width) && ((1400 / irShortDist) > pixy.blocks[maxJ].width)) {
      //  if ((checkDist > pixy.blocks[maxJ].width) && (checkDist > pixy.blocks[maxJ].height)) {
      if ((irPixyDist < 30) && (maxSig == homeQuad));
      //digitalWrite(RED, HIGH);    // LED
      FollowBlock();
    }

    else {
      //Serial.println("None");
      //    digitalWrite(RED, LOW);    // LED
      //    digitalWrite(YELLOW, LOW);    // LED
      //    digitalWrite(GREEN, LOW);    // LED
      //    digitalWrite(BLUE, LOW);    // LED


      DriveForward();
      //      digitalWrite(GREEN, HIGH);
      //      digitalWrite(YELLOW, LOW);
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
  error = sp - max_X;
  if (timeChange > 100) {
    errSum = 0;
    dErr = 0;
  }
  else {
    /*Compute all the working error variables*/
    errSum += (error * timeChange);
    dErr = (error - prevErr) / timeChange;
  }
  /*Compute PID Output*/
  wheelAdj = kp * error + ki * errSum + kd * dErr;

  /*Remember some variables for next time*/
  prevErr = error;
  prevTime = current;

  speedL = (int)constrain(baseSpeedL + wheelAdj, minSpeed, maxSpeed);   // not sure yet if it's L minus adjust or R minus adjust
  speedR = (int)constrain(baseSpeedR - wheelAdj, minSpeed, maxSpeed);

  //  Serial.print("L speed: ");
  //  Serial.print(speedL);
  //  Serial.print(",    R speed: ");
  //  Serial.println(speedR);

  MotorUpdate(speedL, speedR);
}

