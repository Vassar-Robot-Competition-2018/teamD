The way in which we were first approaching the identification of blocks was to set a continuous sweep function related to the servo attached to the Pixy Camera. In practice, this feature turned out to be less helpful than we had anticipated. More often than not, the camera would simply turn a blind eye to potential blocks and thus need to make another sweep in the general area to find it. So, the fate of the sweeping camera seems bleak in future iterations of our robot. We assembled the bot in its entirety on the new body which took up most of our time during this week, and the new bot can be seen in version 3. 

Johnathan was very dedicated to solving an earlier issue that had arisen with attaining false positives when passing in front of the colored tape of the arena. To alleviate this, he experimented at home to derive a formulaic comparison to distance from a block gathered from an IR sensor to the expected width of the color response.This was applied to our existing code and even introduced discriminatory lines from reading a starting quadrant and identifying matching blocks. It is seen below:

````

 void CheckBlocks() {
  // for (int k = 0; k < 10; k++) {
  static int i = 0;
  uint16_t blocks;
  char buf[32];

  maxJ = 0;
  maxSig = 0;
  max_X = 0;
  max_Y = 0;
  maxWidth = 0;
  maxHeight = 0;
  prod = 0;
  maxProd = 0;

  if (homeQuad != 0) {
    blocks = pixy.getBlocks();

    if (blocks) { //if a color sig is detected by pixy cam
      for (j = 0; j < blocks; j++) { //find the largest signature
        if (pixy.blocks[j].signature == homeQuad) {
          prod = pixy.blocks[j].width * pixy.blocks[j].height;
          if (prod > maxProd) {
            maxProd = prod;
            maxJ = j;
          }
        }
      }
      maxSig = pixy.blocks[maxJ].signature;
      max_X = pixy.blocks[maxJ].x;
      max_Y = pixy.blocks[maxJ].y;
      maxHeight = pixy.blocks[maxJ].height;
      maxWidth = pixy.blocks[maxJ].width;

      //    Serial.print("Block Position: ");
      //    Serial.print(max_X);
      //    Serial.print("     ");
      //    Serial.print("Block Width: ");
      //    Serial.print(maxWidth);
      //    Serial.print(" x ");
      //    Serial.println(maxWidth);

      //int checkDist = 1500 / irShortDist;
      //if (((800 / irShortDist) < pixy.blocks[maxJ].width) && ((1400 / irShortDist) > pixy.blocks[maxJ].width)) {
      //  if ((checkDist > pixy.blocks[maxJ].width) && (checkDist > pixy.blocks[maxJ].height)) {
      if (irPixyDist < 30);
      // BlockColor();
      digitalWrite(GREEN, LOW);
      digitalWrite(YELLOW, HIGH);
      FollowBlock();
      //}
    }

    else {
      //Serial.println("None");
      //    digitalWrite(RED, LOW);    // LED
      //    digitalWrite(YELLOW, LOW);    // LED
      //    digitalWrite(GREEN, LOW);    // LED
      //    digitalWrite(BLUE, LOW);    // LED

      DriveForward();
      digitalWrite(GREEN, HIGH);
      digitalWrite(YELLOW, LOW);
    }
  }
  else DriveForward();
  //}
}

````


This introduction of IR centers is also present with a new found strategy to identify blocks. The forward, left, and right facing sides of the chassis all have IR sensors placed in them in order to identify if any block happens to be in range, and if one is found it promptly halts its course and re-adjusts. This feature has not yet been tested, but the core of it can be seen in the above code under IR_Pixy, Left, and Right. These functions are our most current code priority, lead by Johnathan, and Parker, and with Devon desperately trying to make a meaningful contribution. 

Refining the actual way in which the PID is handled is our next step, as our current version is experiencing some jitters possibly due to a lack of power present. Testing this feature, among the others of which code can be found above, has been difficult as the final assembly of our new bot chassis proved to be problematic as a few key mounts separated as they were not able to hold on to pressures associated with traditional movement. We believe this to be caused by the nature of the material, ABS as opposed to our previous attempts to use PLA which proved to be much more successful. We plan on printing and installing a new base mount constructed of PLA during the next class period. 

Our goals for this week are to finish rebuilding the chassis once more and hopefully provide some meaningful applications of the multiple IR sensors. 
