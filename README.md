![GitHub contributors](https://img.shields.io/github/contributors/HenriqueMelo2007/esp32-challenge)
![Commits](https://img.shields.io/github/commit-activity/y/HenriqueMelo2007/esp32-challenge)
![Repository size](https://img.shields.io/github/repo-size/HenriqueMelo2007/esp32-challenge)

# Desafio de Desenvolvimento de Firmware – ESP32

![ESP32 microcontroller](https://www.espressif.com/sites/default/files/banner/esp32-modules-banner1-new.jpg)

## Sumário

- [Visão geral](#visão-geral)
- [Hardware](#hardware)
- [Software](#software)
- [Processo de desenvolvimento](#processo-de-desenvolvimento)
- [Referências](#referências)

## Visão geral

O desafio proposto envolveu o desenvolvimento de um firmware para o módulo ESP32-S3-WROOM-1, com a missão de atuar como um monitor de tags BLE específicas. O sistema foi idealizado para detectar continuamente os dispositivos BLE desejados próximos e, com base na presença ou ausência das tags, acionar indicadores luminosos (LEDs) de forma dinâmica.

A solução implementada combina diversas tecnologias. Foi utilizada a IDE PlatformIO, integrada ao VScode, com o framework Arduino como base de desenvolvimento. A aplicação utiliza a biblioteca NimBLE-Arduino para gerenciar as conexões BLE, a biblioteca FastLED para controle do LED RGB endereçável da placa e o FreeRTOS para a execução de tarefas em paralelo, garantindo um escaneamento contínuo dos dispositivos e alta responsividade dos LEDs ao resultado do escaneamento.

## Hardware

- **ESP32-S3-WROOM-1**

> Função: Atua como o microcontrolador central, responsável por executar o firmware, gerenciar a comunicação BLE, processar os dados recebidos e controlar o LED RGB conforme a lógica implementada.

- **Cabo USB**

> Função: Permite a comunicação entre o computador e o ESP32, facilitando a programação, depuração e fornecimento de energia ao dispositivo durante o desenvolvimento.

- **Tags BLE**

> Função: Servem como os dispositivos de referência monitorados pelo firmware. A detecção das tags é o gatilho para a mudança no comportamento dos indicadores luminosos (LEDs).

## Software

### **Nesta seção, detalharemos cada trecho do código desenvolvido para atender ao desafio. Cada trecho de código é apresentado em blocos, seguido de uma análise sobre seu funcionamento.**

> Intervalos de linhas descartadas entre os trechos explicados correspondem a espaços em brancos

### Trecho 1: Inclusão das Bibliotecas e Definição dos Macros (linhas 1 a 11)

```cpp
#include <Arduino.h>
#include "NimBLEDevice.h"
#include <FastLED.h>

#define NUM_LEDS 1
#define DATA_PIN 38

#define SCANNING_TIME 5
#define MAC_ADDRESS_ONE "ff:ff:11:1f:7d:a2"
#define MAC_ADDRESS_TWO "ff:ff:11:1f:26:53"

CRGB leds[NUM_LEDS];
```

- Arduino.h: Fornece as definições básicas, funções e estruturas do framework Arduino.
- NimBLEDevice.h: Biblioteca específica para manipulação de conexões BLE.
- FastLED.h: Biblioteca para controle de LEDs endereçáveis.

Em seguida, são definidos macros que padronizam constantes utilizadas em todo o código:

- NUM_LEDS e DATA_PIN: Configuram a quantidade de LEDs e o pino de dados para a fita NeoPixel.
- SCANNING_TIME: Define o tempo de escaneamento (em segundos) para a busca de dispositivos BLE.
- MAC_ADDRESS_ONE e MAC_ADDRESS_TWO: Armazenam os endereços MAC das tags BLE que o firmware deverá monitorar.
- leds[]: Array para armazenar o estado dos LEDs NeoPixel.

### Trecho 2: Declaração da Variável Global tagFound (linha 13)

```cpp
volatile bool tagFound = false;
```

- tagFound serve para armazenar o resultado do escaneamento BLE, indicando se alguma das tags monitoradas foi detectada.

### Trecho 3: Protótipos das Funções Utilizadas (linhas 15 a 17)

```cpp
bool scanning();
void scanningTask(void *parameter);
void blinkRedLED(int onTime, int offTime);`
```

Aqui, são declarados os protótipos das funções que compõem a aplicação, garantindo que o compilador conheça suas assinaturas antes de serem utilizadas:

- scanning(): Responsável por realizar o escaneamento dos dispositivos BLE próximos e identificar se alguma tag monitorada está presente.
- scanningTask(void \*parameter): Função que será executada como uma tarefa do FreeRTOS, permitindo o escaneamento contínuo em paralelo com o loop principal.
- blinkRedLED(int onTime, int offTime): Gerencia o comportamento de piscar o LED vermelho de forma não bloqueante utilizando a biblioteca FastLED.

### Trecho 4: Configuração Inicial no setup() (linhas 19 a 25)

```cpp
void setup()
{
Serial.begin(115200);
NimBLEDevice::init("");
FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

xTaskCreate(scanningTask, "ScanningTask", 4096, NULL, 1, NULL);
}
```

Esta função é executada uma única vez na inicialização do firmware e realiza as seguintes operações:

- Inicialização da Comunicação Serial:

  > Serial.begin(115200); configura a taxa de transmissão para 115200 bauds, permitindo a depuração e monitoramento via terminal serial.

- Inicialização do BLE:

  > NimBLEDevice::init(""); inicializa a biblioteca NimBLE, preparando o dispositivo para operações de escaneamento BLE.

- Configuração dos LEDs NeoPixel:

  > FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); define o tipo e pino de controle dos LEDs endereçáveis.

- Criação da Tarefa FreeRTOS:
  > xTaskCreate(scanningTask, "ScanningTask", 4096, NULL, 1, NULL); cria uma tarefa separada para o escaneamento BLE. Utilizar o FreeRTOS permite que o escaneamento seja executado de forma paralela ao loop principal, melhorando a responsividade do sistema.

### Trecho 5: Loop Principal do Programa (linhas 27 a 36)

```cpp
void loop()
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
```

- O loop principal monitora constantemente o estado da variável tagFound e atualiza os LEDs NeoPixel de acordo:
- Se tagFound for true: Atribui cor verde ao LED e atualiza o hardware.
- Se tagFound for false: Aciona o padrão de piscar vermelho através da função blinkRedLED().

### Trecho 6: Função scanning() – Escaneamento de Dispositivos BLE (linhas 38 a 57)

```cpp
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
```

Esta função é o coração do monitoramento BLE:

- Obtenção do Objeto de Escaneamento:
  NimBLEDevice::getScan(); recupera a instância do objeto de escaneamento da biblioteca NimBLE.

- Realização do Escaneamento:
  scan->getResults(SCANNING_TIME \* 1000); executa o escaneamento por um período definido (convertendo os segundos para milissegundos), retornando uma lista de dispositivos detectados.

- Iteração Sobre os Resultados:
  Um laço for percorre todos os dispositivos encontrados. Para cada dispositivo:
  Obtém-se o endereço MAC utilizando getAddress(). Converte o endereço para uma string, facilitando a comparação com os endereços das tags específicas.

- Verificação e Retorno:
  A função utiliza strcmp() para comparar o endereço do dispositivo com os dois endereços definidos nos macros. Se houver correspondência, o firmware imprime uma mensagem de sucesso e retorna true, indicando que uma tag foi encontrada. Caso contrário, após a iteração, imprime que nenhuma tag foi detectada e retorna false.

### Trecho 7: Função scanningTask() – Tarefa do FreeRTOS para Escaneamento Contínuo (linhas 59 a 66)

```cpp
void scanningTask(void *parameter)
{
while (true)
{
tagFound = scanning();
vTaskDelay(10 / portTICK_PERIOD_MS);
}
}
```

Esta função é projetada para ser executada em uma tarefa separada do FreeRTOS, garantindo que o processo de escaneamento BLE ocorra continuamente e de forma assíncrona:

- Loop Infinito:
  A estrutura while (true) assegura que o escaneamento seja realizado repetidamente.

- Atualização da Variável Global:
  A cada iteração, o resultado do escaneamento (true ou false) é atribuído à variável tagFound, atualizando o estado para que o loop principal possa reagir adequadamente.

- Atraso Controlado:
  vTaskDelay(10 / portTICK_PERIOD_MS); introduz uma breve pausa de 10 milissegundos entre os escaneamentos, evitando sobrecarga do processador.

### Trecho 8: Função blinkRedLED() – Controle Não Bloqueante do LED Vermelho (linhas 68 a 96)

```cpp
inline void blinkRedLED(int onTime, int offTime)
{
static unsigned long previousMillis = 0;
static int ledState = LOW;
unsigned long currentMillis = millis();

if (previousMillis == 0)
{
leds[0] = CRGB::Red;
FastLED.show();
ledState = HIGH;
previousMillis = currentMillis;
return;
}
unsigned long elapsed = currentMillis - previousMillis;

if (ledState == HIGH && elapsed >= onTime)
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
```

Esta função controla o piscar do LED vermelho sem bloquear o loop principal:

- Declaração de Variáveis Estáticas:
  previousMillis e ledState mantêm seus valores entre chamadas para rastrear o tempo e estado do LED.

- Uso da Função millis():
  millis() retorna o tempo em milissegundos desde o início do programa, permitindo temporizações não bloqueantes.

- Controle do NeoPixel:
  Utiliza FastLED.show() para atualizar o hardware e FastLED.clear() para apagar o LED, substituindo as operações tradicionais de GPIO.

- Lógica de Alternância:
  Alterna entre estados de cor vermelha e desligado baseado nos tempos de ativação (onTime) e desativação (offTime) fornecidos.

## Processo de desenvolvimento

Familiarização com as tags BLE. Meu primeiro passo foi entender como utilizá-las e descobrir quais informações eram necessárias para a interação proposta entre o microcontrolador e os dispositivos. Segui a instrução da embalagem das TAGs e fiz o download do aplicativo iSearching no meu celular para ter as primeiras impressões sobre o uso das TAGs. Após um breve teste utilizando o aplicativo recomendado, busquei meios de obter os endereços MAC das TAGs de forma eficiente. Instalei, então, no meu celular o aplicativo nRF Connect para obtenção dos endereços através de um scan rápido. Após isso, escolhi qual biblioteca utilizar para o gerenciamento das funcionalidades Bluetooth: a biblioteca NimBLE-Arduino.

Com os endereços "em mãos" e as ferramentas para escrita do código definidas, busquei estruturar em meu caderno as informações mais importantes do projeto e quais tarefas o código deveria ser capaz de executar. Dessa forma, pude desenvolver o código de maneira clara e sequencial. Durante o versionamento do código no GitHub, através do Git, foram utilizados commits padronizados conhecidos como "Conventional Commits" para esclarecer as mudanças implementadas. A maior parte do tempo dedicado ao desenvolvimento do desafio foi empregada na leitura da documentação da biblioteca NimBLE-Arduino, para que fosse possível utilizá-la adequadamente.

### Uso da inteligência artificial generativa

- Revisão e refatoração de código

**Durante a codificação consultei os agentes de IA para revisão do meu código. Verificação de erros e possíveis melhorias.**

- Criação deste README

**As seções [Hardware](#hardware) e [Software](#software) deste documento foram feitas com auxílio dos agentes de IA.**

- Pesquisas curtas

**Dúvidas simples e diretas cujo uma pesquisa no Google ou na própria documentação levariam mais tempo foram feitas através dos agentes de IA.**

## Referências

### Inteligências artificiais generativas

- **ChatGPT( [https://chatgpt.com/](https://chatgpt.com/) )**
- **DeepSeek( [https://chat.deepseek.com/](https://chat.deepseek.com/) )**

### Documentações

- **Arduino Framework ( [https://docs.arduino.cc/programming/](https://docs.arduino.cc/programming/) )**
- **NimBLE-Arduino ( [https://github.com/h2zero/NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino) )**
- **C Programming Language( [https://devdocs.io/c/](https://devdocs.io/c/) )**
- **FastLED ( [https://github.com/FastLED/FastLED](https://github.com/FastLED/FastLED) )**

### Outros

- **ISearching( [https://play.google.com/store/apps/details?id=com.findn.isearching&hl=pt-BR&pli=1](https://play.google.com/store/apps/details?id=com.findn.isearching&hl=pt-BR&pli=1) )**
- **NRF Connect( [https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=pt-br](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=pt-br) )**
- **Artigo sobre como utilizar Conventional Commits Pattern( [https://medium.com/linkapi-solutions/conventional-commits-pattern-3778d1a1e657](https://medium.com/linkapi-solutions/conventional-commits-pattern-3778d1a1e657) )**
