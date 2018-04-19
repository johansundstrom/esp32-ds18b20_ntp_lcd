#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
WiFiMulti WiFiMulti;

const char* ssid     = "MdH-Guest";
const char* password = "Georgia201803";

const char* host = "be9.asuscomm.com";
const char* streamId   = "....................";
const char* privateKey = "....................";

void setup() {                
  Serial.begin(115200);
  delay(10);


  display.clearDisplay();
  // text display
  display.setTextSize(1);
  display.setTextColor(WHITE);
  Serial.println();
  Serial.print("Connecting to ");
  display.println("Connecting to ");
  Serial.println(ssid);
  display.println(ssid);

  WiFi.begin(ssid, password);
  // init done

  // Clear the buffer.
    Serial.println();
    Serial.println();
    display.setCursor(0,0);
    display.println("Wait for WiFi... ");
    Serial.println("Wait for WiFi... ");
    
    display.println("Wait for WiFi...");
    display.display();
    display.clearDisplay();
    
    int i = 0;
    while(WiFiMulti.run() != WL_CONNECTED) {
                
        Serial.print(".");
        display.print(".");
        display.display();
        i++;
        delay(500);
    }
  
  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.println(3.141592);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("0x"); display.println(0xDEADBEEF, HEX);
  display.display();
  delay(2000);
  display.clearDisplay();

}

void loop() {
  
 

}
