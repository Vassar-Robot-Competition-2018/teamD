void GoHome () {
  if (prevQuad == Enemy) {
    if (quadrant == Neutral_2) {
      DriveForward();
      delay(1000);
      RightTurn();
      delay(900);
      while (quadrant != Home) {
        ColorSensor();
        DriveForward();
      }
    }
    if (quadrant == Neutral_1) {
      DriveForward();
      delay(1000);
      LeftTurn();
      delay(900);
      while (quadrant != Home) {
        ColorSensor();
        DriveForward();
      }
    }
  }
  if ((prevQuad == Neutral_1) && (quadrant == Enemy)) {
    DriveForward();
      delay(1000);
    RightTurn();
    while (quadrant != Neutral_2) {
      ColorSensor();
      DriveForward();
    }
    if (quadrant == Neutral_2) {
      DriveForward();
      delay(1000);
      RightTurn();
      delay(900);
      while (quadrant != Home) {
        ColorSensor();
        DriveForward();
      }
    }
  }
  if ((prevQuad == Neutral_2) && (quadrant == Enemy)) {
    DriveForward();
      delay(1000);
      LeftTurn();
    delay(900);
    while (quadrant != Neutral_1) {
      ColorSensor();
      DriveForward();
    }
    if (quadrant == Neutral_1) {
      DriveForward();
      delay(1000);
      LeftTurn();
      delay(900);
      while (quadrant != Home) {
        ColorSensor();
        DriveForward();
      }
    }
  }
else {
  ColorSensor();
    DriveForward();
    return;
  }
}

