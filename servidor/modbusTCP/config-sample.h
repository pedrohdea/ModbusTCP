#ifndef CONFIG_H
#define CONFIG_H

// ================== CONFIGURAÇÕES DE REDE ==================
const char* ssid = "NOME_DA_REDE";         // Ex: "MinhaRedeWiFi"
const char* password = "SENHA_DA_REDE";    // Ex: "12345678"

// ================== CONFIGURAÇÕES DO MODBUS ==================
const uint8_t endereco_escravo = 1;        // ID do escravo Modbus TCP
#define NUM_COILS 16                       // Número total de coils simulados

// ================== PINOS ==================
#define LED_STATUS 2                       // GPIO do LED de status (padrão: 2)

#endif
