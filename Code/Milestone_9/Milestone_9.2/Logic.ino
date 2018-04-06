void GoHome () {
  if (prevQuad == Enemy) {
    if (quadrant == Neutral_2) {
      RightTurn();
    }
    if (quadrant == Neutral_1) {
      LeftTurn();
    }
  }
  if ((prevQuad == Neutral_1) && (quadrant == Enemy)) {
    RightTurn();
  }
  if ((prevQuad == Neutral_2) && (quadrant == Enemy)) {
    LeftTurn();
  }
}

