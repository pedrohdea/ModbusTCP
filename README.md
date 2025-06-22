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

## Para criar executavel e executar main.c
´´´
gcc -o run main.c teste.c writecoils.c mapa.c falhas.c serial.c 
´´´
Executar:
´´´
./run
´´´
## Para compilar e fazer upload no ESP32

Baixar arduino IDE
https://www.arduino.cc/en/software/

### Passos para instalar o Arduino IDE 2.3.6 (Linux)
Extraia o arquivo .zip:
´´´
unzip arduino-ide_2.3.6_Linux_64bit.zip -d arduino
cd arduino
´´´

Corrija a permissão, torne o arquivo executável:
´´´
chmod +x arduino-ide
´´´

### (Opcional) Criar um atalho no menu do sistema:
Se quiser criar um atalho de menu para a IDE:

Crie um arquivo chamado arduino-ide.desktop em ~/.local/share/applications/:

´´´
nano ~/.local/share/applications/arduino-ide.desktop
´´´

´´´
[Desktop Entry]
Name=Arduino IDE
Comment=Arduino IDE 2.x
Exec=/caminho/para/arduino/arduino-ide
Icon=/caminho/para/arduino/resources/app/static/arduino-logo.ico
Terminal=false
Type=Application
Categories=Development;IDE;
´´´

### Baixar extensão do ESP32 expressif
Tools > Preferences > Baixar

Selecionar ESP32 DEV MODULE

Upload do código com USB conectado

# Fluxo de Comunicação: PC (Cliente) ↔ ESP32 (Servidor)

## Etapas do Processo

1. **Início do Programa no Cliente**  
   O usuário executa o software em C pelo terminal Linux.

2. **Navegação no Menu**  
   O usuário escolhe a **opção 1 – Write Multiple Coils**.

3. **Entrada da Jogada de Xadrez**  
   O usuário informa a jogada no formato padrão (ex: `D2D4`).

4. **Conversão da Jogada para Padrão Binário**  
   A jogada é convertida em um mapa de bits que define os LEDs a serem acesos.

5. **Geração dos Dados dos Coils**  
   O padrão de 16 bits é dividido em dois bytes: `resultadoA` e `resultadoB`.

6. **Montagem do Quadro MODBUS TCP**  
   O quadro inclui:
   - Cabeçalho MBAP  
   - Função 0x0F (Write Multiple Coils)  
   - Dados dos coils (`resultadoA`, `resultadoB`)

7. **Envio do Quadro via Wi-Fi**  
   O cliente envia o quadro para o ESP32 pela porta **502**.

8. **Início da Contagem de Timeout**

9. **Recebimento e Validação no Servidor (ESP32)**  
   O ESP32 valida o cabeçalho e os dados recebidos.

10. **Interpretação e Log do Comando**  
    O ESP32 interpreta os dados e imprime o comando recebido no Serial Monitor.

11. **Resposta do Servidor**  
    O ESP32 envia um quadro de confirmação com os dados esperados.

12. **Validação da Resposta no Cliente**  
    O cliente verifica a integridade da resposta.

13. **Exibição de Resultado**
    - Se válida, exibe **“OK”**;
    - Caso contrário, executa **tratamento de falha**.



# Apresentação: Projeto 02 – Dispositivo Modbus TCP

**Curso**: Engenharia de Controle e Automação  
**Instituição**: IFRS – Campus Farroupilha  
**Disciplina**: Barramentos Industriais  
**Professor**: Gustavo Künzel  
**Aluno**: Pedro Henrique de Assumpção  
**Data**: 24/06/2025

---

## Índice

- Introdução  
- Objetivos  
- Justificativa  
- Fundamentação Teórica  
- Proposta  
- Testes  
- Desafios  
- Referências

---

## Introdução

Utilizar conceitos de Modbus TCP e comunicação pela rede Ethernet para desenvolver uma aplicação compatível com o protocolo. O objetivo é compreender como deve ser feita a programação do protocolo nos dispositivos.

---

## Objetivos

- Módulo de saídas digitais (8 a 16 saídas);
- Representadas por mensagens no ESP32 (servidor);
- O programa do PC (cliente) deve enviar comandos ao ESP32;
- Utilizar a função Modbus 0x0F (Write Multiple Coils).

---

## Justificativa

Desenvolver um sistema que permita o envio de comandos ao servidor a partir de uma conexão cliente, utilizando o ESP32 com rede integrada. Também busca-se compreender a implementação prática do protocolo e sua aplicabilidade em sistemas embarcados.

---

## Fundamentação Teórica

### Protocolo Modbus TCP

- Baseado em Ethernet, permite troca de dados via TCP/IP.
- Modelo cliente-servidor.
- Encapsula os dados do Modbus RTU em pacotes TCP/IP, preservando a estrutura lógica.

### Quadro da Mensagem

| Campo             | Seção        | Descrição                                  | Tamanho (bytes) | Exemplo (Hex) |
|------------------|--------------|--------------------------------------------|------------------|----------------|
| Transaction ID    | MBAP Header  | ID da transação definido pelo cliente      | 2                | 00 01          |
| Protocol ID       | MBAP Header  | Sempre 0x0000 para Modbus                  | 2                | 00 00          |
| Length            | MBAP Header  | Nº de bytes restantes (Unit ID + PDU)      | 2                | 00 08          |
| Unit ID           | MBAP Header  | Identificação do dispositivo               | 1                | 01             |
| Function Code     | PDU          | 0x0F (Write Multiple Coils)                | 1                | 0F             |
| Starting Address  | PDU          | Endereço inicial dos coils                 | 2                | 00 13          |
| Quantity of Coils | PDU          | Quantidade de coils (10)                   | 2                | 00 0A          |
| Byte Count        | PDU          | Nº de bytes nos valores dos coils          | 1                | 02             |
| Output Values     | PDU          | Bits para os coils                         | 2 ou +           | 55 01          |

---

## Proposta

### Componentes do Sistema

- ESP32-WROOM-32 configurado como servidor Modbus TCP;
- Comunicação via Wi-Fi, porta 502;
- IP dinâmico atribuído pela rede;
- Estrutura das mensagens segue o padrão Modbus TCP.

### Matriz de LEDs

- Matriz 8x8 com 64 LEDs;
- Controlada por registros que determinam quais LEDs acender;
- Permite criar padrões visuais.

---

## Fluxograma Explicado

1. Usuário executa o programa em C no terminal Linux.  
2. Usuário seleciona a opção 1 no menu (Write Multiple Coils).  
3. Usuário informa a jogada de xadrez (ex: D2D4).  
4. Programa converte a jogada em padrão binário.  
5. Divide os 16 bits em `resultadoA` e `resultadoB`.  
6. Monta quadro Modbus TCP (função 0x0F).  
7. Envia o quadro via Wi-Fi (porta 502).  
8. Inicia contagem de timeout.  
9. ESP32 recebe e valida o quadro.  
10. Interpreta os dados e imprime no Serial Monitor.  
11. Envia quadro de resposta ao cliente.  
12. Cliente valida a resposta.  
13. Se válida, exibe "OK".  
14. Se inválida ou sem resposta, trata o erro.

---

## Testes

### Procedimentos

- Testes confirmaram a comunicação entre cliente e servidor;
- A saída em ambos os lados exibe o mesmo padrão binário.

### Rotinas de Falhas

- O menu do cliente trata erros como endereço inválido e ausência de resposta;
- Exibe mensagens de erro e permite nova tentativa sem travar o sistema.

---

## Desafios

- Melhorar a configuração com parametrização de:
  - Endereço de rede;
  - Senha;
  - Porta serial.
- Tornar o sistema mais flexível e reutilizável com diferentes dispositivos.

---

## Referências

- MODBUS ORGANIZATION. *MODBUS Messaging on TCP/IP Implementation Guide V1.0b*. [S.l.], 2006. Disponível em: https://modbus.org/docs/Modbus_Messaging_Implementation_Guide_V1_0b.pdf. Acesso em: 22 jun. 2025.

- SCHNEIDER ELECTRIC. *Modbus Protocol Reference Guide*. [S.l.], [s.d.]. Disponível em: https://www.se.com/ww/en/download/document/Modbus_Protocol_Reference/. Acesso em: 22 jun. 2025.

- PEDRO HENRIQUE DE ASSUMPÇÃO. *ModbusTCP*. GitHub, 2025. Disponível em: https://github.com/pedrohdea/ModbusTCP. Acesso em: 22 jun. 2025.

- ESPRESSIF SYSTEMS. *ESP32 – TCP/IP Server Example*. Documentation. [S.l.], [s.d.]. Disponível em: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/lwip.html#tcp-server. Acesso em: 22 jun. 2025.

