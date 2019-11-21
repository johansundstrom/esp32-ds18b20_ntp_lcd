/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
  https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/
  
  GPIO22 - SCL
  GPIO21 - SDA
*********/

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

// Replace the next variables with your SSID/Password combination
const char* ssid = "ssid-here";
const char* password = "password-here";
const char* mqtt_server = "be9.asuscomm.com";
const char* temp_topic = "b9/temperature";
const char* humid_topic = "b9/humidity";
const char* pres_topic = "b9/pressure";
const char* alt_topic = "b9/altitude";
const char* sub_topic = "b9/esp32/led";
const char* on_value = "on";
const char* off_value = "off";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

//uncomment the following lines if you're using SPI
/*#include 
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI
float temperature = 0;
float humidity = 0;

// LED Pin
//const int LED_BUILTIN = 4;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  //status = bme.begin();  
  if (!bme.begin(0x77)) { //0x77 är 4-värdes BME, 0x76 är 2-värdes
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT
  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == sub_topic) {
    Serial.print("Changing output to ");
    if(messageTemp == on_value){
      Serial.println(on_value);
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else if(messageTemp == off_value){
      Serial.println(off_value);
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe(sub_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
    // Temperature in Celsius
    temperature = bme.readTemperature();   
    // Uncomment the next line to set temperature in Fahrenheit 
    // (and comment the previous temperature line)
    // temperature = 1.8 * bme.readTemperature() + 32; // Temperature in Fahrenheit
    
    // Convert temp value to a char array
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    Serial.print("Temperature: ");
    Serial.print(tempString);
    Serial.println(" °C");
    client.publish(temp_topic, tempString);

    
    humidity = bme.readHumidity();
    
    // Convert humid value to a char array
    char humString[8];
    dtostrf(humidity, 1, 2, humString);
    Serial.print("Humidity: ");
    Serial.print(humString);
    Serial.println(" %Rh");
    client.publish(humid_topic, humString);


    float pressure = bme.readPressure() / 100.0F;

    char presString[8];
    dtostrf(pressure, 1, 2, presString);
    Serial.print("Pressure: ");
    Serial.print(presString);
    Serial.println(" hPa");
    client.publish(pres_topic, presString);


    float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

    char altString[8];
    dtostrf(altitude, 1, 2, altString);
    Serial.print("Approx. Altitude: ");
    Serial.print(altString);
    Serial.println(" m");
    client.publish(alt_topic, altString);

    Serial.println("--------------"); 
  }
}
