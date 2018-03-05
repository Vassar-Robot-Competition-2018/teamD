#include <SPI.h>
#include <Pixy.h>

int Red = 8,          // Assigns Red LED to pin 8
    Yellow = 9,       // Assigns Yellow LED to pin 9
    Green = 10,       // Assigns Green LED to pin 10
    Blue = 11,        // Assigns Blue LED to pin 8
    j,
    maxProd,
    prod,
    maxJ,
    maxSig,
    max_X,
    max_Y,
    minWidth = 100,
    maxWidth,
    maxHeight;


Pixy pixy;

void setup()
{
  pinMode(Red, OUTPUT);
  pinMode(Yellow, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);


  Serial.begin(9600);

  pixy.init();
}
void loop()
{
  static int i = 0;
  uint16_t blocks;
  char buf[32];

  blocks = pixy.getBlocks();

  if (blocks) //if something is detected
  {
    for (j = 0; j < blocks; j++) { //find the largest object that fits the signature
      prod = pixy.blocks[j].width * pixy.blocks[j].height;
      //     skip = 0;
      if (prod > maxProd)
        maxJ = j;
    }
  }

  if (pixy.blocks[maxJ].x > minWidth) {
    maxSig = pixy.blocks[maxJ].signature;
    max_X = pixy.blocks[maxJ].x;
    max_Y = pixy.blocks[maxJ].y;
    maxWidth = pixy.blocks[maxJ].width;
    maxHeight = pixy.blocks[maxJ].height;

    if (maxSig == 1) {
      Serial.println("Red");
      digitalWrite(Red, HIGH);    // LED
      digitalWrite(Yellow, LOW);    // LED
      digitalWrite(Green, LOW);    // LED
      digitalWrite(Blue, LOW);    // LED
    }

    else if (maxSig == 2) {
      Serial.println("Yellow");
      digitalWrite(Red, LOW);    // LED
      digitalWrite(Yellow, HIGH);    // LED
      digitalWrite(Green, LOW);    // LED
      digitalWrite(Blue, LOW);    // LED
    }

    else if (maxSig == 3) {
      Serial.println("Green");
      digitalWrite(Red, LOW);    // LED
      digitalWrite(Yellow, LOW);    // LED
      digitalWrite(Green, HIGH);    // LED
      digitalWrite(Blue, LOW);    // LED
    }

    else if (maxSig == 4) {
      Serial.println("Blue");
      digitalWrite(Red, LOW);    // LED
      digitalWrite(Yellow, LOW);    // LED
      digitalWrite(Green, LOW);    // LED
      digitalWrite(Blue, HIGH);    // LED
    }
  }

  else {
    maxSig = 0;
    max_X = 0;
    max_Y = 0;
    maxWidth = 0;
    maxHeight = 0;

    Serial.println("None");
    digitalWrite(Red, LOW);    // LED
    digitalWrite(Yellow, LOW);    // LED
    digitalWrite(Green, LOW);    // LED
    digitalWrite(Blue, LOW);    // LED
  }

  Serial.print("Block Position: (");
  Serial.print(max_X, max_Y);
  Serial.println(")");
  Serial.print("Block Dimensions: ");
  Serial.print(maxWidth);
  Serial.print(" x ");
  Serial.println(maxWidth);

  maxJ = 0;
  maxSig = 0;
  max_X = 0;
  max_Y = 0;
  maxWidth = 0;
  maxHeight = 0;

  delay(30);
}

