
This week our group continued to work on our code in order to complete milestone 8 and perfected our PID code. One thing that we did to improve the code is that we changed the way the robot senses whether or not it has captured a block. Previously we had IR sensors to sense if the object was directly in front of the robot, however this didn't seem to work as the short range IR sensor was unable to read accurate measurements when the block was less than 3 cm away. So this week we decided to rely on the pixy cam measurements inside of the code to determine if the block was captured. We tested that when the block is captured, the height is greater than 80 and the width is greater than 105. Below is the code that allowed us to do this. 

````
boolean Captured() {
  if (maxSig != 6) {
    if ((maxHeight > 80)  && (maxWidth > 105)) {
    return true;
  }
  else return false;
  }
else return false;
}
````

In order to test this code to complete milestone 8, which is to find a block that matches the color of your home quadrant and bring it back to your home quadrant, we simulated opening and closing our robots arms by lighting up a green LED when the arms would be closed. Below is our main loop for milestone 8 with LEDs in place of the arms opening and closing. 

````
void loop() {
  ColorSensor();

  if (Captured()) { //Block Captured
    DriveForward();
    delay (1000);
    DriveStop();
    digitalWrite(GREEN_B, HIGH);
    digitalWrite(RED_B, LOW);
    delay (3000);
    //GrabBlock();
    // digitalWrite(WHITE, HIGH);
    while (quadrant != homeQuad) {
      ColorSensor();
      DriveForward();
    }
    DriveStop();
    digitalWrite(GREEN_B, LOW);
    digitalWrite(RED_B, HIGH);
    delay (4000);
  }
  else {
    CheckBlocks();
    digitalWrite(RED_B, LOW);
    digitalWrite(YELLOW_B, LOW);
    digitalWrite(GREEN_B, LOW);
    digitalWrite(BLUE_B, LOW);
  }
}
````

This week we ordered a servo driver to solve our issues with our servos, a AA battery holder to supply battery to our servos, and a time of flight distance sensor to help us indicate how close we are to a block. We are working on creating a new chassis for our robot in order to incorporate the items we just purchased and new design features, such as adding another line color sensor in order to have one on the bottom of each side of the front of the robot, strengthening the design of our arms, and reducing the overall size of the robot. We also found it helpful to add another set of four LEDs to our LED breadboard in order to display multiple colors in order to help us debug and improve our code. 
Next week we hope to implement this code we have been working on on our new chassis design with the arms to ultimately complete milestone 8. 
