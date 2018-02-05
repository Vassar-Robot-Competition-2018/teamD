# teamD

This past week, our goal was to complete the first two milestones:  1) Get the robot to exit a quadrant, and 2) Get the robot to back up and stop when the white border is detected.

In order to complete the first milestone, we needed a chassis, so we constructed a temporary one out of sheets of foam core. On this chassis, we attached a LiPo battery and two continuous-rotation servo motors and wheels to an Arduino Mega 2560 microcontroller. Here is the code that we used to drive the robot forward and complete first milestone:

'''

#include <Servo.h>

Servo servoL;         // Left motor

Servo servoR;         // Right motor

void setup() {

  Serial.begin(9600);

  servoL.attach(50);  // attaches left servo on pin 50

  servoR.attach(51);  // attaches left servo on pin 51

}

void loop() {

  servoL.write(45); // forward at half speed

  servoR.write(135);

}

'''

Next, we added a QRE1113 line sensor to our robot. This is a short range analog sensor that  returns lower values when lighter colors are detected and higher values for darker colors. The sensor was place at the front of the robot, pointing downward, almost touching the ground. We found that the robot returned values less than 200 when detecting white and values over 600 when detecting the gray color of the arena, so in our code we used a threshold value of 400. Whenever the sensor read values less than 400, it was programed to back up for one second, stop for one second, and then resume traveling forward. Here is the “loop” part of our code that was used to complete the second milestone:


'''

void loop() {
QRE_Value = analogRead(QRE1113_Pin);
Serial.println(QRE_Value);  // prints line sensor value to serial monitor

// values less than 400 are treated as white
if (QRE_Value >= 400) {     // no white detected
  digitalWrite(LED, LOW);
  servoL.write(45);         // forward at half speed
  servoR.write(135);
}

else {  // white detected
  digitalWrite(LED, HIGH);  // turns on LED
  servoL.write(135);        // backwards at half speed
  servoR.write(45);
  delay(1000);              // time in milliseconds
  servoL.write(90);         // motors stop
  servoR.write(90);
  delay(1000);              // time in milliseconds
}
}

'''

Initially, we had the servos receiving power from the “5V” on the microcontroller, but the servos behaved erratically and inconsistently. After switching the servos from “5V” to “Vin”, the servos began responding correctly.

Next, we will focus our efforts on the Pixy Cam in order to get our robot to detect colored blocks. We hope to complete at least two more milestones this coming week.
