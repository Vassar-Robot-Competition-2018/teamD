coverage: 

This week served as a time to see a number of breakthroughs in the way our robot functions. Our V5 chassis seems to be our last iteration as there were no physical changes since last week. 

Our time of flight range sensor was swapped out for a more close proximity edition and it has served much better as a discriminator between captured and free blocks. The range is much more limited by comparison, but its closer proximity provides much more usable data. 

We also changed our motors to run through VIN outputs, so our output speed has increased exponentially. We are "cruisin’ now". 

We have also spent some changing the calibration of our PID, specifically focusing on our P and D values. 

Sunday night served as a large turning point for our code as a significant portion of our logic had been expanded upon and narrowed down in other areas. We updated the logic of what the robot should be doing in order to find its way to the correct release quadrant depending on the color of the block in relation to our home quadrant. 

```
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
      DriveForward();
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
      DriveForward();
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
      DriveForward();
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
      DriveForward();
    }
  }
  else {
    DriveForward();
  }
  oldQuad = quadrant;
}
```

Things are looking good for our upcoming competition, and we have made some really good strides these last couple of weeks. Our final change might come in the form of a physical change to the robots exterior to make it more aesthetically pleasing to coincide with our groups adopted theme of The Beatles.
