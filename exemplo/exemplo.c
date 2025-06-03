//****************************************************************************
// Codigo adaptado para Linux: Cliente Modbus TCP usando sockets POSIX
//****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

#define TAMANHO_BUFFER 512
#define DEFAULT_PORT "502"
#define IP_SERVIDOR "192.168.0.27"

void exibeDados(unsigned char *buff, int length) {
    for (int i = 0; i < length; i++) {
        printf("%02X ", buff[i]);
        if (i == 6) printf("| ");
    }
    printf("\n");
}

int main() {
    int sockfd, bytes;
    struct addrinfo hints, *res;
    unsigned char sendbuf[TAMANHO_BUFFER];
    unsigned char recvbuf[TAMANHO_BUFFER];
    int id = 0; // Transaction ID

    // Configura hints para getaddrinfo
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // Resolve endereco e porta
    int status = getaddrinfo(IP_SERVIDOR, DEFAULT_PORT, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    // Cria socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) {
        perror("socket");
        freeaddrinfo(res);
        return 1;
    }

    // Conecta ao servidor
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
        perror("connect");
        close(sockfd);
        freeaddrinfo(res);
        return 1;
    }

    freeaddrinfo(res);
    printf("Conexao estabelecida!\n");

    while (1) {
        // Monta quadro Modbus TCP (Write Single Coil)
        sendbuf[0] = id >> 8;
        sendbuf[1] = id & 0xff;
        sendbuf[2] = 0x00;
        sendbuf[3] = 0x00;
        sendbuf[4] = 0x00;
        sendbuf[5] = 0x06; // Remaining bytes
        sendbuf[6] = 0x01; // Unit ID
        sendbuf[7] = 0x05; // Function code
        sendbuf[8] = 0x00; // Address high
        sendbuf[9] = 0x02; // Address low
        sendbuf[10] = 0xFF; // Data high (0xFF00 = ON)
        sendbuf[11] = 0x00; // Data low

        int tamReq = 12;

        printf("Enviando comando:\t");
        exibeDados(sendbuf, tamReq);

        // Envia dados
        bytes = send(sockfd, sendbuf, tamReq, 0);
        if (bytes < 0) {
            perror("send");
            break;
        }

        // Recebe resposta
        bytes = recv(sockfd, recvbuf, TAMANHO_BUFFER, 0);
        if (bytes < 0) {
            perror("recv");
            break;
        } else if (bytes == 0) {
            printf("Conexao encerrada pelo servidor.\n");
            break;
        } else {
            printf("Resposta recebida:\t");
            exibeDados(recvbuf, bytes);
        }

        id++;
        sleep(1);
    }

    // Encerra conexao
    close(sockfd);
    printf("Conexao encerrada.\n");
    return 0;
}
