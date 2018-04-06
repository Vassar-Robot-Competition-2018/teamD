void Status_0() {
  DriveForward();
}

void Status_1() {
  UpdateArms(100, 100);
  if (quadrant = Home) {
    if (HQ_count != 0) { // only for solo runs
      if ((maxSig == Neutral_1) || (maxSig == Neutral_2)) {
        captureB = maxSig;
        releaseQ = Enemy;
        captureQ = quadrant;
        Status = 2;
      }
      else if (maxSig == Enemy) {
        captureB = maxSig;
        releaseQ = Neutral_1;
        captureQ = quadrant;
        Status = 2;
      }
      else {
        Scan();
        DriveForward();
      }
    }
    else {
      DriveForward();
    }
  }
  else if (quadrant = Neutral_1) {
    if (NQ1_count != 0) {
      if (maxSig == Home) {
        captureB = maxSig;
        releaseQ = Home;
        captureQ = quadrant;
        Status = 2;
      }
      else if ((maxSig == Neutral_1) || (maxSig == Neutral_2)) {
        captureB = maxSig;
        releaseQ = Enemy;
        captureQ = quadrant;
        Status = 2;
      }
      else {
        Scan();
        DriveForward();
      }
    }
    else {
      DriveForward();
    }
  }
  else if (quadrant = Enemy) {
    if (EQ_count != 0) {
      if (maxSig == Home) {
        captureB = maxSig;
        releaseQ = Home;
        captureQ = quadrant;
        Status = 2;
      }
      else if (maxSig == Enemy) {
        captureB = maxSig;
        releaseQ = Neutral_1;
        captureQ = quadrant;
        Status = 2;
      }
      else {
        Scan();
        DriveForward();
      }
    }
    else {
      DriveForward();
    }
  }
  else if (quadrant = Neutral_2) {
    if (EQ_count != 0) {
      if (maxSig == Home) {
        captureB = maxSig;
        releaseQ = Home;
        captureQ = quadrant;
        Status = 2;
      }
      else if ((maxSig == Neutral_1) || (maxSig == Neutral_2)) {
        captureB = maxSig;
        releaseQ = Enemy;
        captureQ = quadrant;
        Status = 2;
      }
      else {
        Scan();
        DriveForward();
      }
    }
    else {
      DriveForward();
    }
  }
}

void Status_2() {
  UpdateArms(0, 0);
  if ((captureQ == quadrant) && (maxSig == captureB)) {
    if (Captured()) {
      Grab();
      UpdatePan(0);
      Status = 3;
    }
    else {
      Track();
      Follow();
    }
  }
  else {
    Status = 1;
  }
}

void Status_3() { //Once ToF sensor is added, check ToF sensor in Status_3 to see if block is still captured. If not, set Status to 1.
  if (quadrant == releaseQ) {
    Release();
    if (captureQ == Home) {
      HQ_count -= 1;
    }
    else if (captureQ == Neutral_1) {
      NQ1_count -= 1;
    }
    else if (captureQ == Enemy) {
      EQ_count -= 1;
    }
    else if (captureQ == Neutral_2) {
      NQ2_count -= 1;
    }
    Status = 1;
  }
  else {
    DriveForward();
  }
}

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

