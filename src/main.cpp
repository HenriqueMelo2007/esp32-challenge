#include <Arduino.h>
#include "NimBLEDevice.h"

#define RED_LED 13
#define GREEN_LED 15

#define SCANNING_TIME 5
#define MAC_ADDRESS "ff:ff:11:1f:7d:a2"

bool scanning();

void setup()
{
  Serial.begin(115200);
  NimBLEDevice::init("");
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
}

void loop()
{
  if (scanning())
  {
    Serial.print("");
  }
}

bool scanning()
{
  NimBLEScan *scan = NimBLEDevice::getScan();
  NimBLEScanResults scanResult = scan->getResults(SCANNING_TIME * 1000);

  for (int i = 0; i < scanResult.getCount(); i++)
  {
    const NimBLEAdvertisedDevice *specificDevice = scanResult.getDevice(i);

    NimBLEAddress specificMacAddress = specificDevice->getAddress();
    const char *addresString = specificMacAddress.toString().c_str();

    int comparing = strcmp(addresString, MAC_ADDRESS);

    if (comparing == 0)
    {
      Serial.println("*** TAG encontrada ***");

      return true;
    }
  }

  return false;
}