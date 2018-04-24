void loop() {
  ColorSensors();
  TOF();
  CheckBlocks();
  HeadingLED();
  StatusLED();
  DriveForward();
  Logic();
}

//void loop() {
//Heading();
//
////
//    Serial.print("heading: ");
//    Serial.println(heading);
//    Serial.print("                  North: ");
//    Serial.println(North);
//    delay(200);
//}
