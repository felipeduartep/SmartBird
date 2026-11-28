#ifndef COMUNICACAO_H
#define COMUNICACAO_H

#include <Arduino.h>

// Seleção da comunicação:
//
// 0 = Serial1 (ESP8266, 9600 baud)
// 1 = Serial USB (Monitor Serial, 115200 baud)

#define SMARTBIRD_COMUNICACAO_USB 0

// Limite máximo de uma mensagem.
constexpr size_t MAX_BYTES_MENSAGEM = 1024;
constexpr size_t TAMANHO_BUFFER_MENSAGEM = MAX_BYTES_MENSAGEM + 1;

// Inicializa a comunicação.
void inicializarComunicacao();

// Retorna uma linha completa sem o terminador, válida até a próxima chamada.
// Retorna nullptr quando ainda não há uma mensagem completa.
const char* receberMensagem();

// Porta usada para receber e enviar as linhas do protocolo.
Stream& portaComunicacao();

// Consulta e limpa o indicador de overflow.
bool houveOverflowComunicacao();

#endif
