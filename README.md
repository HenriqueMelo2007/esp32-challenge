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


## Software


## Processo de desenvolvimento


## Como utilizar no seu ESP32 (clonagem de repositório)