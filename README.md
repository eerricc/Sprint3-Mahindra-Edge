# **2sem - Sprint 3 Edge**

> O objetivo desta tarefa é desenvolver a arquitetura inicial de uma aplicação loT capaz de receber e transmitir dados provenientes de dispositivos conectados

## 📋 Pré-requisitos
+ Ter o arduino IDE instalado em sua máquina, para rodar os códigos da placa.
+ materiais :
  - ESP32
  - Cabos Jumper
  - Sensor LDR
  - Sensor DHT 22 ou 11
  - Postman
  - Virtual Machine

## 📡 🔍 Link do Projeto
Segue codigo do projeto : 

## 🛠️ Construído com
+ Wokwi IDE
+ Virtual Box
+ Postman
+ Arduino IDE

## Linguagens Utilizadas
![Static Badge](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&labelColor=black)

![Static Badge](https://img.shields.io/badge/C-A8B9CC?style=for-the-badge&logo=c&labelColor=black)

## Diagrama
![fiwareDeploySprint3 drawio](https://github.com/user-attachments/assets/582891f5-ea2b-42f0-b70f-7695ecd69e8a)
>O diagrama representa uma arquitetura de sistema IoT onde os dispositivos coletam dados e os enviam para um backend robusto, que processa, armazena e disponibiliza esses dados por meio de diferentes interfaces, como um dashboard e um aplicativo móvel. Essa estrutura permite a visualização e o gerenciamento eficiente das informações coletadas.

## 📖 Mini Guide
> Este guia não demonstra o passo a passo completo; o objetivo é apenas mostrar para o usuário a estrutura GERAL do projeto.

### 1) Criar uma Virtual Machine
- **Instalar**:
  - Docker
  - Docker Compose
  - FIWARE
  - Necessita de um código Python para dashboards

- **Abrir as portas do firewall**:
  - `1026/TCP` - Orion Context Broker
  - `1883/TCP` - Eclipse-Mosquito MQTT
  - `4041/TCP` - IoT-Agent MQTT
  - `8666/TCP` - STH-Comet

### 2) Postman
- **Instalar**:
  - Necessita de uma collection JSON
  - Adaptar as variáveis de acordo com as informações da sua máquina virtual

### 3) Simulação Wokwi ou Hands-On
- Utilize um código base, adaptando as variáveis de acordo com sua preferência e necessidade.

### 4) Guia Completo: https://github.com/fabiocabrini/fiware

## ✒️ Autores
+ Eric Segawa Montagner RM558224
+ João Victor Oliveira dos Santos RM557948
+ Gabriel Lacerda Araujo RM558307
+ Gabriel Luni Nakashima RM558096
+ Melyssa Huang Gonçalves RM 558401
