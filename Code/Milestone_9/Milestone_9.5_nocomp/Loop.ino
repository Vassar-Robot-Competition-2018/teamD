//void loop() {
//  ColorSensors();
//  TOF();
//  CheckBlocks();
//  QuadrantLED();
//  ReleaseLED();
//  DriveForward();
//  Logic();
//}

/*void loop() {

  tempTimer = millis();
  while ((millis() - tempTimer) <= 6000) {

    course = North;
    UpdateHeading();

    Serial.print("heading (north): ");
    Serial.println(heading);
    Serial.print("                  error: ");
    Serial.println(headingError);
  }

  tempTimer = millis();
  while ((millis() - tempTimer) <= 6000) {
    course = East; current = millis();
    UpdateHeading();

    Serial.print("heading (east): ");
    Serial.println(heading);
    Serial.print("                  error: ");
    Serial.println(headingError);
  }
  }*/
  
void loop() {
  ColorSensors();
  TOFshort();
  CheckBlocks();
  StatusLED();
  QuadrantLED();
  Logic();
  //  HeadingLED();
  // DriveForward();
}
