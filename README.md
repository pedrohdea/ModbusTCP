# Projeto – Servidor Modbus TCP

**Barramentos Industriais**  
**IFRS – Campus Farroupilha**  
**Engenharia de Controle e Automação**  
**Professor: Gustavo Künzel**
**Aluno: Pedro Henrique de Assumpção**

---

## Objetivo

Este projeto é uma continuação do Projeto 01. Utilizar conceitos de Modbus e comunicação TCP/IP para desenvolver um dispositivo compatível com o protocolo Modbus TCP. O objetivo é compreender como deve ser feita a programação do protocolo nos dispositivos. Nas I/Os do Arduino serão conectados potenciômetros e LEDs, de acordo com a aplicação.

---

## Material necessário

- Microcontrolador com Wi-Fi (ESP32, NodeMCU) ou Arduino (Uno, Mega) com módulo Ethernet W5100;
- Componentes diversos para os I/Os;
- A porta serial (COM) do microcontrolador pode ser usada para debug;
- PC, DEV-C++, IDE, bibliotecas para envio e recebimento de comunicação TCP por sockets;
- Códigos desenvolvidos em outras disciplinas e atividades.

---

## Equipes de desenvolvimento

- Individual, seguindo o Projeto 1;
- Em dupla (neste caso, cada membro será responsável por uma parte):
  - Um aluno foca no desenvolvimento no Dev-C++;
  - Outro no Arduino/ESP.

---

## Requisitos para o Servidor (Arduino, ESP ou outro microcontrolador)

1. Montagem física em protoboard ou placa e funcionalidade do protótipo conectado à uma rede.

2. Deve implementar um servidor TCP (usando Sockets TCP) e o ADU do Modbus TCP com a função do Projeto 1:
   - a. Abrir uma comunicação TCP/IP na porta Modbus (502) e aguardar que um cliente se conecte e faça requisições;
   - b. Interpretar adequadamente o cabeçalho MBAP (responder com o transaction ID da requisição, testar o length, o unit ID e o protocol ID, ignorando a requisição quando houver erros no MBAP);
   - c. O unit ID do servidor é configurado pelas chaves de endereço e ele responde a requisições somente deste unit ID;
   - d. Implementar as funções para acesso aos I/Os propostos, inclusive respondendo com as exceções (função inválida, registrador inválido, dado inválido) em casos de erro na requisição.

---

## Programa do Cliente (PC com DEV-C++)

3. O programa é um cliente TCP:
   - a. O programa estabelece uma conexão TCP com o servidor, encerrando a conexão apenas ao término do programa;
   - b. O programa deverá ter um menu de opções que permita interagir com o escravo (similar ao Projeto 1):
     - Uma opção permitirá validar a função implementada, lendo o estado atual das entradas ou escrevendo nas saídas;
     - Alguns comandos podem permitir acesso e leitura de mais de uma entrada ou saída por vez, logo, o programa deve permitir pleno teste da função Modbus escolhida (similar ao Projeto 1);
   - c. Cada nova requisição enviada pelo cliente deve incrementar o transaction ID do cabeçalho MBAP;
   - d. O programa deve identificar e exibir na tela do prompt os seguintes erros:
     - Indicar erro se o transaction ID enviado na requisição for diferente ao recebido na resposta;
     - Erros de conexão com o servidor, TCP e sockets (o próprio código de Sockets TCP já indica alguns testes de erros);
     - Exibir ao usuário as exceções Modbus;
     - Para fins de teste, o cliente deve ter a opção de enviar um quadro com erro (unit ID diferente) para o servidor;
     - Para fins de teste de exceções, o cliente deve ter a opção de enviar um quadro com função inválida, registrador inválido e dado inválido;
   - e. Um arquivo `config.txt` deve armazenar o endereço IP e a porta do servidor. Este arquivo é lido pelo programa na inicialização.

---

> Pode-se utilizar uma ferramenta como o Modbus Poll ou Modbus Simulator para testar a compatibilidade do servidor com o protocolo Modbus TCP.

---

## Entrega e apresentação

- **Data limite da apresentação**: 24/06/2025
- **Nota total**: 5 pontos de N2

### Apresentação com slides (1 ponto):
- 15 minutos de apresentação + 5 minutos para comentários;
- Apresentação do projeto do circuito;
- Explicação da função implementada;
- Explicação sobre escolhas feitas no projeto;
- Apresentação das seções de código principais do cliente;
- Apresentação das seções de código principais no servidor;
- Demonstração funcional (pode fazer um vídeo também, para o caso de não funcionar na hora).

**Enviar um ZIP com os códigos do mestre e escravo e PDF da apresentação no Moodle.**

---

### Critérios de avaliação

| Requisito               | Pontos |
|-------------------------|--------|
| Req1                    | 0,5    |
| Req2                    | 2,0    |
| Req3                    | 1,5    |
| Apresentação            | 1,0    |
| **Total**               | **5,0**|

---

# Desenvolvimento

Para criar executavel..
´´´
gcc -o run main.c
´´´