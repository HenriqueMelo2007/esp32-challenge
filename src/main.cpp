#include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println("Primeira mensagem");
}

void loop()
{
  Serial.println("Conexão estabelecida 2");
  delay(2000);
}