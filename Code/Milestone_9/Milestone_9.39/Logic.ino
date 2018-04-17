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

  if ((TOF_distance < 100) && (releaseQ == 0)) {
    SetReleaseQ();
  }

  HeadingRelease();
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


void HeadingRelease() {
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

