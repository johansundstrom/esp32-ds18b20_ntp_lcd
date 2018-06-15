const char *ssid     = "MdH-Guest";
const char *password = "Houston201804";
int DS18S20_pin = 18;   //ds18b20 --> ESP32(D18)
int LCD_address = 0x3f; //I2C address (0x3f)
int LCD_chars = 16;
int LCD_rows = 2;


// Grader Celcius-tecken
uint8_t centigrade[8] = {
  B01000,
  B10100,
  B01000,
  B00011,
  B00100,
  B00100,
  B00011,
  B00000
};
