/*
 * Servidor TCP com ESP32 - Exemplo com retorno (echo) dos dados recebidos
 * Reage a comandos Modbus TCP recebidos via socket na porta 502
 * Pode ser expandido para interpretar comandos Modbus corretamente
 */

#include <WiFi.h>

// Configurações da rede Wi-Fi
//const char* ssid = "IFRS-ALUNOS";       // Nome da rede Wi-Fi
//const char* password = "ifrsfarroupilha"; // Senha da rede Wi-Fi
const char* ssid = "Assump";       // Nome da rede Wi-Fi
const char* password = "cocacola"; // Senha da rede Wi-Fi


// Porta padrão para comunicação Modbus TCP
WiFiServer server(502);

// Pino conectado ao LED do ESP32 (geralmente GPIO 2)
const int ledPin = 2;

void setup() {
  Serial.begin(115200);                // Inicializa a comunicação serial
  pinMode(ledPin, OUTPUT);             // Configura o pino do LED como saída
  digitalWrite(ledPin, LOW);           // Garante que o LED comece desligado

  // Conecta à rede Wi-Fi especificada
  WiFi.begin(ssid, password);
  Serial.print("Conectando à rede Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConectado com sucesso!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Inicia o servidor na porta 502
  server.begin();
  Serial.println("Servidor TCP iniciado!");
}

void loop() {
  // Verifica se há um cliente conectado
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Cliente conectado.");

    // Enquanto o cliente estiver conectado
    while (client.connected()) {
      // Verifica se há dados recebidos do cliente
      if (client.available()) {
        int available = client.available();
        char receivedChar[20];  // Buffer para armazenar os dados recebidos (limitado a 20 bytes)

        // Lê os dados recebidos byte a byte
        for (int i = 0; i < available && i < sizeof(receivedChar); i++) {
          receivedChar[i] = client.read();
        }

        // Imprime os dados recebidos no terminal serial em hexadecimal
        Serial.println("Bytes recebidos:");
        for (int i = 0; i < available && i < sizeof(receivedChar); i++) {
          Serial.print(receivedChar[i], HEX);
          Serial.print(' ');
        }
        Serial.println();

        // Envia os mesmos dados de volta para o cliente (echo)
        client.write(receivedChar, available);
      }
    }

    // Cliente desconectado
    client.stop();
    Serial.println("Cliente desconectado.");
  }
}
