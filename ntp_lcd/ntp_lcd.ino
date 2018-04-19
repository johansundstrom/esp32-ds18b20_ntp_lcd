//Initial I2C LCD example from:
// http://arduino-info.wikispaces.com/LCD-Blue-I2C

// Initial DS18B20 example from:
// http://bildr.org/2011/07/ds18b20-arduino/

// Read the DS18B20 and then write to an I2C LCD and serial monitor
// http://www.hoaglun.com/blog/2013/1/31/arduino-ds18b20-i2c-lcd

#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h> 
#include <OneWire.h> 
#include <LiquidCrystal_I2C.h>

const char *ssid     = "MdH-Guest";
const char *password = "Houston201804";
int DS18S20_Pin = 18; //DS18S20 Signal pin on digital x

//Temperature chip i/o
OneWire ds(DS18S20_Pin);  // on digital pin 
LiquidCrystal_I2C lcd(0x3f,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
WiFiUDP ntpUDP;

// By default 'time.nist.gov' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP);

// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
// NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  timeClient.begin();
  lcd.init();                      // initialize the lcd 

    // Print a message to the LCD.
    lcd.backlight();
}

void loop() {
  timeClient.update();

  Serial.println(timeClient.getFormattedTime());

    float tempsensor = getTemp();
    Serial.println(tempsensor);

    lcd.setCursor(0, 0);
    lcd.print(timeClient.getFormattedTime());

    lcd.setCursor(0, 1); 
    lcd.print(tempsensor);
    lcd.print("\337C   ");
    
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
