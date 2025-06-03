#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include "serial.h"
#include "writecoils.h"
#include "falhas.h"

unsigned char sendbuf[TAMANHO_BUFFER];
int id = 0;

void testEco(void)
{
    sendbuf[0]  = id >> 8;        // Byte alto do Transaction ID (identificador da transação)
    sendbuf[1]  = id & 0xFF;      // Byte baixo do Transaction ID
    sendbuf[2]  = 0x00;           // Byte alto do Protocol ID (sempre 0 em Modbus TCP)
    sendbuf[3]  = 0x00;           // Byte baixo do Protocol ID (sempre 0)
    sendbuf[4]  = 0x00;           // Byte alto do comprimento do restante do pacote (Unit ID + PDU)
    sendbuf[5]  = 0x06;           // Byte baixo do comprimento (6 bytes após aqui)
    sendbuf[6]  = 0x01;           // Unit ID (ID do escravo Modbus, aqui é 1)
    sendbuf[7]  = 0x0E;           // Código da função (0x05 = Escrever em uma única bobina - Write Single Coil)
    sendbuf[8]  = 0x00;           // Byte alto do endereço da bobina (coil)
    sendbuf[9]  = 0x02;           // Byte baixo do endereço da bobina (coil 2 no total: 0x0002)
    sendbuf[10] = 0xFF;           // Valor alto (0xFF00 = ligar a bobina)
    sendbuf[11] = 0x00;           // Valor baixo

    modbusWrite((const char *)sendbuf, 12);

    id++;
    sleep(1);
}