/*
 ESP32 I2C Scanner
 inspired by the following:
 Nick Gammon http://www.gammon.com.au/forum/?id=10896&reply=6#reply6
 Rajendra Jain https://github.com/jainrk/i2c_port_address_scanner
*/

#include <Arduino.h>
#include <Wire.h>

// int pins[] = {33, 35};  // pins to test
int pins[] = {SDA, SCL};  // test default pins

void scanAddresses() {
  for (int address = 0x1; address <= 0x7F; address++) {
    Wire.beginTransmission(address);
    switch (Wire.endTransmission()) {
      case 0:
        Serial.print("I2C device found at address 0x");
        if (address < 0x10) Serial.print("0");
        Serial.print(address, HEX);
        Serial.println(" !");
        break;
      case 4:
        Serial.println("I2C in slave mode!");
        return;
      default:
        continue;
    }
  }
}

void scanPorts() {
  for (int i = 0; i < sizeof(pins) / sizeof(int); i++) {
    for (int j = 0; j < sizeof(pins) / sizeof(int); j++) {
      if (i == j) continue;
      Serial.print("> (SDA : SCL) - (" + String(pins[i]) + " : " + String(pins[j]) + ")");
      if (pins[i] == SDA) Serial.print(" - DEFAULT SDA");
      if (pins[j] == SCL) Serial.print(" - DEFAULT SCL");
      Serial.println(" <");
      Wire.begin(pins[i], pins[j]);
      scanAddresses();
      Wire.end();
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
#ifdef LOLIN_S2_PICO
  digitalWrite(LED_BUILTIN, LOW);
#else
  digitalWrite(LED_BUILTIN, HIGH);
#endif
  Serial.begin(115200);
  while (!Serial)
    ;
#ifdef LOLIN_S2_PICO
  digitalWrite(LED_BUILTIN, HIGH);
#else
  digitalWrite(LED_BUILTIN, LOW);
#endif
  delay(1000);
  Serial.println("Scanning...");
  scanPorts();
  Serial.println("done.");
}

void loop() {
}
