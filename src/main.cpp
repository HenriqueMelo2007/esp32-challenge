#include <Arduino.h>
#include "NimBLEDevice.h"

#define RED_LED 13
#define GREEN_LED 15

#define SCANNING_TIME 2
#define MAC_ADDRESS_ONE "ff:ff:11:1f:7d:a2"
#define MAC_ADDRESS_TWO "ff:ff:11:1f:26:53"

volatile bool tagFound = false;

bool scanning();
void scanningTask(void *parameter);
void blinkRedLED(int onTime, int offTime);


void setup()
{
  Serial.begin(115200);
  NimBLEDevice::init("");
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  xTaskCreate(scanningTask, "ScanningTask", 4096, NULL, 1, NULL);
}

void loop()
{
  if (tagFound)
  {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  }
  else
  {
    digitalWrite(GREEN_LED, LOW);
    blinkRedLED(500, 500);
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
    const char *addressString = specificMacAddress.toString().c_str();

    if (strcmp(addressString, MAC_ADDRESS_ONE) == 0 || strcmp(addressString, MAC_ADDRESS_TWO) == 0)
    {
      Serial.println("*** TAG encontrada ***");
      return true;
    }
  }
  Serial.println("*** TAG não encontrada ***");
  return false;
}

void scanningTask(void *parameter)
{
  while (true)
  {
    tagFound = scanning();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

inline void blinkRedLED(int onTime, int offTime)
{
  static unsigned long previousMillis = 0;
  static int ledState = LOW;
  unsigned long currentMillis = millis();

  if (previousMillis == 0)
  {
    digitalWrite(RED_LED, HIGH);
    ledState = HIGH;
    previousMillis = currentMillis;
    return;
  }

  unsigned long elapsed = currentMillis - previousMillis;

  if (ledState == HIGH && elapsed >= onTime)
  {
    digitalWrite(RED_LED, LOW);
    ledState = LOW;
    previousMillis = currentMillis;
  }
  else if (ledState == LOW && elapsed >= offTime)
  {
    digitalWrite(RED_LED, HIGH);
    ledState = HIGH;
    previousMillis = currentMillis;
  }
}