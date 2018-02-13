
This past week, we created an entirely new chasis based on the cad designs Jonathan sketched out, and printed the bot before beginnning class on Tuesday. Progressing towards the third milestone,(Signal that the robot has found a block), was our main objective. We were unsucessful in achieving this goal; although our progress towards it is exponetially better than that of a few short days ago.

To complete the first milestone, we had to create a foam chasis as a temporary solution. This weeks efforts were all based entirely on anew design. Our Arduino Mega 2560 microcontroller, bread boards, and servo motors all had prescribed indents in the chasis of our new 3-D printed chasis seen in video 238. The newest ittreation of the chasis also inlcudes a series of utility holes that can be used for placing atachements, or reinforcing the multipart frame. 

The included a pixy camera, which was the most notable difference in this version of the robot. This camera is unique in our situation, as it is mounted to a rotating build plated and actuated by a smaller servo controller on the upper deck of the build platform. This camera acts as the chief operator as it is used to communicate the position of found blocks. This addition served as both a leap forward and a large hurdle in completing our third objective. This camera can be seen distiguishing colors in the video labeled 199.

In reviewing the code from the first week of coding, a serious revision was needed in order to more clearly sort out errors that we were experiencing in navigation and locomotion. Sorting out the existing code from Milestone_3_V2 took up the majority of our in class time on Tuesday as served as a learning experience in how we must sort our code moving forward. Now all called functions can be found in a serperate thread of the code designated MAIN. 

The function listed below is the bread and butter of our approach to sorting blocks and identifying what actions to follow in the code. The function is listed in Functions.ino, and calls a few other functions which can also be found here.

````
void CheckBlocks() {
  static int i = 0;
  uint16_t blocks;
  char buf[32];

  maxJ = 0;
  maxSig = 0;
  max_X = 0;
  max_Y = 0;
  maxWidth = 0;
  maxHeight = 0;

  IR_Long();

  blocks = pixy.getBlocks();

  if (blocks) { //if a color sig is detected by pixy cam
    for (j = 0; j < blocks; j++) { //find the largest object that fits the signature
      prod = pixy.blocks[j].width * pixy.blocks[j].height;
      if (prod > maxProd)
        maxJ = j;
    }
  }

  if (pixy.blocks[maxJ].width > minWidth) { // if color sig is big enough and object is detected by the IR sensor
    maxSig = pixy.blocks[maxJ].signature;
    max_X = pixy.blocks[maxJ].x;
    max_Y = pixy.blocks[maxJ].y;
    maxHeight = pixy.blocks[maxJ].height;
    maxWidth = pixy.blocks[maxJ].width;

    BlockColor();
    PixyPID();
    WheelPID();
  }

  else {
    //Serial.println("None");
    digitalWrite(RED, LOW);    // LED
    digitalWrite(YELLOW, LOW);    // LED
    digitalWrite(GREEN, LOW);    // LED
    digitalWrite(BLUE, LOW);    // LED
    state = 0;
    StateCheck();
  }

  //  Serial.print("Block Position: (");
  //  Serial.print(max_X, max_Y);
  //  Serial.println(")");
  //  Serial.print("Block Dimensions: ");
  //  Serial.print(maxWidth);
  //  Serial.print(" x ");
  //  Serial.println(maxWidth);

  Serial.print("maxWidth: "); Serial.println(maxWidth);
  Serial.print("irLongDist: "); Serial.println(irLongDist);

  if ((irLongDist < 50) && (maxWidth > stopWidth)) {
    state = 1;
  }

  delay(30);
}
````
Initially, an introduction of the pixy code into our original lines resulted in a massive decresase in mobility and the camera. This same code worked just fine on as seperate Arduino rig. Scowering the code left no easy answer as to what the problem might be. We later identified that when the pixy camera is connected to the Arduino Mega, the series of digital outputs at the end of the board can not run simultaneously.

Next, we solved this problem by simply connecting the digital inputs to another pin set. The tracking of the bot was incredibly improved and our more highly optimized code lent to this improvement as well.


Finally, we added an RGB sensor to replace our QRE1113 line sensor as our primary line identifier. Doing so also switched from a strictly analog input based on an overage of a barrier. This will serve as a much better option for identifying the initial quadrant position 

Next week we will try to finish the calibration of the pixy cam, as well as actuallizing a pair of servos to serve as forward facing arms for wrangling blocks.
