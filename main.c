#include <stdio.h>
#include "teste.h"
#include "writecoils.h"
#include "mapa.h"
#include "falhas.h"
#include "serial.h"

#define TAMANHO_BUFFER 512
#define DEFAULT_PORT 502
#define IP_SERVIDOR "192.168.0.21"

void mostrarHeader() {
    printf("=========================================\n");
    printf("MODBUS TCP CLIENT - PEDRO HENRIQUE DE ASSUMPÇÃO\n");
    printf("=========================================\n\n");
}

void mostrarMenu() {
    printf("\n");
    printf("1. Escrever múltiplos coils (0x0F)\n");
    printf("2. Enviar comando com endereço inválido (teste de timeout)\n");
    printf("3. Enviar função inválida (teste de exceção 01)\n");
    printf("4. Enviar registrador inválido (teste de exceção 02)\n");
    printf("5. Enviar valor inválido (teste de exceção 03)\n");
    printf("x. Sair\n");
}

int main() {
    mostrarHeader();

    if (startSocket(IP_SERVIDOR, DEFAULT_PORT) != 0) {
        return 1;
    }
    
    char opcao;

    do {
        mostrarMenu();
        printf("Digite a opção desejada: ");
        scanf(" %c", &opcao);  // espaço ignora ENTER anterior
        switch (opcao) {
            case '1':
                printf("\n1. Escrever múltiplos coils (0x0F)\n");
                sendPosicao(); // writecoils.h
                break;
            case '2':
                printf("\n2. Enviar comando com endereço inválido (teste de timeout)\n");
                enviarEnderecoInvalido();
                break;
            case '3':
                printf("\n3. Enviar função inválida (teste de exceção 01)\n");
                enviarFuncaoInvalida();
                break;
            case '4':
                printf("\n4. Enviar registrador inválido (teste de exceção 02)\n");
                enviarRegistradorInvalido();
                break;
            case '5':
                printf("\n5. Enviar valor inválido (teste de exceção 03)\n");
                enviarValorInvalido();
                break;
            case 't':
            case 'T':
                printf("\nT. Teste ECO\n");
                testEco();
                break;
            case 'x':
            case 'X':
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }

        printf("\n");

    } while (opcao != 'x' && opcao != 'X');

    closeSocket();
    printf("🔌 Conexão encerrada.\n");
    return 0;
}

