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
    oldQuad = quadrant;
  }
  else if (quadrant == Home) {
    if (releaseQ == Enemy) {
      if (oldQuad == Neutral_1) {
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
      else if (oldQuad == Neutral_2) {
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
    }
    if (releaseQ != Enemy) {
      
    }
  }
  else if (quadrant == Neutral_1) {
    if (releaseQ == Home) {
      if (oldQuad == Enemy) {
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
    }
    else if (releaseQ == Enemy) {
      if (oldQuad == Home) {
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
    }
    else if (releaseQ == Neutral_2) {
      Release();
      oldQuad = quadrant;
      return;
    }
  }
  else if (quadrant == Enemy) {
    if (releaseQ == Home) {
      if (oldQuad == Neutral_1) {
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
      else if (oldQuad == Neutral_2) {
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
    }
  }
  else if (quadrant == Neutral_2) {
    if (releaseQ == Home) {
      if (oldQuad == Enemy) {
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
    }
    else if (releaseQ == Neutral_1) {
      Release();
      oldQuad = quadrant;
      return;
    }
    else if (releaseQ == Enemy) {
      if (oldQuad == Home) {
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
    }
  }
  else {
    //UpdateArms(100, 100);
    //UpdateWheels(highSpeed, highSpeed);
    DriveForward();
  }
  oldQuad = quadrant;
}

/*void Status_3b() {
  if (!Captured()) {
    Status = 1;
    return;
  }
  UpdatePan(0);
  UpdateArms(100, 100);

  if ((TOF_distance < 100) && (releaseQ == 0)) {
    SetReleaseQ();
  }

  HeadingRelease();
}*/


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


/*void HeadingRelease() {
  if (releaseQ == quadrant) {
    Release();
  }
  else if (releaseQ == Home) {
    if ((quadrant == Neutral_1) || (quadrant == Enemy)) {
      course = South;
      UpdateHeading();
    }
    else if (quadrant == Neutral_2) {
      course = West;
      UpdateHeading();
    }
  }
  else if (releaseQ == Neutral_1) {
    if ((quadrant == Home) || (quadrant == Neutral_2)) {
      course = North;
      UpdateHeading();
    }
    else if (quadrant == Enemy) {
      course = West;
      UpdateHeading();
    }
  }
  else if (releaseQ == Enemy) {
    if ((quadrant == Home) || (quadrant == Neutral_1)) {
      course = East;
      UpdateHeading();
    }
    else if (quadrant == Neutral_2) {
      course = North;
      UpdateHeading();
    }
  }
  else if (releaseQ == Neutral_2) {
    if ((quadrant == Home) || (quadrant == Neutral_1)) {
      course = East;
      UpdateHeading();
    }
    else if (quadrant == Enemy) {
      course = South;
      UpdateHeading();
    }
  }
}
*/
