#include <Arduino.h>
#include "NimBLEDevice.h"

#define RED_LED 13
#define GREEN_LED 15

#define SCANNING_TIME 5
#define MAC_ADDRESS_ONE "ff:ff:11:1f:7d:a2"
#define MAC_ADDRESS_TWO "ff:ff:11:1f:26:53"

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
    digitalWrite(GREEN_LED, HIGH);
  } else {
    digitalWrite(GREEN_LED, LOW);
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

    int comparingOne = strcmp(addresString, MAC_ADDRESS_ONE);
    int comparingTwo = strcmp(addresString, MAC_ADDRESS_TWO);

    if (comparingOne == 0 || comparingTwo == 0)
    {
      Serial.println("*** TAG encontrada ***");

      return true;
    }
  }

  Serial.println("*** TAG não encontrada ***");

  return false;
}