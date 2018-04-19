//Initial I2C LCD example from:
//http://arduino-info.wikispaces.com/LCD-Blue-I2C

//Initial DS18B20 example from:
//http://bildr.org/2011/07/ds18b20-arduino/

//Read the DS18B20 and then write to an I2C LCD and serial monitor
//http://www.hoaglun.com/blog/2013/1/31/arduino-ds18b20-i2c-lcd

//LCD_SCL-->ESP32_D22
//LCD_SDA-->ESP32_D21
//DS18B20-->ESP32_D18

#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h> 
#include <OneWire.h> 
#include <LiquidCrystal_I2C.h>
#include "config.h"

const char *ssid     = "MdH-Guest";
const char *password = "Houston201804";
int DS18S20_pin = 18;   //ds18b20 --> ESP32(D18)
int LCD_address = 0x3f; //I2C address (0x3f)
int LCD_chars = 16;
int LCD_rows = 2;

OneWire ds(DS18S20_pin);
LiquidCrystal_I2C lcd(LCD_address, LCD_chars, LCD_rows);
WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600*2, 60000);
// par.1 - ntpUDP 
// par.2 - ntp-pool (time.nist.gov}
// par.3 - offset in S(central european summer time = 3600*2)
// par.4 - update interval in mS (60k = varje minut)

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

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }
  timeClient.begin();

  //förbered display
  lcd.backlight();
  lcd.init();
  lcd.begin(16, 2);
  lcd.createChar(0, centigrade);
}

void loop() {
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());

  float tempsensor = getTemp();
  Serial.println(tempsensor);

  lcd.setCursor(0, 0);
  lcd.print(timeClient.getFormattedTime());
  lcd.print(" CEST");

  lcd.setCursor(0, 1); 
  lcd.print(tempsensor);
  //lcd.print("\337C   ");
  lcd.print((char)0); // Custom char
  lcd.print("   ");
  
  delay(100); //just here to slow down the output so it is easier to read
}



float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad

  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  
  ds.reset_search();
  
  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  
  return TemperatureSum;
}
