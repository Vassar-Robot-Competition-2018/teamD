void loop() {
  ColorSensors();
  TOF();
  CheckBlocks();
  ReleaseLED();
  StatusLED();
  Logic();
}
