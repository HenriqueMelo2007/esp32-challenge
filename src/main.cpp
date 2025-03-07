#include <Arduino.h>
#include "NimBLEDevice.h"
#include "NimBLEAddress.h"

void setup()
{
  Serial.begin(115200);
  Serial.printf("Iniciando setup...\n");

  NimBLEDevice::init("");

  Serial.printf("Setup finalizado!\n");
}

void loop()
{
  Serial.printf("Loop\n");
  delay(5000);
}