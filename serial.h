#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#define TAMANHO_BUFFER 512

extern int sockfd;

int startSocket(const char *ip, int porta);
void closeSocket(void);
int modbusWrite(const char *req, int total);
char *lerResposta(void);
void exibeDados(const unsigned char *buff, int length);

#endif
