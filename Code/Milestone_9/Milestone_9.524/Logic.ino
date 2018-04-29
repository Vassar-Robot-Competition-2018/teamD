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
  Rel_Table();
  captureQ = 0;
  oldQuad = 0;

  if (Captured()) {
    SetReleaseQ();
    Status = 3;
  }
  else if (quadrant == Home) {
    if ((maxSig == Neutral_1) || (maxSig == Neutral_2)) {
      captureB = maxSig;
      releaseQ = Enemy;
      Rel_Table();
      captureQ = quadrant;
      Status = 2;
    }
    else if (maxSig == Enemy) {
      captureB = maxSig;
      releaseQ = Neutral_1;
      Rel_Table();
      captureQ = quadrant;
      Status = 2;
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
      Rel_Table();
      captureQ = quadrant;
      Status = 2;
    }
    else if ((maxSig == Neutral_1) || (maxSig == Neutral_2)) {
      captureB = maxSig;
      releaseQ = Enemy;
      Rel_Table();
      captureQ = quadrant;
      Status = 2;
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
      Rel_Table();
      captureQ = quadrant;
      Status = 2;
    }
    else if (maxSig == Enemy) {
      captureB = maxSig;
      releaseQ = Neutral_1;
      Rel_Table();
      captureQ = quadrant;
      Status = 2;
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
      Rel_Table();
      captureQ = quadrant;
      Status = 2;
    }
    else if ((maxSig == Neutral_1) || (maxSig == Neutral_2)) {
      captureB = maxSig;
      releaseQ = Enemy;
      Rel_Table();
      captureQ = quadrant;
      Status = 2;
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
    if (Captured()) {
      Grab();
      UpdatePan(0);
      Status = 3;
    }
    else {
      TrackBlock();
      FollowBlock();
    }
  }
  else {
    Status = 1;
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
  }
  else if ((quadrant == Neutral_1) && (releaseQ == Neutral_2)) {
    Release();
  }
  else if ((quadrant == Neutral_2) && (releaseQ == Neutral_1)) {
    Release();
  }
  else if ((quadrant == CW_relQ) && (oldQuad == Diag_relQ)) {
    FWD_LeftTurn();
  }
  else if ((quadrant == Diag_relQ) && (oldQuad == CW_relQ)) {
    Back_RightTurn();
  }
  else if ((quadrant == Diag_relQ) && (oldQuad == CCW_relQ)) {
    Back_LeftTurn();
  }
  else if ((quadrant == CCW_relQ) && (oldQuad == Diag_relQ)) {
    FWD_RightTurn();
  }
  else {
    DriveForward();
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
  Rel_Table();
}
