#include <Wire.h>
#include "SSD1306.h" 

const int SDA = 21;
const int SCL = 22;

SSD1306 display(0x3f, SDA, SCL);

void setup() {
  display.init();
  display.flipScreenVertically();
  display.drawString(0, 0, "Johan Sundström");
  display.display();
}

void loop() {
}
