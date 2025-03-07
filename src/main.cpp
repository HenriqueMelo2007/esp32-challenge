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

    printf("Novo dispositivo encontrado! Endereço MAC: %s\n", specificMacAddress.toString().c_str());
  }

  printf("Finalizando setup...\n");
}

void loop()
{
  Serial.printf("Loop\n");
  delay(5000);
}