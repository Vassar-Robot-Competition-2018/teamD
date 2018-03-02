This week we successfully completed milestones 3, 5, and 6 on Tuesday in class and milestone 4 on Friday. Milestone 3 consisted of the robot successfully finding a block and signaling the color of the block. In order to finally complete this milestone, we had found that putting the servo pins into the digital ports on the Arduino Mega microcontroller had caused the servos to stop working while the pixy cam was active. We fixed this issue by plugging the servo pins into the PWM ports. After rewiring the pins, we were able to use our previous code in our last update to complete milestone 3. 

To complete milestone 5 our robot needed to be randomly placed in a quadrant and signify which quadrant it is in. This is the main function for milestone 5 with its coordinating functions below:

````
void loop() {
ColorSensor(); // includes ColorCalibration, WhiteCheck, and Quadrant functions
StateCheck();
}

void ColorSensor() {  //checks floor color sensor (up to 20 times per second)
int clear, red, green, blue;
tcs.setInterrupt(false);      // turn on LED
tcs.getRawData(&red, &green, &blue, &clear);
tcs.setInterrupt(true);  // turn off LED

if (Cgray == 0) {
ColorCalibration(clear, red, green, blue); }

WhiteCheck(clear);
Quadrant(red, green, blue);
}

void SetHome(int r, int g, int b) {
if ((homeQ == 0) && (quadrant != 0)) {
homeQ = quadrant; }
}

void Quadrant(int r, int g, int b) {
if (r > (g + b))  //red
quadrant = 1;
else if ((r + g) > (4 * b))  //yellow
quadrant = 2;
else if ((3 * g) > ((r + b) * 2))  //green
quadrant = 3;
else if (b > (2 * r))  //blue
quadrant = 4;

SetHome(r, g, b);

if (quadrant != quadCheck) {
prev_quadrant = quadCheck;
quadCheck = quadrant; }

digitalWrite((2), LOW);    // LED
digitalWrite((3), LOW);    // LED
digitalWrite((4), LOW);    // LED
digitalWrite((5), LOW);    // LED
digitalWrite((quadrant + 1), HIGH);    // LED

if(quadrant != 0) {
DriveStop();
delay(5000);}
}


void ColorCalibration(int c, int r, int g, int b) {
Cgray = c;
Rgray = r;
Ggray = g;
Bgray = b;
}


void WhiteCheck(int c) {
if (c > (4 * Cgray)) { 
DriveReverse();
delay(1000);
RightTurn();
delay(1000); }
}


void StateCheck() {
if (state == 0) { // if searching for block
DriveForward(); }
}
````

Milestone 6 was very similar to milestone 5, however instead of stopping after reading a color with the floor RGB sensor, it continued to drive forward and light up the last seen color that it sensed on the floor and needed to reach every quadrant and go back to its home quadrant in under a minute. In order to have the robot pass through every quadrant, it would drive forward until it hits the white line, rotate, and continue, which eventually resulted in the robot driving through every quadrant. In order to have the LED display the current quadrant color we implemented the same code as milestone 5, but changed the Quadrant function: 

````
void Quadrant(int r, int g, int b) {
if (r > (g + b))  //red
quadrant = 1;
else if ((r + g) > (4 * b))  //yellow
quadrant = 2;
else if ((3 * g) > ((r + b) * 2))  //green
quadrant = 3;
else if (b > (2 * r))  //blue
quadrant = 4;

SetHome(r, g, b);

if (quadrant != quadCheck) {
prev_quadrant = quadCheck;
quadCheck = quadrant; }

digitalWrite((prev_quadrant + 1), LOW);    // LED
digitalWrite((quadrant + 1), HIGH);    // LED
}

````

This week we added a foam core extension to the front lower level of the robot to support metal arms for the robot to capture blocks with. The arms are connected to two servos that allow the arms to swing open and closed. We later realized that we would not need to use the two new servos at the moment in order to complete milestone 4.

We had difficulties with milestone 4 because as our robot would roam the arena in order to find a block, it would recognize the tape as a block. Thus, to compensate for the robot sensing the tape instead of the blocks we added a long range IR sensor on top of the pixie cam and a short range IR sensor vertically to the right of the pixie cam. This allowed us to include the condition of sensing an object within a certain range in front of it along side sensing the color in order for the robot to conclude that the robot sees a block. We also implemented a PID code in order for the robot to turn towards the block as to center the block in the pixy cam's view. This is our code for milestone 4:

````
void loop() {
ColorSensor(); // includes ColorCalibration, WhiteCheck, and Quadrant functions
IR_Short();
IR_Long();

if (irShortDist > captured) {
CheckBlocks(); }

else { 
DriveForward();
digitalWrite(WHITE, LOW);    // LED}
}

void PID_Pixy_Wheels() {
if (irLongDist <= minDist) {
digitalWrite(WHITE, HIGH);    // LED
pixyError = pixySP - max_X;
wheelAdj = wheelKp * pixyError + wheelKd * (pixyError - lastPixyError);
lastPixyError = pixyError;
speedL = int(baseSpeedL + wheelAdj);   // not sure yet if it's L minus adjust or R minus adjust
speedR = int(baseSpeedR - wheelAdj);

if (speedL > maxSpeedL)
speedL = maxSpeedL;
else if (speedL < minSpeedL)
speedL = minSpeedL;
if (speedR > maxSpeedR)
speedR = maxSpeedR;
else if (speedR < minSpeedR)
speedR = minSpeedR;

MotorUpdate(speedL, speedR);
}
else
digitalWrite(WHITE, LOW);    // LED
}

void MotorUpdate(double L, double R) {
double servoL_speed = (L * (-0.9)) + 92;
double servoR_speed = (R * 0.9 ) + 92;
servoL.write(servoL_speed);
servoR.write(servoR_speed);
}

void IR_Short() {
irShort = analogRead(IR_SHORT);

if ((irShort > 100) && (irShort < 700)) {
irShortDist = 3187.58 / irShort - 1.6016;}
else {
irShortDist = 100;}
}


void IR_Long() {
irLong = analogRead(IR_LONG);

if ((irLong > 70) && (irLong < 700)) {
irLongDist = 6206.62 / irLong - 0.1826;}
else {
irLongDist = 100;}
}

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

blocks = pixy.getBlocks();

if (blocks) { //if a color sig is detected by pixy cam
for (j = 0; j < blocks; j++) { //find the largest object that fits the signature
prod = pixy.blocks[j].width * pixy.blocks[j].height;
if (prod > maxProd)
maxJ = j; }
}

if ((pixy.blocks[maxJ].width > minWidth) && ((pixy.blocks[maxJ].width < lineWidth) && (pixy.blocks[maxJ].height < lineHeight))) { // if color sig is big enough and object is detected by the IR sensor
maxSig = pixy.blocks[maxJ].signature;
max_X = pixy.blocks[maxJ].x;
max_Y = pixy.blocks[maxJ].y;
maxHeight = pixy.blocks[maxJ].height;
maxWidth = pixy.blocks[maxJ].width;

BlockColor();
PID_Pixy_Wheels();
}

else {
digitalWrite(RED, LOW);    // LED
digitalWrite(YELLOW, LOW);    // LED
digitalWrite(GREEN, LOW);    // LED
digitalWrite(BLUE, LOW);    // LED
state = 0;
StateCheck(); }

delay(30);
}

````

This week Jonathan also created a new chassis design which improves our robot's arms, adds IR sensors to the sides of our robot, and better balances out the weight of the battery on the robot by creating three tiers (adding an intermediate tier for the breadboard). Below is a picture of our current iteration of our robot. 

Next week we plan to print our new chassis, better refine our robot's PID program and ability to differentiate blocks from tape lines, and work towards milestone 7. 