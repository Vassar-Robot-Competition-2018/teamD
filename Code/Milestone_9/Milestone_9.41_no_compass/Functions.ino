// Functions


void ColorSensorL() { // checks floor color sensor (up to 20 times per second)
  //uint16_t clear, red, green, blue;
  int clear, red, green, blue;

  tcsL.setInterrupt(false);      // turn on LED
  tcsL.getRawData(&red, &green, &blue, &clear);
  tcsL.setInterrupt(true);  // turn off LED

  //  Serial.print("Left: C = "); Serial.print(clear);
  //  Serial.print("; R = "); Serial.print(red);
  //  Serial.print("; G = "); Serial.print(green);
  //  Serial.print("; B = "); Serial.print(blue);

  if (clear == 0) {
    tcsL.begin();  // initializes left color sensor
    Serial.print("RESET");
  }

  if (LC_gray == 0) {
    L_ColorCalibration(clear, red, green, blue);
  }
  else {
    WhiteCheck(clear, LC_gray);
    Quadrant(red, green, blue, 1);
  }
}


void ColorSensorR() { // checks floor color sensor (up to 20 times per second)
  //uint16_t clear, red, green, blue;
  int clear, red, green, blue;

  tcsR.setInterrupt(false);      // turn on LED
  //delay(60);  // takes 50ms to read
  tcsR.getRawData(&red, &green, &blue, &clear);
  tcsR.setInterrupt(true);  // turn off LED

  //  Serial.print("     Right: C = "); Serial.print(clear);
  //  Serial.print("; R = "); Serial.print(red);
  //  Serial.print("; G = "); Serial.print(green);
  //  Serial.print("; B = "); Serial.println(blue);

  if (clear == 0) {
    tcsR.begin();  // initializes left color sensor
    Serial.print("RESET");
  }

  if (RC_gray == 0) {
    R_ColorCalibration(clear, red, green, blue);
  }
  else {
    WhiteCheck(clear, RC_gray);
    Quadrant(red, green, blue, 2);
  }
}

void ColorSensors() { // checks floor color sensor (up to 20 times per second)
  current = millis();
  if ((current - lastColorUpdate) >= colorInterval) {
    ColorSensorL();
    ColorSensorR();
  }
}


void SetHome() { // sets home quadrant
  if ((Home == 5) && (quadrant != 0)) {
    Home = quadrant;
    Cap_Rel_Table();
    Status = 1;
  }
}


void Quadrant(int r, int g, int b, int side) {

  if (side == 1) {
    if (r > (g + b))  //red
      quadL = 1;

    else if ((3 * g) > ((r + b) * 2))  //green
      quadL = 2;

    else if ((r + g) > (4 * b))  //yellow
      quadL = 3;

    else if (b > (2 * r))  //blue
      quadL = 4;
  }

  else if (side == 2) {
    if (r > (g + b))  //red
      quadR = 1;

    else if ((3 * g) > ((r + b) * 2))  //green
      quadR = 2;

    else if ((r + g) > (4 * b))  //yellow
      quadR = 3;

    else if (b > (2 * r))  //blue
      quadR = 4;
  }

  if (quadL == quadR) {
    quadrant = quadL;
  }

  else if (quadL != quadrant) {
    UpdateWheels(-highSpeed, highSpeed);
  }

  else if (quadR != quadrant) {
    UpdateWheels(highSpeed, -highSpeed);
  }

  SetHome();

  if (quadrant != tempQuad) {
    prevQuad = tempQuad;
    tempQuad = quadrant;
  }

  //  Serial.print("Home Quadrant: ");
  //  Serial.print(Home);
  //  Serial.print("Quadrant color: ");
  //  Serial.println(quadrant);
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

    UpdateWheels(-baseSpeedL, -baseSpeedR);
    delay(700);

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


void UpdateWheels(double L, double R) {
  double l_wheel_pulse = map(L, -100, 100, L_WHEEL_MAX, L_WHEEL_MIN); // -100 is full reverse, 100 is full forward
  double r_wheel_pulse = map(R, -100, 100, R_WHEEL_MIN, R_WHEEL_MAX); // 0 is stopped
  pwm.setPWM(L_WHEEL, 0, l_wheel_pulse);
  pwm.setPWM(R_WHEEL, 0, r_wheel_pulse);
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


void CheckBlocks() {
  current = millis();
  if ((current - lastBlockUpdate) >= blockInterval) // time to update
  {
    lastBlockUpdate = millis();

    static int i = 0;
    uint16_t blocks;
    char buf[32];

    maxJ = 0;
    maxSig = 6;
    max_X = camSP;
    maxWidth = 0;
    maxHeight = 0;
    prod = 0;
    maxProd = 100;

    if (TOF_distance < 400) {
      blocks = pixy.getBlocks();

      if (blocks) { //if a color sig is detected by pixy cam
        for (j = 0; j < blocks; j++) { //find the largest signature
          if ((pixy.blocks[j].height > 20) && (pixy.blocks[j].height < 125)) {
            if ((pixy.blocks[j].width > 20) && (pixy.blocks[j].width < 125)) {
              prod = pixy.blocks[j].width * pixy.blocks[j].height;
              if (prod > maxProd) {
                maxProd = prod;
                maxJ = j;
                maxSig = pixy.blocks[maxJ].signature;
                max_X = pixy.blocks[maxJ].x;
                maxHeight = pixy.blocks[maxJ].height;
                maxWidth = pixy.blocks[maxJ].width;
              }
            }
          }
        }
      }
    }
  }
}


void TrackBlock()
{
  current = millis();
  if ((current - lastCamUpdate) >= camInterval) { // time to update

    trackTimeChange = (double)(current - lastCamUpdate);
    trackError = camSP - max_X;

    if (trackTimeChange > 100) {
      trackdErr = 0;
    }
    else {
      /*Compute all the working error variables*/
      trackdErr = (trackError - trackPrevErr) / trackTimeChange;
    }
    /*Compute PID Output*/
    servoAdj = trackKp * trackError + trackKd * trackdErr;

    servoPos = (int)constrain(servoPos + servoAdj, servoMin, servoMax);

    trackPrevErr = trackError;
    lastCamUpdate = millis();
    UpdatePan(servoPos);
  }
}


void FollowBlock()
{
  current = millis();
  if ((current - lastWheelUpdate) >= wheelInterval) { // time to update

    followTimeChange = (double)(current - lastWheelUpdate);
    followError = servoSP - servoPos;

    if (followTimeChange > 100) {
      followdErr = 0;
    }
    else {
      /*Compute all the working error variables*/
      followdErr = (followError - followPrevErr) / followTimeChange;
    }
    /*Compute PID Output*/
    wheelAdj = followKp * followError + followKd * followdErr;

    differentialSpeed = (int)constrain(130 - abs(followError), 30, 100);

    speedL = (int)constrain(differentialSpeed + wheelAdj, minSpeed, maxSpeed);
    speedR = (int)constrain(differentialSpeed - wheelAdj, minSpeed, maxSpeed);

    followPrevErr = followError;
    lastWheelUpdate = millis();
    //  MotorUpdate(speedL, speedR);
    UpdateWheels(speedL, speedR);
  }
}


void AvoidBlock()
{
  current = millis();
  if ((current - lastWheelUpdate) >= wheelInterval) { // time to update

    avoidTimeChange = (double)(current - lastWheelUpdate);
    if (max_X < camSP) {
      avoidError = max_X;
    }
    else if (max_X >= camSP) {
      avoidError = (max_x - 320);
    }
    if (avoidTimeChange > 100) {
      avoiddErr = 0;
    }
    else {
      /*Compute all the working error variables*/
      avoiddErr = (avoidError - avoidPrevErr) / avoidTimeChange;
    }
    /*Compute PID Output*/
    wheelAdj = avoidKp * avoidError + avoidKd * avoiddErr;

    differentialSpeed = (int)constrain(130 - abs(avoidError), 30, 100);

    speedL = (int)constrain(differentialSpeed + wheelAdj, minSpeed, maxSpeed);
    speedR = (int)constrain(differentialSpeed - wheelAdj, minSpeed, maxSpeed);

    avoidPrevErr = avoidError;
    lastWheelUpdate = millis();
    //  MotorUpdate(speedL, speedR);
    UpdateWheels(speedL, speedR);
  }
}


void Scan() {
  current = millis();
  if ((current - lastScanUpdate) >= scanInterval) // time to update
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


bool Captured() {
  // if (((maxHeight > 80)  && (maxWidth > 105)) || (TOF_distance < 80)) {
  if (TOF_distance < 50) {
    return true;
  }
  else return false;
}



void TOF() {
  current = millis();
  if ((current - lastTOFupdate) >= TOFinterval) { // time to update

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
    lastTOFupdate = millis();
  }
}


void Heading() {
  if (heading == 400) {
    compass.read();
    heading = compass.heading();

    North = (heading);
    if (North <= 0) {
      North += 360;
    }
    East = (North + 90);
    if (East > 360) {
      East -= 360;
    }
    South = (North + 180);
    if (South > 360) {
      South -= 360;
    }
    West = (North + 270);
    if (West > 360) {
      West -= 360;
    }
  }
  else {
    compass.read();
    heading = compass.heading();
    //    Serial.println(heading);
  }
}

void UpdateHeading() {

  current = millis();
  if ((current - lastHeadingUpdate) >= headingInterval) { // time to update

    Heading();

    headingTimeChange = (double)(current - lastHeadingUpdate);

    headingError = course - heading;
    if (headingError > 180) {
      headingError -= 360;
    }
    else if (headingError < -180) {
      headingError += 360;
    }

    if (headingTimeChange > 100) {
      headingdErr = 0;
    }
    else {
      /*Compute all the working error variables*/
      headingdErr = (headingError - headingPrevErr) / headingTimeChange;
    }
    /*Compute PID Output*/
    wheelAdj = headingKp * headingError + headingKd * headingdErr;

    differentialSpeed = (int)constrain(180 - abs(headingError), 0, 180);
    differentialSpeed = (int)(differentialSpeed * 0.56);

    speedL = (int)constrain(differentialSpeed + wheelAdj, minSpeed, maxSpeed);
    speedR = (int)constrain(differentialSpeed - wheelAdj, minSpeed, maxSpeed);

    headingPrevErr = headingError;
    lastHeadingUpdate = millis();
    //  MotorUpdate(speedL, speedR);
    UpdateWheels(speedL, speedR);
  }
}


