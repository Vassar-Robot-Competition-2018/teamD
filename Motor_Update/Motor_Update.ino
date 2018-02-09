// speedL and speedR range from -100 (full reverse) to 100 (full forward). motor_update converts these speeds to servo motor commands.
//
// This program causes the robot to drive forward until the downward-pointing QRE1113 line sensor detects a white surface.
// This causes the robot to drive backwards for 1 second, turns for 2 second, and then resume going forward. An LED also
// lights up when a white surface is detected

#include <Servo.h>

Servo servoL;         // left motor
Servo servoR;         // right motor

int QRE1113_Pin = 0,  // connect to analog pin 0
    LED = 5,          // LED
    QRE_Value = 0,    // line sensor value
    speedL = 0,
    speedR = 0;

void setup() {
  Serial.begin(9600);
  servoL.attach(50);  // attaches left servo on pin 50
  servoR.attach(51);  // attaches left servo on pin 51
  pinMode(LED, OUTPUT);
}

void motor_update(int speedL, int speedR) {
  int servoL_speed = (-9) / 10 * speedL + 90;
  int servoR_speed = 9 / 10 * speedR + 91;
  servoL.write(servoL_speed);
  servoR.write(servoR_speed);
}

void loop() {
  QRE_Value = analogRead(QRE1113_Pin);
  Serial.println(QRE_Value);  // prints line sensor value to serial monitor

  // values less than 400 are treated as white
  if (QRE_Value >= 400) {     // no white detected
    digitalWrite(LED, LOW);
    speedL = 50;         // forward at half speed
    speedR = 50;

    motor_update(speedL, speedR);
  }

  else {  // white detected
    digitalWrite(LED, HIGH);  // turns on LED
    speedL = -50;         // reverse at half speed
    speedR = -50;

    motor_update(speedL, speedR);
    delay(1000);              // time in milliseconds

    speedL = -50;         // reverse at half speed
    speedR = 50;

    motor_update(speedL, speedR);
    delay(2000);              // time in milliseconds
  }

  motor_update(speedL, speedR);
}
