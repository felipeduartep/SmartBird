#include "Manutencao.h"
#include "Tempo.h"
#include "Persistencia.h"

// ==========================================
// CONFIGURAÇÕES INTERNAS
// ==========================================

static constexpr unsigned long INTERVALO_VERIFICACAO_MS = 1000UL;

// ==========================================
// CONTROLE INTERNO
// ==========================================

static unsigned long ultimaVerificacaoMs = 0;

// ==========================================
// INICIALIZAÇÃO
// ==========================================

bool inicializarManutencao() {

    ultimaVerificacaoMs = 0;

    return true;
}

// ==========================================
// ATUALIZAR MANUTENÇÃO
// ==========================================

void atualizarManutencao(DadosSistema& dados) {

    unsigned long agoraMs = millis();

    // Respeita o intervalo entre verificações.
    if (agoraMs - ultimaVerificacaoMs < INTERVALO_VERIFICACAO_MS) {
        return;
    }

    ultimaVerificacaoMs = agoraMs;

    // ----------------------------------
    // PRIMEIRA LIMPEZA
    // ----------------------------------

    if (!dados.manutencao.limpezaRegistrada) {

        dados.manutencao.limpezaPendente = true;
        dados.manutencao.alertaAtivo = true;

        return;
    }

    // ----------------------------------
    // CONSULTAR RTC
    // ----------------------------------

    DateTime agora;

    if (!obterDataHora(agora)) {
        return;
    }

    uint32_t horarioAtual = agora.unixtime();

    uint32_t ultimaLimpeza =
        dados.manutencao.ultimaLimpezaEpoch;

    // Evita erro caso o relógio tenha sido retrocedido.
    if (horarioAtual < ultimaLimpeza) {
        return;
    }

    // ----------------------------------
    // VERIFICAR INTERVALO CONFIGURADO
    // ----------------------------------

    uint32_t intervaloConfigurado =
        dados.configuracaoManutencao.intervaloLimpezaSegundos;

    // Intervalo zero é considerado configuração inválida.
    if (intervaloConfigurado == 0) {

        dados.manutencao.limpezaPendente = true;
        dados.manutencao.alertaAtivo = true;

        return;
    }

    uint32_t tempoDecorrido =
        horarioAtual - ultimaLimpeza;

    // ----------------------------------
    // VERIFICAR NECESSIDADE DE LIMPEZA
    // ----------------------------------

    if (tempoDecorrido >= intervaloConfigurado) {

        dados.manutencao.limpezaPendente = true;
        dados.manutencao.alertaAtivo = true;
    }
}

// ==========================================
// CONFIRMAR LIMPEZA
// ==========================================

bool confirmarLimpeza(DadosSistema& dados) {

    DateTime agora;

    // Só registra a limpeza se o RTC estiver disponível.
    if (!obterDataHora(agora)) {
        return false;
    }

    const uint32_t ultimaAnterior = dados.manutencao.ultimaLimpezaEpoch;
    const bool registradaAnterior = dados.manutencao.limpezaRegistrada;
    const bool pendenteAnterior = dados.manutencao.limpezaPendente;
    const bool alertaAnterior = dados.manutencao.alertaAtivo;

    dados.manutencao.ultimaLimpezaEpoch = agora.unixtime();
    dados.manutencao.limpezaRegistrada = true;
    dados.manutencao.limpezaPendente = false;
    dados.manutencao.alertaAtivo = false;

    if (!salvarDadosPersistidos(dados)) {
        dados.manutencao.ultimaLimpezaEpoch = ultimaAnterior;
        dados.manutencao.limpezaRegistrada = registradaAnterior;
        dados.manutencao.limpezaPendente = pendenteAnterior;
        dados.manutencao.alertaAtivo = alertaAnterior;
        return false;
    }

    return true;
}