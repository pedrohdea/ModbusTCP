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

#define TAMANHO_BUFFER 512
#define DEFAULT_PORT 502
#define IP_SERVIDOR "192.168.0.27"

unsigned char sendbuf[TAMANHO_BUFFER];
int id = 0;

void testPing(void)
{
    sendbuf[0] = id >> 8;
    sendbuf[1] = id & 0xff;
    sendbuf[2] = 0x00;
    sendbuf[3] = 0x00;
    sendbuf[4] = 0x00;
    sendbuf[5] = 0x06;
    sendbuf[6] = 0x01;
    sendbuf[7] = 0x05;
    sendbuf[8] = 0x00;
    sendbuf[9] = 0x02;
    sendbuf[10] = 0xFF;
    sendbuf[11] = 0x00;

    modbusWrite((const char *)sendbuf, 12);

    id++;
    sleep(1);
}

void testChess(void)
{
    sendPosicao();
    sendPosicao();
}

int main()
{
    if (startSocket(IP_SERVIDOR, DEFAULT_PORT) != 0)
    {
        return 1;
    }
    testPing();

    testChess();

    enviarEnderecoInvalido();

    enviarFuncaoInvalida();

    enviarRegistradorInvalido();

    enviarValorInvalido();

    closeSocket();
    printf("🔌 Conexão encerrada.\n");
    return 0;
}
