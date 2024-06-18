#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHTesp.h"
#include <DHT.h>  
#include <HardwareSerial.h>



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define ncom 3 // number of commands
char commar[ncom] = {0x1, 0x3, 0x5}; // Actual commands
char respar[ncom][30] = {"Phosphorous value is: ", "Potassium value is: ", "Nitrogen value is: "};
uint8_t rtValue[ncom]; // Store the return values from the custom chip in here

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

void setup() {
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);  
  Serial.begin(115200);

  Serial2.begin(15200, SERIAL_8N1, 16, 17); // Initialize the custom chip communication line

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Initialize with the I2C address 0x3C

  Serial.println("Hello, ESP32!");

  display.clearDisplay(); // Clear the buffer
  display.display();      // Display initial buffer

  // Display title for 5 seconds
  displayTitle();
  delay(5000);
}

void displayTitle() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Smart Soil Monitoring");
  display.println("System Using IoT");
  display.display();
}

void loop() {
  readNPKSensor();
  displayNPKValues(rtValue[0], rtValue[1], rtValue[2]);
  delay(2000); // Update every 2 seconds
}

void readNPKSensor() {
  for (uint8_t i = 0; i < ncom; i++) {
    Serial2.print((char)commar[i]); // Send the command stored in ncom array through serial2
    if (Serial2.available()) {      // If serial2 data is there
      rtValue[i] = Serial2.read();  // Read serial2
      Serial2.flush();              // Flush serial2, very important. Otherwise, extra bits may interfere with communication
      Serial.print(respar[i]);      // Print the response array to the console
      Serial.println(rtValue[i]);   // Print the return value with newline at console
    }
  }
}

void displayNPKValues(uint8_t nitrogen, uint8_t phosphorous, uint8_t potassium) {
  display.ssd1306_command(SSD1306_DISPLAYON); // Turn on the OLED display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("N: ");
  display.println(nitrogen);
  display.print("P: ");
  display.println(phosphorous);
  display.print("K: ");
  display.println(potassium);
  display.display();
  int16_t i = analogRead(34);
  String msg = i < 2165 ? "DRY" : i > 3135 ? "WET" : "OK";
}





