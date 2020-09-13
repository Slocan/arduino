// Init libraries for Oled display ssd1306, 128x32
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Potentiometer is connected to Pin 34
const int potPin = 34;

// variable for storing the potentiometer value
int potValue = 0;
const int AirValue = 2800;
const int WaterValue = 1280;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.display();
  delay(1000);
}

void loop() {
  // Reading sensor value
  potValue = analogRead(potPin);
  int moisturePercentage = 100-(potValue-WaterValue)*100/(AirValue-WaterValue);
  Serial.println(moisturePercentage);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print(moisturePercentage);
  display.println(" % humidity");
  display.display();
  //Serial.println(potValue);
  delay(1000);
}
