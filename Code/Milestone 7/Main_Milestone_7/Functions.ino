// Functions
//    TrackBlock()
//    Scan()


//void PID_Pixy_Wheels() {
//  if (irFrontDist <= minDist) {
//
//    digitalWrite(WHITE, HIGH);    // LED
//    pixyError = pixySP - max_X;
//    //Serial.print(pixyError: ";
//    //Serial.println(pixyError);
//
//    wheelAdj = wheelKp * pixyError + wheelKd * (pixyError - lastPixyError);
//    lastPixyError = pixyError;
//    speedL = (int)(baseSpeedL + wheelAdj);   // not sure yet if it's L minus adjust or R minus adjust
//    speedR = (int)(baseSpeedR - wheelAdj);
//
//    if (speedL > maxSpeedL)
//      speedL = maxSpeedL;
//
//    else if (speedL < minSpeedL)
//      speedL = minSpeedL;
//
//    if (speedR > maxSpeedR)
//      speedR = maxSpeedR;
//
//    else if (speedR < minSpeedR)
//      speedR = minSpeedR;
//
//    MotorUpdate(speedL, speedR);
//  }
//  else
//    digitalWrite(WHITE, LOW);    // LED
//}

//checks floor color sensor (up to 20 times per second)
void ColorSensor() {
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


void SetHome(int r, int g, int b) {
  if ((homeQ == 0) && (quadrant != 0)) {
    homeQ = quadrant;
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

  if (quadrant != quadCheck) {
    prev_quadrant = quadCheck;
    quadCheck = quadrant;
  }

  Serial.print("Quadrant color: ");
  Serial.println(quadrant);
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


//void ServoPos(double pos) {  // converts servo position from (-100)-100 to 0-180 and moves servo
//  int servoPos = int (((pos * 10)) - 90);
//  return servoPos;
//}


//void StateCheck() {
//  if (state == 0) { // if searching for block
//    DriveForward();
//    // Sweep();
//  }
//}


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


//void PixyPID() {
//
//  pixyError = pixySP - max_X;
//  //Serial.print(pixyError: ";
//  //Serial.println(pixyError);
//
//  servoAdj = pixyKp * pixyError + pixyKd * (pixyError - lastPixyError);
//  lastPixyError = pixyError;
//  pos = lastPos + servoAdj;
//  lastPos = pos;
//
//  if (pos > servoMax)
//    pos = servoMax;
//
//  else if (pos < servoMin)
//    pos = servoMin;
//
//  camServo.write(pos);
//  delay(20);
//}
//
//
//void WheelPID() {
//  servoError = servoSP - pos;
//  //Serial.print("servoError: ");
//  //Serial.println(servoError);
//
//  wheelAdj = wheelKp * servoError + wheelKd * (servoError - lastServoError);
//  lastServoError = servoError;
//  speedL = baseSpeedL + wheelAdj;   // not sure yet if it's L minus adjust or R minus adjust
//  speedR = baseSpeedR - wheelAdj;
//
//  if (speedL > maxSpeedL)
//    speedL = maxSpeedL;
//
//  else if (speedL < minSpeedL)
//    speedL = minSpeedL;
//
//  if (speedR > maxSpeedR)
//    speedR = maxSpeedR;
//
//  else if (speedR < minSpeedR)
//    speedR = minSpeedR;
//
//  MotorUpdate(speedL, speedR);
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


//void Sweep() {
//  if (dir == 0)   // dir 0 sweeps from left to right
//  {
//    pos += 2;
//    camServo.write(pos);
//    delay(10);
//    if (pos >= servoMax)
//    {
//      dir = 1;
//    }
//  }
//
//  else if (dir == 1) // dir 1 sweeps from right to left
//  {
//    pos -= 2;
//    camServo.write(pos);
//    delay(10);
//    if (pos <= servoMin)
//    {
//      dir = 0;
//    }
//  }
//}


void IR_Short() {
  irShort = analogRead(IR_SHORT);

  if ((irShort > 100) && (irShort < 700)) {
    irShortDist = 3187.58 / irShort - 1.6016;
  }
  else {
    irShortDist = 100;

    //    Serial.print("Short IR Value: ");  // returns it to the serial monitor
    //    Serial.print(irShort);
    //    Serial.print("     Distance: ");  // returns it to the serial monitor
    //    Serial.print(irShortDist);
    //    Serial.println(" cm");
    //    Serial.println();
  }
}


void IR_Front() {
  irFront = analogRead(IR_FRONT);

  if ((irFront > 70) && (irFront < 700)) {
    irFrontDist = 6206.62 / irFront - 0.1826;
  }
  else {
    irFrontDist = 100;
  }

  //  Serial.print("Long IR Value: ");  // returns it to the serial monitor
  //  Serial.print(irFront);
  //  Serial.print("     Distance: ");  // returns it to the serial monitor
  //  Serial.print(irFrontDist);
  //  Serial.println(" cm");
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
  if (irFrontDist <= captured)
    DriveForward();
}


void BlockComplete() {
  speedL = 0;
  speedR = 0;
  MotorUpdate(speedL, speedR);
  BlockColor();
  digitalWrite(WHITE, HIGH);    // LED
  Serial.print("maxWidth: "); Serial.println(maxWidth);
  Serial.print("irFrontDist: "); Serial.println(irFrontDist);
  delay(5000);
}


void MotorUpdate(double L, double R) {
  double servoL_speed = (L * (-0.9)) + 92;
  double servoR_speed = (R * 0.9 ) + 92;
  servoL.write(servoL_speed);
  servoR.write(servoR_speed);
}

void CheckBlocks() {
  static int i = 0;
  uint16_t blocks;
  char buf[32];

  maxJ = 0;
  maxSig = 0;
  max_X = 0;
  max_Y = 0;
  maxWidth = 0;
  maxHeight = 0;

  blocks = pixy.getBlocks();

  if (blocks) { //if a color sig is detected by pixy cam
    for (j = 0; j < blocks; j++) { //find the largest object that fits the signature
      prod = pixy.blocks[j].width * pixy.blocks[j].height;
      if (prod > maxProd) {
        maxProd = prod;
        maxJ = j;
      }
    }

    //if ((pixy.blocks[maxJ].width > minWidth) && ((pixy.blocks[maxJ].width < lineWidth) && (pixy.blocks[maxJ].height < lineHeight))) { // if color sig is big enough and object is detected by the IR sensor
    //    if (pixy.blocks[maxJ].width > minWidth) {
    maxSig = pixy.blocks[maxJ].signature;
    max_X = pixy.blocks[maxJ].x;
    max_Y = pixy.blocks[maxJ].y;
    maxHeight = pixy.blocks[maxJ].height;
    maxWidth = pixy.blocks[maxJ].width;

    //    Serial.print("Block Position: ");
    //    Serial.print(max_X);
    //    Serial.print("     ");
    //    Serial.print("Block Dimensions: ");
    //    Serial.print(maxWidth);
    //    Serial.print(" x ");
    //    Serial.println(maxWidth);
    Serial.print("Error: ");
    Serial.println(camError);

    BlockColor();
    tracking = 1;
    scanning = 0;
    digitalWrite(GREEN, HIGH);
    digitalWrite(YELLOW, LOW);
    TrackBlock();
  }

  else {
    Serial.println("None");
    //    digitalWrite(RED, LOW);    // LED
    //    digitalWrite(YELLOW, LOW);    // LED
    //    digitalWrite(GREEN, LOW);    // LED
    //    digitalWrite(BLUE, LOW);    // LED


    //DriveForward();
    tracking = 0;
    scanning = 1;
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, HIGH);
    Scan();
  }

  //  Serial.print("Block Position: (");
  //  Serial.print(max_X, max_Y);
  //  Serial.println(")");
  //  Serial.print("Block Dimensions: ");
  //  Serial.print(maxWidth);
  //  Serial.print(" x ");
  //  Serial.println(maxWidth);

  //Serial.print("maxWidth: "); Serial.println(maxWidth);
  //Serial.print("irFrontDist: "); Serial.println(irFrontDist);

  // delay(30);
}


void TrackBlock()
{
  current = millis();

  /*How long since we last calculated*/
  trackTimeChange = (double)(current - trackLastTime);

  /*Compute all the working error variables*/


  if (trackTimeChange < 20) {
    camError = camSP - max_X;
    camErrSum += (camError * trackTimeChange);
    camdErr = (camError - prevCamError) / trackTimeChange;
  }
  else {
    camError = 0;
    camErrSum = 0;
    camdErr = 0;
  }
  /*Compute PID Output*/
  panAdjust = panKp * camError + panKi * camErrSum + panKd * camdErr;

  panPos = constrain(panPos + panAdjust, panMin, panMax);
  camServo.write(panPos);
  /*Remember some variables for next time*/
  prevCamError = camError;
  trackLastTime = current;
}


void Scan() {
  current = millis();
  if ((current - trackLastTime) > 500)
  {
    if ((current - lastScanUpdate) > scanUpdateInterval) // time to update
    {
      lastScanUpdate = current;
      //panPos = constrain( panPos + increment, panMin, panMax);
      panPos += increment;
      camServo.write(panPos);

      Serial.print("Servo position: ");
      Serial.println(panPos);
      if ((panPos >= panMax) || (panPos <= panMin)) // end of sweep
      {
        // reverse direction
        increment = -increment;
      }
    }
  }
}

