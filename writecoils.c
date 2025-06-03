#include "serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mapa.h"
#include <unistd.h>

int escreverMultiplosCoils(int inicio, int quantidade, int byteCount, const unsigned char *dados) {
    static unsigned short transaction_id = 0;
    transaction_id = (transaction_id + 1) % 0x10000;  // Incrementa e reseta em 65536

    unsigned char req[260] = {0};

    // MBAP Header
    req[0] = (transaction_id >> 8) & 0xFF;  // Transaction ID high
    req[1] = transaction_id & 0xFF;         // Transaction ID low
    req[2] = 0x00;                          // Protocol ID high
    req[3] = 0x00;                          // Protocol ID low
    req[4] = 0x00;                          // Length high (to be set below)
    req[5] = 7 + byteCount;                 // Length low: Unit ID + Function + 5 bytes + data
    req[6] = 0x01;                          // Unit ID

    // PDU
    req[7] = 0x0F;                          // Function code
    req[8] = (inicio >> 8) & 0xFF;        // Coil start address high
    req[9] = inicio & 0xFF;               // Coil start address low
    req[10] = (quantidade >> 8) & 0xFF;     // Quantity high
    req[11] = quantidade & 0xFF;            // Quantity low
    req[12] = byteCount;                   // Byte count

    memcpy(&req[13], dados, byteCount);

    int total = 13 + byteCount;

    modbusWrite((const char *)req, total);
    return 1;
}

int posicaoParaIndice(char coluna, char linha)
{
    coluna = toupper(coluna);
    if (coluna < 'A' || coluna > 'H' || linha < '1' || linha > '8')
        return -1;
    int col = coluna - 'A';
    int lin = linha - '1';
    return lin * 8 + col;
}

/**
 * @brief Lê uma jogada do usuário no formato "A2H8" e aciona os coils correspondentes no tabuleiro.
 *
 * Essa função solicita ao usuário que digite uma jogada no formato de coordenadas iniciais e finais
 * (por exemplo, "A2H8"), converte essas posições para um mapa de bits (coils) e envia uma requisição
 * Modbus RTU para ativar os LEDs correspondentes usando a função 0x0F (Write Multiple Coils).
 *
 * A jogada é mapeada em um vetor de 16 bytes, mas apenas os dois primeiros bytes são utilizados (16 bits),
 * sendo cada bit responsável por acionar uma posição na matriz 8x8 do tabuleiro.
 *
 * Exemplo:
 * - Jogada: "A2H8"
 * - Resultado esperado nos coils:
 *   - Byte 0: colunas ativadas (bit 0 para 'A', bit 7 para 'H')
 *   - Byte 1: linhas ativadas (bit 1 para '2', bit 7 para '8')
 *
 * @note Essa função assume que a matriz do tabuleiro tem exatamente 8 colunas e 8 linhas (64 posições),
 *       e que o hardware usa dois 74HC595 para controlar 16 LEDs/coils.
 */
void sendPosicao(void)
{
    char comando[5];
    printf("Digite a jogada (ex: A2H8): ");
    scanf("%4s", comando);

    if (strlen(comando) != 4)
    {
        printf("❌ Entrada inválida. Use 4 caracteres (ex: A2H8).\n");
        return;
    }

    printf("📍 Início: coluna = %c, linha = %c\n", comando[0], comando[1]);
    printf("📍 Fim:    coluna = %c, linha = %c\n", comando[2], comando[3]);

    uint8_t posicaoDe[16] = {0};
    uint8_t posicaoPara[16] = {0};
    gerarMapaCoils(comando, posicaoDe, posicaoPara);  // Preenche os 2 primeiros bytes

    printf("🧠 Coils a enviar:\n");
    exibeBits(posicaoDe, 2);  // Mostra só os dois bytes
    exibeBits(posicaoPara, 2);  // Mostra só os dois bytes

    escreverMultiplosCoils(0, 16, 2, posicaoDe);  // Envia sempre 16 coils, começando do endereço 0
    sleep(1);  // espera 1 segundo
    escreverMultiplosCoils(0, 16, 2, posicaoPara);  // Envia sempre 16 coils, começando do endereço 0
}
