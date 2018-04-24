void HomeLED() {
  if (Home == 1) {
    //Serial.println("Red");
    digitalWrite(RED_F, HIGH);    // LED
    digitalWrite(YELLOW_F, LOW);    // LED
    digitalWrite(GREEN_F, LOW);    // LED
    digitalWrite(BLUE_F, LOW);    // LED
  }

  else if (Home == 3) {
    //Serial.println("Yellow");
    digitalWrite(RED_F, LOW);    // LED
    digitalWrite(YELLOW_F, HIGH);    // LED
    digitalWrite(GREEN_F, LOW);    // LED
    digitalWrite(BLUE_F, LOW);    // LED
  }

  else if (Home == 2) {
    //Serial.println("Green");
    digitalWrite(RED_F, LOW);    // LED
    digitalWrite(YELLOW_F, LOW);    // LED
    digitalWrite(GREEN_F, HIGH);    // LED
    digitalWrite(BLUE_F, LOW);    // LED
  }

  else if (Home == 4) {
    //Serial.println("Blue");
    digitalWrite(RED_F, LOW);    // LED
    digitalWrite(YELLOW_F, LOW);    // LED
    digitalWrite(GREEN_F, LOW);    // LED
    digitalWrite(BLUE_F, HIGH);    // LED
  }

  else {
    digitalWrite(RED_F, LOW);    // LED
    digitalWrite(YELLOW_F, LOW);    // LED
    digitalWrite(GREEN_F, LOW);    // LED
    digitalWrite(BLUE_F, LOW);    // LED
  }
}

void QuadrantLED() {
  if (quadrant == 1) {
    //Serial.println("Red");
    digitalWrite(RED_B, HIGH);    // LED
    digitalWrite(YELLOW_B, LOW);    // LED
    digitalWrite(GREEN_B, LOW);    // LED
    digitalWrite(BLUE_B, LOW);    // LED
  }

  else if (quadrant == 3) {
    //Serial.println("Yellow");
    digitalWrite(RED_B, LOW);    // LED
    digitalWrite(YELLOW_B, HIGH);    // LED
    digitalWrite(GREEN_B, LOW);    // LED
    digitalWrite(BLUE_B, LOW);    // LED
  }

  else if (quadrant == 2) {
    //Serial.println("Green");
    digitalWrite(RED_B, LOW);    // LED
    digitalWrite(YELLOW_B, LOW);    // LED
    digitalWrite(GREEN_B, HIGH);    // LED
    digitalWrite(BLUE_B, LOW);    // LED
  }

  else if (quadrant == 4) {
    //Serial.println("Blue");
    digitalWrite(RED_B, LOW);    // LED
    digitalWrite(YELLOW_B, LOW);    // LED
    digitalWrite(GREEN_B, LOW);    // LED
    digitalWrite(BLUE_B, HIGH);    // LED
  }

  else {
    digitalWrite(RED_B, LOW);    // LED
    digitalWrite(YELLOW_B, LOW);    // LED
    digitalWrite(GREEN_B, LOW);    // LED
    digitalWrite(BLUE_B, LOW);    // LED
  }
}


void BlockColor () {
  if (maxSig == 1) {
    //Serial.println("Red");
    digitalWrite(RED_B, HIGH);    // LED
    digitalWrite(YELLOW_B, LOW);    // LED
    digitalWrite(GREEN_B, LOW);    // LED
    digitalWrite(BLUE_B, LOW);    // LED
  }

  else if (maxSig == 3) {
    //Serial.println("Yellow");
    digitalWrite(RED_B, LOW);    // LED
    digitalWrite(YELLOW_B, HIGH);    // LED
    digitalWrite(GREEN_B, LOW);    // LED
    digitalWrite(BLUE_B, LOW);    // LED
  }

  else if (maxSig == 2) {
    //Serial.println("Green");
    digitalWrite(RED_B, LOW);    // LED
    digitalWrite(YELLOW_B, LOW);    // LED
    digitalWrite(GREEN_B, HIGH);    // LED
    digitalWrite(BLUE_B, LOW);    // LED
  }

  else if (maxSig == 4) {
    //Serial.println("Blue");
    digitalWrite(RED_B, LOW);    // LED
    digitalWrite(YELLOW_B, LOW);    // LED
    digitalWrite(GREEN_B, LOW);    // LED
    digitalWrite(BLUE_B, HIGH);    // LED
  }
  else {
    digitalWrite(RED_B, LOW);    // LED
    digitalWrite(YELLOW_B, LOW);    // LED
    digitalWrite(GREEN_B, LOW);    // LED
    digitalWrite(BLUE_B, LOW);    // LED
  }
}
