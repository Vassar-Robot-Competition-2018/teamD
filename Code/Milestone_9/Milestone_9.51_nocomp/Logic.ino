void Logic() {
  if (Status == 0) {
    Status_0();
  }
  else if (Status == 1) {
    Status_1();
  }
  else if (Status == 2) {
    Status_2();
  }
  else if (Status == 3) {
    Status_3();
  }
}


void Status_0() {
  DriveForward();
}

void Status_1() {
  UpdateArms(100, 100);
  // captureB = 0;
  releaseQ = 0;
  captureQ = 0;
  oldQuad = 0;

  if (Captured()) {
    SetReleaseQ();
    Status = 3;
    return;
  }
  if (quadrant == Home) {
    if ((maxSig == Neutral_1) || (maxSig == Neutral_2)) {
      captureB = maxSig;
      releaseQ = Enemy;
      captureQ = quadrant;
      Status = 2;
      return;
    }
    else if (maxSig == Enemy) {
      captureB = maxSig;
      releaseQ = Neutral_1;
      captureQ = quadrant;
      Status = 2;
      return;
    }
    else {
      Scan();
      DriveForward();
    }
  }
  else if (quadrant == Neutral_1) {
    if (maxSig == Home) {
      captureB = maxSig;
      releaseQ = Home;
      captureQ = quadrant;
      Status = 2;
      return;
    }
    else if ((maxSig == Neutral_1) || (maxSig == Neutral_2)) {
      captureB = maxSig;
      releaseQ = Enemy;
      captureQ = quadrant;
      Status = 2;
      return;
    }
    else {
      Scan();
      DriveForward();
    }
  }
  else if (quadrant == Enemy) {
    if (maxSig == Home) {
      captureB = maxSig;
      releaseQ = Home;
      captureQ = quadrant;
      Status = 2;
      return;
    }
    else if (maxSig == Enemy) {
      captureB = maxSig;
      releaseQ = Neutral_1;
      captureQ = quadrant;
      Status = 2;
      return;
    }
    else {
      Scan();
      DriveForward();
    }
  }
  else if (quadrant == Neutral_2) {
    if (maxSig == Home) {
      captureB = maxSig;
      releaseQ = Home;
      captureQ = quadrant;
      Status = 2;
      return;
    }
    else if ((maxSig == Neutral_1) || (maxSig == Neutral_2)) {
      captureB = maxSig;
      releaseQ = Enemy;
      captureQ = quadrant;
      Status = 2;
      return;
    }
    else {
      Scan();
      DriveForward();
    }
  }
}

void Status_2() {
  if ((Captured()) && (releaseQ == 0)) {
    SetReleaseQ();
    Status = 3;
    return;
  }
  UpdateArms(0, 0);
  if ((captureQ == quadrant) && (maxSig == captureB)) {
    if (TOF_distance < 600) {
      if (Captured()) {
        Grab();
        UpdatePan(0);
        Status = 3;
        return;
      }
      else {
        TrackBlock();
        FollowBlock();
        //UpdateWheels(0, 0);
      }
    }
  }
  else {
    Status = 1;
    return;
  }
}

void Status_3() {
  if (!Captured()) {
    Status = 1;
    return;
  }
  UpdatePan(0);
  UpdateArms(100, 100);
  if (oldQuad == 0) {
    oldQuad = quadrant;
  }
  if ((Captured()) && (releaseQ == 0)) {
    SetReleaseQ();
  }
  if (quadrant == releaseQ) {
    Release();
    return;
  }
  else if ((quadrant == Neutral_1) && (releaseQ == Neutral_2)) {
    Release();
    return;
  }
  else if ((quadrant == Neutral_2) && (releaseQ == Neutral_1)) {
    Release();
    return;
  }
  else if (quadrant == Home) {
    if ((releaseQ == Neutral_1) && (oldQuad == Neutral_2)) {
      digitalWrite(WHITE, HIGH);
      DriveForward();
      delay(2500);
      RightTurn();
      delay(1000);
      DriveStop();
      delay(10);
      oldQuad = quadrant;
      digitalWrite(WHITE, LOW);
      return;
    }
    else if ((releaseQ == Enemy) && (oldQuad == Neutral_2)) {
      digitalWrite(WHITE, HIGH);
      DriveReverse();
      delay(2500);
      RightTurn();
      delay(1000);
      DriveStop();
      delay(10);
      oldQuad = quadrant;
      digitalWrite(WHITE, LOW);
      return;
    }
    else if ((releaseQ == Neutral_2) && (oldQuad == Neutral_1)) {
      digitalWrite(WHITE, HIGH);
      DriveForward();
      delay(2500);
      LeftTurn();
      delay(1000);
      DriveStop();
      delay(10);
      oldQuad = quadrant;
      digitalWrite(WHITE, LOW);
      return;
    }
    else if ((releaseQ == Enemy) && (oldQuad == Neutral_1)) {
      digitalWrite(WHITE, HIGH);
      DriveReverse();
      delay(2500);
      LeftTurn();
      delay(1000);
      DriveStop();
      delay(10);
      oldQuad = quadrant;
      digitalWrite(WHITE, LOW);
      return;
    }
    else {
      DriveForward();
    }
  }

  else if (quadrant == Neutral_1) {
    if ((releaseQ == Enemy) && (oldQuad == Home)) {
      digitalWrite(WHITE, HIGH);
      DriveForward();
      delay(2500);
      RightTurn();
      delay(1000);
      DriveStop();
      delay(10);
      oldQuad = quadrant;
      digitalWrite(WHITE, LOW);
      return;
    }
    //    else if ((releaseQ == Neutral_2) && (oldQuad == Home)) {
    //      digitalWrite(WHITE, HIGH);
    //      DriveReverse();
    //      delay(2500);
    //      RightTurn();
    //      delay(1000);
    //      DriveStop();
    //      delay(10);
    //      oldQuad = quadrant;
    //      digitalWrite(WHITE, LOW);
    //      return;
    //    }
    else if ((releaseQ == Home) && (oldQuad == Enemy)) {
      digitalWrite(WHITE, HIGH);
      DriveForward();
      delay(2500);
      LeftTurn();
      delay(1000);
      DriveStop();
      delay(10);
      oldQuad = quadrant;
      digitalWrite(WHITE, LOW);
      return;
    }
    else if ((releaseQ == Neutral_2) && (oldQuad == Enemy)) {
      digitalWrite(WHITE, HIGH);
      DriveReverse();
      delay(2500);
      LeftTurn();
      delay(1000);
      DriveStop();
      delay(10);
      oldQuad = quadrant;
      digitalWrite(WHITE, LOW);
      return;
    }
    else {
      DriveForward();
    }
  }

  else if (quadrant == Enemy) {
    if ((releaseQ == Neutral_2) && (oldQuad ==  Neutral_1)) {
      digitalWrite(WHITE, HIGH);
      DriveForward();
      delay(2500);
      RightTurn();
      delay(1000);
      DriveStop();
      delay(10);
      oldQuad = quadrant;
      digitalWrite(WHITE, LOW);
      return;
    }
    else if ((releaseQ == Home) && (oldQuad == Neutral_1)) {
      digitalWrite(WHITE, HIGH);
      DriveReverse();
      delay(2500);
      RightTurn();
      delay(1000);
      DriveStop();
      delay(10);
      oldQuad = quadrant;
      digitalWrite(WHITE, LOW);
      return;
    }
    else if ((releaseQ == Neutral_1) && (oldQuad == Neutral_2)) {
      digitalWrite(WHITE, HIGH);
      DriveForward();
      delay(2500);
      LeftTurn();
      delay(1000);
      DriveStop();
      delay(10);
      oldQuad = quadrant;
      digitalWrite(WHITE, LOW);
      return;
    }
    else if ((releaseQ == Home) && (oldQuad == Neutral_2)) {
      digitalWrite(WHITE, HIGH);
      DriveReverse();
      delay(2500);
      LeftTurn();
      delay(1000);
      DriveStop();
      delay(10);
      oldQuad = quadrant;
      digitalWrite(WHITE, LOW);
      return;
    }
    else {
      DriveForward();
    }
  }

  else if (quadrant == Neutral_2) {
    if ((releaseQ == Home) && (oldQuad ==  Enemy)) {
      digitalWrite(WHITE, HIGH);
      DriveForward();
      delay(2500);
      RightTurn();
      delay(1000);
      DriveStop();
      delay(10);
      oldQuad = quadrant;
      digitalWrite(WHITE, LOW);
      return;
    }
    //    else if ((releaseQ == Neutral_1) && (oldQuad == Enemy)) {
    //      digitalWrite(WHITE, HIGH);
    //      DriveReverse();
    //      delay(2500);
    //      RightTurn();
    //      delay(1000);
    //      DriveStop();
    //      delay(10);
    //      oldQuad = quadrant;
    //      digitalWrite(WHITE, LOW);
    //      return;
    //    }
    else if ((releaseQ == Enemy) && (oldQuad == Home)) {
      digitalWrite(WHITE, HIGH);
      DriveForward();
      delay(2500);
      LeftTurn();
      delay(1000);
      DriveStop();
      delay(10);
      oldQuad = quadrant;
      digitalWrite(WHITE, LOW);
      return;
    }
    else if ((releaseQ == Neutral_1) && (oldQuad == Home)) {
      digitalWrite(WHITE, HIGH);
      DriveReverse();
      delay(2500);
      LeftTurn();
      delay(1000);
      DriveStop();
      delay(10);
      oldQuad = quadrant;
      digitalWrite(WHITE, LOW);
      return;
    }
    else {
      DriveForward();
    }
  }

  else {
    UpdateArms(100, 100);
    UpdateWheels(highSpeed, highSpeed);
  }
  oldQuad = quadrant;
}


void SetReleaseQ() {
  if (captureB == Home) {
    releaseQ = Home;
  }
  else if ((captureB == Neutral_1) || (captureB == Neutral_2)) {
    releaseQ = Enemy;
  }
  else if (captureB == Enemy) {
    releaseQ = Neutral_1;
  }
}
