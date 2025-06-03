#include "serial.h"           // Sua biblioteca de serial (não usada aqui, mas mantida caso você use futuramente)
#include <sys/time.h>         // Para gettimeofday
#include <stdio.h>            // Para printf, perror
#include <stdlib.h>           // Para malloc, free etc.
#include <string.h>           // Para memset
#include <unistd.h>           // Para close(), read()
#include <errno.h>            // Para errno
#include <sys/socket.h>       // Para sockets TCP
#include <netinet/in.h>       // Para sockaddr_in
#include <arpa/inet.h>        // Para inet_addr

#define TIMEOUT_TOTAL_MS 2000         // Timeout máximo de espera (2 segundos)

int sockfd = -1;    // Socket global (deve ser conectado antes)


// Função para obter o tempo atual em milissegundos
unsigned long millis_now() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000UL) + (tv.tv_usec / 1000UL);
}

void exibeDados(const unsigned char *buff, int length) {
    for (int i = 0; i < length; i++) {
        printf("%02X ", buff[i]);
        if (i == 6) printf("| ");
    }
    printf("\n");
}

int startSocket(const char *ip, int porta) {
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Erro ao criar socket");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(porta);

    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        perror("Endereço IP inválido");
        close(sockfd);
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erro ao conectar");
        close(sockfd);
        return -1;
    }

    // Configura timeout para recv() com SO_RCVTIMEO
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_TOTAL_MS / 1000;
    timeout.tv_usec = (TIMEOUT_TOTAL_MS % 1000) * 1000;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    printf("Conectado ao servidor %s:%d\n", ip, porta);
    return 0;
}

// Função para fechar o socket
void closeSocket(void) {
    if (sockfd >= 0) {
        close(sockfd);
        sockfd = -1;
        printf("🔌 Socket fechado.\n");
    }
}

// Função para receber e validar a resposta Modbus TCP
char *lerResposta(void) {
    static unsigned char buffer[TAMANHO_BUFFER];
    memset(buffer, 0, sizeof(buffer));

    unsigned long inicio = millis_now();
    printf("🔍 Aguardando frame até %lu ms...\n", inicio + TIMEOUT_TOTAL_MS);

    // Realiza leitura da resposta
    int bytesRecebidos = recv(sockfd, buffer, TAMANHO_BUFFER, 0);
    unsigned long agora = millis_now();

    if (bytesRecebidos <= 0) {
        printf("⏱️  Timeout global de %lu ms atingido.\n", agora - inicio);
        perror("❌ Falha ao receber dados");
        return NULL;
    }

    //Utilizado nesse trecho para analisar a exceção; correto seria depois da exceção
    printf("📥 Resposta recebida (%d bytes): ", bytesRecebidos);
    exibeDados(buffer, bytesRecebidos);

    // Verifica se houve exceção Modbus (bit mais significativo de function code está setado)
    if (buffer[7] & 0x80) {
        printf("❗ Exceção Modbus recebida: Código 0x%02X\n", buffer[8]);
        return NULL;
    }

    return (char *)buffer;
}

// Envia um frame Modbus TCP e trata a resposta
int modbusWrite(const char *sendbuf, int tamReq)
{
    printf("📤 Enviando: ");
    exibeDados((unsigned char *)sendbuf, tamReq); // cast necessário se exibeDados espera unsigned char*

    // Envia os dados pela conexão TCP
    int bytes = send(sockfd, sendbuf, tamReq, 0);
    if (bytes != tamReq)
    {
        fprintf(stderr, "❌ Falha ao enviar dados. Esperado %d bytes, enviados %d.\n", tamReq, bytes);
        return 0;
    }

    // Aguarda e processa a resposta do servidor
    char *resposta = lerResposta();
    // Verifica se houve resposta
    if (resposta == NULL) {
        printf("❌ Nenhuma resposta válida.\n");
    } else if (!(resposta[0] == sendbuf[0] && resposta[1] == sendbuf[1])) {
        printf("❌ Resposta inválida (transaction ID diferente).\n");
    } else {
        printf("✅ Resposta válida recebida.\n");
        return 0;
    }

    return 1;
}
