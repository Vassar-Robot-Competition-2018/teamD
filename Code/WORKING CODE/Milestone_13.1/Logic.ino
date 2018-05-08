void Logic() {
 // while(zerovar == 0) {
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
 // }
}


void Status_0() {
  DriveForward();
}


void Status_1() {
  //captureB = 0;
  releaseQ = 0;
  captureQ = 0;
  oldQuad = 0;

  if (Captured()) {
    UpdateArms(100, 100);
    UpdatePan(0);
    Status = 3;
    return;
  }
  if (quadrant == Home) {
    if ((maxSig == Neutral_1) || (maxSig == Neutral_2)) {
      captureB = maxSig;
      releaseQ = Enemy;
      captureQ = quadrant;
      UpdateArms(0, 0);
      Status = 2;
      return;
    }
    else if (maxSig == Enemy) {
      captureB = maxSig;
      releaseQ = Neutral_1;
      captureQ = quadrant;
      UpdateArms(0, 0);
      Status = 2;
      return;
    }
    else {
      Scan();
      DriveForward();
    }
  }
  else if ((quadrant == Neutral_1) || (quadrant == Neutral_2)) {
    if (maxSig == Home) {
      captureB = maxSig;
      releaseQ = Home;
      captureQ = quadrant;
      UpdateArms(0, 0);
      Status = 2;
      return;
    }
    else if ((maxSig == Neutral_1) || (maxSig == Neutral_2)) {
      captureB = maxSig;
      releaseQ = Enemy;
      captureQ = quadrant;
      UpdateArms(0, 0);
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
      UpdateArms(0, 0);
      Status = 2;
      return;
    }
    else if (maxSig == Enemy) {
      captureB = maxSig;
      releaseQ = Neutral_1;
      captureQ = quadrant;
      UpdateArms(0, 0);
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
  /*if (Captured()) {
    UpdatePan(0);
    UpdateArms(100, 100);
    Status = 3;
    return;
  }*/
  if ((captureQ == quadrant)) { //(captureQ == quadrant) && (maxSig == captureB)
    //   if (TOF_distance < 200) {  // remove this condition if the block width and height conditions work well in CheckBlocks
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
  // }
  else {
    UpdateArms(100, 100);
    Status = 1;
    return;
  }
}

void Status_3() {
  if (!Captured()) {
    UpdateArms(100, 100);
    Status = 1;
    return;
  }
  if (oldQuad == 0) {
    oldQuad = quadrant;
  }
  if (releaseQ == 0) {
    SetReleaseQ();
  }
  if (quadrant == releaseQ) {
    Release();
    return;
  }
  else if ((quadrant == Neutral_1) && (releaseQ == Neutral_2)) {
    digitalWrite(RED_F, HIGH);
    digitalWrite(GREEN_F, HIGH);
    Release();
    return;
  }
  else if ((quadrant == Neutral_2) && (releaseQ == Neutral_1)) {
    digitalWrite(RED_F, HIGH);
    digitalWrite(GREEN_F, HIGH);
    Release();
    return;
  }
  else if (quadrant == Home) {
    if ((releaseQ == Neutral_1) && (oldQuad == Neutral_2)) {
      Fwd_RightTurn();
    }
    else if ((releaseQ == Enemy) && (oldQuad == Neutral_2)) {
      Back_RightTurn();
    }
    else if ((releaseQ == Enemy) && (oldQuad == Neutral_1)) {
      Back_LeftTurn();
    }
    else if ((releaseQ == Neutral_2) && (oldQuad == Neutral_1)) {
      Fwd_LeftTurn();
    }
    else {
      DriveForwardHigh();
    }
  }
  else if (quadrant == Neutral_1) {
    if ((releaseQ == Enemy) && (oldQuad == Home)) {
      Fwd_RightTurn();
    }
    else if (releaseQ == Neutral_2) {
      Release();
      return;
    }
    else if ((releaseQ == Home) && (oldQuad == Enemy)) {
      Fwd_LeftTurn();
    }
    else {
      DriveForwardHigh();
    }
  }
  else if (quadrant == Enemy) {
    if ((releaseQ == Neutral_2) && (oldQuad == Neutral_1)) {
      Fwd_RightTurn();
    }
    else if ((releaseQ == Home) && (oldQuad == Neutral_1)) {
      Back_RightTurn();
    }
    else if ((releaseQ == Home) && (oldQuad == Neutral_2)) {
      Back_LeftTurn();
    }
    else if ((releaseQ == Neutral_1) && (oldQuad == Neutral_2)) {
      Fwd_LeftTurn();
    }
    else {
      DriveForwardHigh();
    }
  }
  else if (quadrant == Neutral_2) {
    if ((releaseQ == Home) && (oldQuad == Enemy)) {
      Fwd_RightTurn();
    }
    else if (releaseQ == Neutral_1) {
      Release();
      return;
    }
    else if ((releaseQ == Enemy) && (oldQuad == Home)) {
      Fwd_LeftTurn();
    }
    else {
      DriveForwardHigh();
    }
  }
  else {
    DriveForwardHigh ();
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

