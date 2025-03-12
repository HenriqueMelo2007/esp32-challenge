#include <Arduino.h> // Inclusão das Bibliotecas e Definição dos Macros
#include "NimBLEDevice.h"
#include <FastLED.h>

#define NUM_LEDS 1
#define DATA_PIN 38

#define SCANNING_TIME 5
#define MAC_ADDRESS_ONE "ff:ff:11:1f:7d:a2"
#define MAC_ADDRESS_TWO "ff:ff:11:1f:26:53"

CRGB leds[NUM_LEDS];

volatile bool tagFound = false; // Declaração da Variável Global tagFound

bool scanning(); // Protótipos das Funções Utilizadas
void scanningTask(void *parameter);
void blinkRedLED(int onTime, int offTime);

void setup() // Configuração Inicial no setup()
{
  Serial.begin(115200);
  NimBLEDevice::init("");
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  xTaskCreate(scanningTask, "ScanningTask", 4096, NULL, 1, NULL); // Criação da tarefa do FreeRTOS
}

void loop() // Loop Principal do Programa
{

  if (tagFound)
  {
    leds[0] = CRGB::Green;
    FastLED.show();
  }
  else
  {
    blinkRedLED(500, 500);
  }
}

bool scanning() // Função scanning() – Escaneamento de Dispositivos BLE
{
  NimBLEScan *scan = NimBLEDevice::getScan();
  NimBLEScanResults scanResult = scan->getResults(SCANNING_TIME * 1000); // Obtém o conjunto de resultados do scan

  for (int i = 0; i < scanResult.getCount(); i++) // Itera sobre cada item do conjunto de resultados
  {
    const NimBLEAdvertisedDevice *specificDevice = scanResult.getDevice(i); // Obtém item específico na ordem do loop
    NimBLEAddress specificMacAddress = specificDevice->getAddress();        // Obtém endereço MAC para comparação posterior
    const char *addressString = specificMacAddress.toString().c_str();      // Endereço em string para comparação

    if (strcmp(addressString, MAC_ADDRESS_ONE) == 0 || strcmp(addressString, MAC_ADDRESS_TWO) == 0) // Uso de strcmp() para comparar MAC do iterador com macro do MAC das tags
    {
      Serial.println("*** TAG encontrada ***");
      return true;
    }
  }
  Serial.println("*** TAG não encontrada ***");
  return false;
}

void scanningTask(void *parameter) // Função scanningTask() – Tarefa do FreeRTOS para Escaneamento Contínuo
{
  while (true) // Scan infinito
  {
    tagFound = scanning();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

inline void blinkRedLED(int onTime, int offTime) // Função blinkRedLED() – Controle Não Bloqueante do LED Vermelho
{
  static unsigned long previousMillis = 0; // Variáveis usadas para calcular se o LED deve ou não mudar de estado ( on - off )
  static int ledState = LOW;
  unsigned long currentMillis = millis();

  if (previousMillis == 0) // Primeiro acionamento
  {
    leds[0] = CRGB::Red;
    FastLED.show();
    ledState = HIGH;
    previousMillis = currentMillis;
    return;
  }

  unsigned long elapsed = currentMillis - previousMillis; // Variável que armazena a diferença de tempo entre o momento atual e a última mudança de estado do LED

  if (ledState == HIGH && elapsed >= onTime) // Estrutura condicional para definir o estado do LED vermelho
  {
    FastLED.clear();
    FastLED.show();
    ledState = LOW;
    previousMillis = currentMillis;
  }
  else if (ledState == LOW && elapsed >= offTime)
  {
    leds[0] = CRGB::Red;
    FastLED.show();
    ledState = HIGH;
    previousMillis = currentMillis;
  }
}