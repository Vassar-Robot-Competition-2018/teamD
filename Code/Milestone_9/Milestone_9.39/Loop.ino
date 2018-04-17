void loop() {
  ColorSensors();
  TOF();
  CheckBlocks();
  QuadrantLED();
  ReleaseLED();
  DriveForward();
  Logic();
}

//void loop() {
//
//    course = North;
//    UpdateHeading();
//
//    Serial.print("heading (north): ");
//    Serial.println(heading);
//    Serial.print("                  error: ");
//    Serial.println(headingError);
//}
