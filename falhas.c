#include "serial.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

static uint16_t transaction_id = 0;

void enviarPacoteInvalido(const uint8_t *pdu, uint8_t pdu_len, const char *descricao) {
    uint8_t req[260] = {0};

    req[0] = (transaction_id >> 8) & 0xFF;  // Transaction ID
    req[1] = transaction_id & 0xFF;
    req[2] = 0x00;  // Protocol ID
    req[3] = 0x00;
    req[4] = 0x00;  // Length (high)
    req[5] = pdu_len + 1;  // Length = Unit ID + PDU
    req[6] = 0x01;  // Unit ID válido (exceto no teste de endereço)

    memcpy(&req[7], pdu, pdu_len);

    printf("🚫 %s\n", descricao);
    modbusWrite((const char *)req, 7 + pdu_len);
    lerResposta();

    transaction_id = (transaction_id + 1) % 0x10000;
}

void enviarFuncaoInvalida(void) {
    uint8_t pdu[] = {0x99, 0x00, 0x00, 0x00, 0x01};  // função 0x99 inexistente
    enviarPacoteInvalido(pdu, sizeof(pdu), "Funcao inexistente (0x99)");
}

void enviarRegistradorInvalido(void) {
    uint8_t pdu[] = {0x0F, 0xFF, 0xF0, 0x00, 0x10, 0x02, 0x00, 0x00};  // Endereço alto
    enviarPacoteInvalido(pdu, sizeof(pdu), "Endereco de registrador invalido");
}

void enviarValorInvalido(void) {
    uint8_t pdu[] = {0x0F, 0x00, 0x00, 0x00, 0x10, 0x01, 0xFF};  // ByteCount < esperado
    enviarPacoteInvalido(pdu, sizeof(pdu), "Valor de dado invalido");
}

void enviarEnderecoInvalido(void) {
    uint8_t req[260] = {0};
    uint8_t pdu[] = {0x0F, 0x00, 0x00, 0x00, 0x10, 0x02, 0xFF, 0xFF};

    // MBAP + Unit ID inválido (0x09, por ex)
    req[0] = (transaction_id >> 8) & 0xFF;
    req[1] = transaction_id & 0xFF;
    req[2] = 0x00;
    req[3] = 0x00;
    req[4] = 0x00;
    req[5] = sizeof(pdu) + 1;
    req[6] = 0x09;  // escravo inexistente
    memcpy(&req[7], pdu, sizeof(pdu));

    printf("🚫 Endereco de escravo inexistente (ID 0x09)\n");
    modbusWrite((const char *)req, 7 + sizeof(pdu));
    lerResposta();

    transaction_id = (transaction_id + 1) % 0x10000;
}
