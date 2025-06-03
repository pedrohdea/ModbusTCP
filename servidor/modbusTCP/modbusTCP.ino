/*
 * Servidor TCP com ESP32 - Para receber os múltiplos dados
 * Reage a comandos Modbus TCP recebidos via socket na porta 502
 * Pode ser expandido para interpretar comandos Modbus corretamente
 */

#include <WiFi.h>

// ================== CONFIGURAÇÕES ==================
// const char* ssid = "Assump";          // Nome da rede Wi-Fi
// const char* password = "cocacola";   // Senha da rede Wi-Fi
const uint8_t endereco_escravo = 1;  // Unit ID do Modbus TCP
const char* ssid = "IFRS-ALUNOS";       // Nome da rede Wi-Fi
const char* password = "ifrsfarroupilha"; // Senha da rede Wi-Fi

#define NUM_COILS 16                 // Total de coils simulados
#define LED_STATUS 2                // LED do ESP32 (GPIO2)

// ================== VARIÁVEIS GLOBAIS ==================
WiFiServer server(502);
WiFiClient client;
bool coils[NUM_COILS] = {false};     // Simulação de coils

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);
  pinMode(LED_STATUS, OUTPUT);
  digitalWrite(LED_STATUS, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Conectando à rede Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConectado com sucesso!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  blink(500);
  digitalWrite(LED_STATUS, HIGH);

  server.begin();
  Serial.println("Servidor Modbus TCP iniciado na porta 502");
}

// ================== LOOP PRINCIPAL ==================
void loop() {
  client = server.available();
  if (client) {
    Serial.println("Cliente conectado.");
    digitalWrite(LED_STATUS, HIGH);

    while (client.connected()) {
      digitalWrite(LED_STATUS, HIGH);
      if (client.available()) {
        int dataAvailable = client.available();
        if (dataAvailable > 260) dataAvailable = 260;

        byte buffer[260];
        for (int i = 0; i < dataAvailable; i++) {
          buffer[i] = client.read();
        }

        digitalWrite(LED_STATUS, LOW);
        delay(500);
        Serial.println("Dados recebidos:");
        exibirHex(buffer, dataAvailable);

        if (dataAvailable >= 8) {
          byte unitId = buffer[6];
          byte funcCode = buffer[7];

          if (unitId == endereco_escravo || unitId == 0) {
            switch (funcCode) {
              case 0x0F:
                funcaoWriteMultipleCoils(buffer, dataAvailable);
                break;
              case 0x0E:
                funcaoEco(buffer, dataAvailable);
                break;
              default:
                enviaExcecao(buffer, 0x01);  // Função inválida
                break;
            }
          }
        }
      }
    }

    client.stop();
    Serial.println("Cliente desconectado.");
  }
}

// ================== FUNÇÕES AUXILIARES ==================
void exibirHex(const byte* data, int len) {
  for (int i = 0; i < len; i++) {
    Serial.printf("%02X ", data[i]);
  }
  Serial.println();
}

void blink(int ms) {
  digitalWrite(LED_STATUS, HIGH);
  delay(ms);
  digitalWrite(LED_STATUS, LOW);
  delay(ms);
}

// ================== MODBUS TCP ==================
void enviaExcecao(byte* req, byte codigo) {
  byte resp[9];
  memcpy(resp, req, 4);            // Transaction ID + Protocol ID
  resp[4] = 0x00;
  resp[5] = 0x03;                  // Comprimento = 3 bytes
  resp[6] = req[6];                // Unit ID
  resp[7] = req[7] | 0x80;         // Função com MSB ativado
  resp[8] = codigo;                // Código de exceção

  client.write(resp, 9);
}

void imprimirCoils() {
  Serial.print("Estado atual dos coils: ");
  for (int i = 0; i < NUM_COILS; i++) {
    Serial.print(coils[i] ? "1" : "0");
    Serial.print(' ');

    // Quebra de linha a cada 8 coils, se desejar
    if ((i + 1) % 8 == 0) Serial.print("| ");
  }
  Serial.println();
}

void funcaoWriteMultipleCoils(byte* req, int length) {
  if (length < 13) {
    enviaExcecao(req, 0x03); // Erro de tamanho
    return;
  }

  uint16_t startAddr = (req[8] << 8) | req[9];
  uint16_t quantity = (req[10] << 8) | req[11];
  byte byteCount = req[12];

  if (startAddr + quantity > NUM_COILS || byteCount != (quantity + 7) / 8) {
    enviaExcecao(req, 0x03); // Dado inválido
    return;
  }

  // Escreve nos coils simulados
  for (int i = 0; i < quantity; i++) {
    int byteIndex = 13 + (i / 8);
    int bitIndex = i % 8;
    bool bit = (req[byteIndex] >> bitIndex) & 0x01;
    coils[startAddr + i] = bit;
  }
  //print coils
  imprimirCoils();

  // Resposta Modbus TCP
  byte resp[12];
  memcpy(resp, req, 4); // Transaction ID + Protocol ID
  resp[4] = 0x00;
  resp[5] = 0x06;       // Comprimento = 6 bytes
  resp[6] = req[6];     // Unit ID
  resp[7] = 0x0F;       // Código da função
  resp[8] = req[8];     // Addr High
  resp[9] = req[9];     // Addr Low
  resp[10] = req[10];   // Qtd High
  resp[11] = req[11];   // Qtd Low

  client.write(resp, 12);
}

void funcaoEco(byte* req, int length) {
  // Copia a requisição completa como resposta
  byte resp[260];
  if (length > sizeof(resp)) length = sizeof(resp);

  memcpy(resp, req, length);
  client.write(resp, length);

  Serial.println("Função Eco executada:");
  exibirHex(resp, length);
  blink(25);
}

