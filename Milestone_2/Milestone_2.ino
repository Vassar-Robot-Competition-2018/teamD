// This program causes the robot to drive forward until the downward-pointing QRE1113 line sensor detects a white surface.
// This causes the robot to drive backwards for 1 second, stop for 1 second, and then resume going forward. An LED also
// lights up when a white surface is detected.

#include <Servo.h>

Servo servoL;         // Left motor
Servo servoR;         // Right motor
int QRE1113_Pin = 0;  // connected to analog pin 0
int LED = 5;          // LED
int QRE_Value = 0;    // Line sensor reading
void setup() {
  Serial.begin(9600);
  servoL.attach(50);  // attaches left servo on pin 50
  servoR.attach(51);  // attaches left servo on pin 51
  pinMode(LED, OUTPUT);
}

void loop() {
  QRE_Value = analogRead(QRE1113_Pin);
  Serial.println(QRE_Value);    // prints line sensor value to serial monitor

  // values less than 400 are treated as white
  if (QRE_Value >= 400) {   // no white detected
    digitalWrite(LED, LOW);
    servoL.write(45); // forward at half speed
    servoR.write(135);
    //   delay(10);
  }
  else {  // white detected
    digitalWrite(LED, HIGH);    // turns on LED
    servoL.write(135); // backwards at half speed
    servoR.write(45);
    delay(1000);      // time in milliseconds
    servoL.write(90); // motors stopped
    servoR.write(90);
    delay(1000);      // time in milliseconds
  }
}
