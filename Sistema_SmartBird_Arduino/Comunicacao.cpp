#include "Comunicacao.h"

// Buffer interno de recepção.
static char bufferRecepcao[TAMANHO_BUFFER_MENSAGEM];

static size_t indiceRecepcao = 0;
static bool descartandoMensagem = false;
static bool overflowDetectado = false;

// ==========================================
// INICIALIZAÇÃO
// ==========================================

void inicializarComunicacao() {

#if SMARTBIRD_COMUNICACAO_USB
    // Serial USB já é inicializada no setup() do arquivo .ino.
    // Não chamar Serial.begin() novamente aqui.
#else
    // UART dedicada ao ESP8266.
    Serial1.begin(9600);
#endif

    indiceRecepcao = 0;
    descartandoMensagem = false;
    overflowDetectado = false;
}

// ==========================================
// RECEBER MENSAGEM
// ==========================================

Stream& portaComunicacao() {
#if SMARTBIRD_COMUNICACAO_USB
    return Serial;
#else
    return Serial1;
#endif
}

const char* receberMensagem() {
    Stream& porta = portaComunicacao();
    while (porta.available() > 0) {
        const char caractere = porta.read();

        // Final da mensagem.
        if (caractere == '\n') {

            if (descartandoMensagem) {
                descartandoMensagem = false;
                indiceRecepcao = 0;
                continue;
            }

            // Ignora mensagens vazias.
            if (indiceRecepcao == 0) {
                continue;
            }

            bufferRecepcao[indiceRecepcao] = '\0';
            indiceRecepcao = 0;
            return bufferRecepcao;
        }

        // Ignora o CR usado em finais de linha CRLF.
        if (caractere == '\r') {
            continue;
        }

        // Se a mensagem exceder o limite, descarta até o próximo \n.
        if (descartandoMensagem) {
            continue;
        }

        if (indiceRecepcao >= MAX_BYTES_MENSAGEM) {

            indiceRecepcao = 0;
            descartandoMensagem = true;
            overflowDetectado = true;

            continue;
        }

        bufferRecepcao[indiceRecepcao] = caractere;
        indiceRecepcao++;
    }

    return nullptr;
}

// ==========================================
// CONSULTAR OVERFLOW
// ==========================================

bool houveOverflowComunicacao() {

    bool resultado = overflowDetectado;

    overflowDetectado = false;

    return resultado;
}
