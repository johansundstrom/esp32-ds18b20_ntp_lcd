#include <Wire.h>
#include "SSD1306.h" 

const int mySDA = 21;
const int mySCL = 22;

SSD1306  display(0x3c, mySDA, mySCL);

void setup() {
  display.init();
  //display.flipScreenVertically();
  display.drawString(0, 0, "Johan Sundström");
  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:

}
