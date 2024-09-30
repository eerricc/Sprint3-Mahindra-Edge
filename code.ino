//Autor: Fábio Henrique Cabrini
//Resumo: Esse programa possibilita ligar e desligar o led onboard, além de mandar o status para o Broker MQTT possibilitando o Helix saber
//se o led está ligado ou desligado.
//Revisões:
//Rev1: 26-08-2023 Código portado para o ESP32 e para realizar a leitura de luminosidade e publicar o valor em um tópico aprorpiado do broker 
//Autor Rev1: Lucas Demetrius Augusto 
//Rev2: 28-08-2023 Ajustes para o funcionamento no FIWARE Descomplicado
//Autor Rev2: Fábio Henrique Cabrini
//Rev3: 1-11-2023 Refinamento do código e ajustes para o funcionamento no FIWARE Descomplicado
//Autor Rev3: Fábio Henrique Cabrini
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Configurações do DHT
#define DHTPIN 4          // Pino onde o sensor DHT está conectado
#define DHTTYPE DHT22     // Tipo do sensor DHT
DHT dht(DHTPIN, DHTTYPE); // Instancia o objeto DHT

// Configurações - variáveis editáveis
const char* default_SSID = "Wokwi-GUEST"; // Nome da rede Wi-Fi
const char* default_PASSWORD = ""; // Senha da rede Wi-Fi
const char* default_BROKER_MQTT = "104.41.55.114"; // IP do Broker MQTT
const int default_BROKER_PORT = 1883; // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/fimt003/cmd"; // Tópico MQTT para escuta
const char* default_TOPICO_PUBLISH_1 = "/TEF/fimt003/attrs"; // Tópico para envio de informações
const char* default_TOPICO_PUBLISH_2 = "/TEF/fimt003/attrs/l"; // Tópico para luminosidade
const char* default_TOPICO_PUBLISH_3 = "/TEF/fimt003/attrs/h"; // Tópico para umidade
const char* default_TOPICO_PUBLISH_4 = "/TEF/fimt003/attrs/t"; // Tópico para temperatura
const char* default_ID_MQTT = "fiware_003"; // ID do cliente MQTT
const int default_D4 = 2; // Pino do LED onboard

WiFiClient espClient; // Cliente Wi-Fi
PubSubClient MQTT(espClient); // Cliente MQTT
char EstadoSaida = '0'; // Estado do LED (0 = desligado, 1 = ligado)

void initSerial() {
    Serial.begin(115200); // Inicializa a comunicação serial
}

void initWiFi() {
    delay(10);
    Serial.println("------Conexão WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(default_SSID);
    Serial.println("Aguarde");
    reconectWiFi(); // Chama a função para conectar ao Wi-Fi
}

void initMQTT() {
    MQTT.setServer(default_BROKER_MQTT, default_BROKER_PORT); // Configura o broker MQTT
    MQTT.setCallback(mqtt_callback); // Define a função de callback para mensagens recebidas
}

void InitOutput() {
    pinMode(default_D4, OUTPUT); // Configura o pino do LED como saída
    digitalWrite(default_D4, HIGH); // Garante que o LED inicie desligado
}

void setup() {
    InitOutput(); // Inicializa a saída do LED
    initSerial(); // Inicializa a comunicação serial
    initWiFi(); // Conecta ao Wi-Fi
    initMQTT(); // Inicializa a conexão MQTT
    dht.begin(); // Inicializa o sensor DHT
    delay(5000);
    MQTT.publish(default_TOPICO_PUBLISH_1, "s|on"); // Publica estado inicial no tópico
}

void loop() {
    VerificaConexoesWiFIEMQTT(); // Verifica as conexões Wi-Fi e MQTT
    EnviaEstadoOutputMQTT(); // Envia o estado do LED via MQTT
    handleLuminosity(); // Lê e publica o valor de luminosidade
    handleDHT(); // Lê e publica temperatura e umidade
    MQTT.loop(); // Mantém a conexão MQTT ativa
}

void reconectWiFi() {
    if (WiFi.status() == WL_CONNECTED) // Verifica se já está conectado
        return;
    WiFi.begin(default_SSID, default_PASSWORD); // Conecta à rede Wi-Fi
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print("."); // Indicador de conexão
    }
    Serial.println();
    Serial.println("Conectado com sucesso na rede ");
    Serial.print(default_SSID);
    Serial.println(" IP obtido: ");
    Serial.println(WiFi.localIP()); // Exibe o IP obtido
    digitalWrite(default_D4, LOW); // Liga o LED para indicar conexão
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (int i = 0; i < length; i++) {
        char c = (char)payload[i];
        msg += c; // Constrói a mensagem recebida
    }
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);

    // Compara a mensagem recebida para controle do LED
    String onTopic = String("fimt003@on|");
    String offTopic = String("fimt003@off|");

    if (msg.equals(onTopic)) {
        digitalWrite(default_D4, HIGH); // Liga o LED
        EstadoSaida = '1'; // Atualiza estado
    }

    if (msg.equals(offTopic)) {
        digitalWrite(default_D4, LOW); // Desliga o LED
        EstadoSaida = '0'; // Atualiza estado
    }
}

void VerificaConexoesWiFIEMQTT() {
    if (!MQTT.connected()) // Verifica se está conectado ao MQTT
        reconnectMQTT();
    reconectWiFi(); // Verifica conexão Wi-Fi
}

void EnviaEstadoOutputMQTT() {
    // Envia o estado atual do LED via MQTT
    if (EstadoSaida == '1') {
        MQTT.publish(default_TOPICO_PUBLISH_1, "s|on");
        Serial.println("- LED Ligado");
    } else if (EstadoSaida == '0') {
        MQTT.publish(default_TOPICO_PUBLISH_1, "s|off");
        Serial.println("- LED Desligado");
    }

    delay(1000); // Aguarda antes de enviar novamente
}

void handleLuminosity() {
    const int potPin = 34; // Pino do sensor de luminosidade
    int sensorValue = analogRead(potPin); // Lê valor analógico
    int luminosity = map(sensorValue, 0, 4095, 0, 100); // Mapeia para percentual
    String mensagem = String(luminosity);
    Serial.print("Valor da luminosidade: ");
    Serial.println(mensagem.c_str());
    MQTT.publish(default_TOPICO_PUBLISH_2, mensagem.c_str()); // Publica luminosidade
}

void handleDHT() {
    float humidity = dht.readHumidity(); // Lê umidade
    float temperature = dht.readTemperature(); // Lê temperatura

    // Verifica se a leitura falhou
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Falha ao ler do DHT!");
        return;
    }

    Serial.print("Temperatura: ");
    Serial.println(temperature);
    Serial.print("Umidade: ");
    Serial.println(humidity);

    // Publica temperatura e umidade
    MQTT.publish(default_TOPICO_PUBLISH_4, String(temperature).c_str());
    MQTT.publish(default_TOPICO_PUBLISH_3, String(humidity).c_str());
}

void reconnectMQTT() {
    // Tenta reconectar ao broker MQTT
    while (!MQTT.connected()) {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(default_BROKER_MQTT);
        if (MQTT.connect(default_ID_MQTT)) {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(default_TOPICO_SUBSCRIBE); // Inscreve no tópico
        } else {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Haverá nova tentativa de conexão em 2s");
            delay(2000); // Aguarda antes da nova tentativa
        }
    }
}






