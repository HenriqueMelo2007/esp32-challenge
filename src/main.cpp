#include <Arduino.h>
#include "NimBLEDevice.h"

#define MAC_ADDRESS "ff:ff:11:1f:7d:a2"

bool deviceIsPresent = false;

void setup()
{
  Serial.begin(115200);
  Serial.printf("Iniciando setup...\n");

  NimBLEDevice::init("");
  NimBLEScan* scan = NimBLEDevice::getScan();
  NimBLEScanResults scanResult = scan->getResults(5 * 1000);
  
  for (int i = 0; i < scanResult.getCount(); i++) {
    const NimBLEAdvertisedDevice* specificDevice = scanResult.getDevice(i);
    NimBLEAddress specificMacAddress = specificDevice->getAddress();
    const char* addresString = specificMacAddress.toString().c_str();

    int comparing = strcmp(addresString, MAC_ADDRESS);

    if (comparing == 0) {
      Serial.printf("*** TAG encontrada ***\n");
      Serial.printf("Endereço MAC: %s\n", addresString);
      Serial.printf("***************\n");

    } else {
      Serial.printf("Dispositivo qualquer encontrado. Endereço MAC: %s\n", addresString);
    }
  }

  printf("Setup finalizado.\n");
}

void loop()
{
  Serial.printf("Loop\n");
  delay(15000);
}