![GitHub contributors](https://img.shields.io/github/contributors/HenriqueMelo2007/esp32-challenge)
![Commits](https://img.shields.io/github/commit-activity/y/HenriqueMelo2007/esp32-challenge)
![Last commit](https://img.shields.io/github/last-commit/HenriqueMelo2007/esp32-challenge)
![Repository size](https://img.shields.io/github/repo-size/HenriqueMelo2007/esp32-challenge)


# Desafio de Desenvolvimento de Firmware – ESP32

![ESP32 microcontroller](https://www.espressif.com/sites/default/files/banner/esp32-modules-banner1-new.jpg)

## Sumário
- [Visão geral](#visão-geral)
- [Hardware](#hardware)
- [Software](#software)
- [Processo de desenvolvimento](#processo-de-desenvolvimento)
- [Como utilizar no seu ESP32 (clonagem de repositório)](#como-utilizar-no-seu-esp32-clonagem-de-repositório)

## Visão geral

O desafio proposto envolveu o desenvolvimento de um firmware para o módulo ESP32-S3-WROOM-1, com a missão de atuar como um monitor de tags BLE específicas. O sistema foi idealizado para detectar continuamente os dispositivos BLE desejados próximos e, com base na presença ou ausência das tags, acionar indicadores luminosos (LEDs) de forma dinâmica.

A solução implementada combina diversas tecnologias. Foi utilizada a IDE PlatformIO, integrada ao VScode, com o framework Arduino como base de desenvolvimento. A aplicação utiliza a biblioteca NimBLE-Arduino para gerenciar as conexões BLE e o FreeRTOS para a execução de tarefas em paralelo, garantindo um escaneamento contínuo dos dispositivos e alta responsividade dos LEDs ao resultado do escaneamento.

## Hardware

- **ESP32-S3-WROOM-1**

> Função: Atua como o microcontrolador central, responsável por executar o firmware, gerenciar a comunicação BLE, processar os dados recebidos e controlar os LEDs conforme a lógica implementada.

- **Cabo USB**

> Função: Permite a comunicação entre o computador e o ESP32, facilitando a programação, depuração e fornecimento de energia ao dispositivo durante o desenvolvimento.

- **Tags BLE**

> Função: Servem como os dispositivos de referência monitorados pelo firmware. A detecção das tags é o gatilho para a mudança no comportamento dos indicadores luminosos (LEDs).

- **Protoboard**

> Função: Fornece uma plataforma de prototipagem para a montagem e testes do circuito, permitindo conexões rápidas e seguras entre os componentes sem a necessidade de solda.

- **LEDs (verde e vermelho)**

> Função: Funcionam como indicadores visuais do estado do sistema. O LED verde acende continuamente quando uma tag é detectada, enquanto o LED vermelho pisca para indicar a ausência das tags monitoradas.

- **Resistores de 220 ohms**

> Função: Limitam a corrente que passa pelos LEDs, protegendo-os contra sobrecargas e garantindo um funcionamento seguro e prolongado dos indicadores luminosos.

- **Jumpers macho-fêmea**

> Função: Realizam as conexões necessárias entre o ESP32 e os demais componentes na protoboard. Um jumper é utilizado para conectar o GND do ESP32 à protoboard, enquanto os outros dois conectam os pinos GPIO 13 (LED vermelho) e GPIO 15 (LED verde), permitindo o controle individual dos LEDs.

## Software


## Processo de desenvolvimento


## Como utilizar no seu ESP32 (clonagem de repositório)