#include "Persistencia.h"

#include <EEPROM.h>
#include <math.h>
#include <stddef.h>
#include <string.h>

namespace {
constexpr uint32_t MAGIC_EEPROM = 0x53424D31UL; // "SBM1"
constexpr uint16_t VERSAO_EEPROM = 1;
constexpr int ENDERECO_SLOT_A = 0;

struct HorarioPersistido {
    uint8_t hora;
    uint8_t minuto;
    uint8_t ativo;
};

struct ConfiguracoesPersistidas {
    float pesoAlvoGramas;
    float fatorCalibracao;
    uint32_t tempoMaximoAlimentacaoMs;
    uint32_t tempoMaximoBombaMs;

    float alimentoDistanciaCheioCm;
    float alimentoDistanciaVazioCm;
    float alimentoNivelMinimoPercentual;

    float aguaDistanciaCheioCm;
    float aguaDistanciaVazioCm;
    float aguaNivelMinimoPercentual;

    uint32_t intervaloLimpezaSegundos;
    uint32_t ultimaLimpezaEpoch;
    uint8_t limpezaRegistrada;

    HorarioPersistido agendaAlimentacao[MAX_HORARIOS_DIARIOS];
    HorarioPersistido agendaHidratacao[MAX_HORARIOS_DIARIOS];
};

struct RegistroEEPROM {
    uint32_t magic;
    uint16_t versao;
    uint16_t tamanho;
    uint32_t sequencia;
    ConfiguracoesPersistidas dados;
    uint32_t crc;
};

static_assert(sizeof(RegistroEEPROM) * 2 <= 4096,
              "Os dois slots devem caber na EEPROM do Arduino Mega 2560");

uint32_t calcularCRC32(const uint8_t* bytes, size_t tamanho) {
    uint32_t crc = 0xFFFFFFFFUL;
    for (size_t i = 0; i < tamanho; ++i) {
        crc ^= bytes[i];
        for (uint8_t bit = 0; bit < 8; ++bit) {
            if (crc & 1UL) crc = (crc >> 1) ^ 0xEDB88320UL;
            else crc >>= 1;
        }
    }
    return ~crc;
}

int enderecoSlot(uint8_t slot) {
    return ENDERECO_SLOT_A + (int)slot * (int)sizeof(RegistroEEPROM);
}

bool lerSlot(uint8_t slot, RegistroEEPROM& registro) {
    const int endereco = enderecoSlot(slot);
    if (endereco < 0 || endereco + (int)sizeof(registro) > EEPROM.length())
        return false;

    EEPROM.get(endereco, registro);

    if (registro.magic != MAGIC_EEPROM ||
        registro.versao != VERSAO_EEPROM ||
        registro.tamanho != sizeof(RegistroEEPROM)) return false;

    const uint32_t crcCalculado = calcularCRC32(
        reinterpret_cast<const uint8_t*>(&registro),
        offsetof(RegistroEEPROM, crc)
    );
    return crcCalculado == registro.crc;
}

bool sequenciaMaisNova(uint32_t a, uint32_t b) {
    return (int32_t)(a - b) > 0;
}

bool floatNoIntervalo(float valor, float minimo, float maximo) {
    return isfinite(valor) && valor >= minimo && valor <= maximo;
}

bool validarConfiguracoes(const ConfiguracoesPersistidas& d) {
    if (!floatNoIntervalo(d.pesoAlvoGramas, 0.0f, 500.0f) ||
        !floatNoIntervalo(d.fatorCalibracao, -10000000.0f, 10000000.0f) ||
        fabs(d.fatorCalibracao) < 0.001f ||
        d.tempoMaximoAlimentacaoMs > 120000UL ||
        d.tempoMaximoBombaMs > 120000UL ||
        d.intervaloLimpezaSegundos == 0 ||
        d.limpezaRegistrada > 1) return false;

    const float cheio[2] = {d.alimentoDistanciaCheioCm, d.aguaDistanciaCheioCm};
    const float vazio[2] = {d.alimentoDistanciaVazioCm, d.aguaDistanciaVazioCm};
    const float minimo[2] = {d.alimentoNivelMinimoPercentual, d.aguaNivelMinimoPercentual};

    for (uint8_t i = 0; i < 2; ++i) {
        if (!floatNoIntervalo(cheio[i], 2.0f, 400.0f) ||
            !floatNoIntervalo(vazio[i], 2.1f, 400.0f) ||
            vazio[i] <= cheio[i] ||
            !floatNoIntervalo(minimo[i], 0.0f, 100.0f)) return false;
    }

    const HorarioPersistido* agendas[2] = {
        d.agendaAlimentacao, d.agendaHidratacao
    };
    for (uint8_t a = 0; a < 2; ++a) {
        for (uint8_t i = 0; i < MAX_HORARIOS_DIARIOS; ++i) {
            const HorarioPersistido& h = agendas[a][i];
            if (h.ativo > 1 || h.hora > 23 || h.minuto > 59) return false;
            if (!h.ativo) continue;
            for (uint8_t j = i + 1; j < MAX_HORARIOS_DIARIOS; ++j) {
                const HorarioPersistido& outro = agendas[a][j];
                if (outro.ativo && outro.hora == h.hora && outro.minuto == h.minuto)
                    return false;
            }
        }
    }
    return true;
}

void copiarAgendaParaEEPROM(const AgendaDiaria& origem,
                            HorarioPersistido destino[MAX_HORARIOS_DIARIOS]) {
    for (uint8_t i = 0; i < MAX_HORARIOS_DIARIOS; ++i) {
        destino[i].hora = origem.horarios[i].hora;
        destino[i].minuto = origem.horarios[i].minuto;
        destino[i].ativo = origem.horarios[i].ativo ? 1 : 0;
    }
}

void copiarAgendaDaEEPROM(const HorarioPersistido origem[MAX_HORARIOS_DIARIOS],
                          AgendaDiaria& destino) {
    destino.quantidadeHorarios = 0;
    for (uint8_t i = 0; i < MAX_HORARIOS_DIARIOS; ++i) {
        Horario& h = destino.horarios[i];
        h.hora = origem[i].hora;
        h.minuto = origem[i].minuto;
        h.ativo = origem[i].ativo != 0;
        // Registro de execucao e pendencia sao volateis e sempre reiniciam.
        h.registro.dataReferencia = 0;
        h.registro.horarioExecucaoEpoch = 0;
        h.registro.estado = EstadoExecucao::AGUARDANDO;
        h.pendencia.dataReferencia = 0;
        h.pendencia.horarioOcorrenciaEpoch = 0;
        h.pendencia.estado = EstadoExecucao::AGUARDANDO;
        h.pendencia.ativa = false;
        if (h.ativo) ++destino.quantidadeHorarios;
    }
}

void criarSnapshot(const DadosSistema& dados, ConfiguracoesPersistidas& p) {
    memset(&p, 0, sizeof(p));
    p.pesoAlvoGramas = dados.configuracaoAlimentacao.pesoAlvoGramas;
    p.fatorCalibracao = dados.configuracaoAlimentacao.fatorCalibracao;
    p.tempoMaximoAlimentacaoMs = dados.configuracaoAlimentacao.tempoMaximoMs;
    p.tempoMaximoBombaMs = dados.configuracaoHidratacao.tempoMaximoBombaMs;

    p.alimentoDistanciaCheioCm = dados.reservatorioAlimento.configuracao.distanciaNivelCheioCm;
    p.alimentoDistanciaVazioCm = dados.reservatorioAlimento.configuracao.distanciaNivelVazioCm;
    p.alimentoNivelMinimoPercentual = dados.reservatorioAlimento.configuracao.nivelMinimoPercentual;
    p.aguaDistanciaCheioCm = dados.reservatorioAgua.configuracao.distanciaNivelCheioCm;
    p.aguaDistanciaVazioCm = dados.reservatorioAgua.configuracao.distanciaNivelVazioCm;
    p.aguaNivelMinimoPercentual = dados.reservatorioAgua.configuracao.nivelMinimoPercentual;

    p.intervaloLimpezaSegundos = dados.configuracaoManutencao.intervaloLimpezaSegundos;
    p.ultimaLimpezaEpoch = dados.manutencao.ultimaLimpezaEpoch;
    p.limpezaRegistrada = dados.manutencao.limpezaRegistrada ? 1 : 0;

    copiarAgendaParaEEPROM(dados.agendaAlimentacao, p.agendaAlimentacao);
    copiarAgendaParaEEPROM(dados.agendaHidratacao, p.agendaHidratacao);
}

void aplicarSnapshot(const ConfiguracoesPersistidas& p, DadosSistema& dados) {
    dados.configuracaoAlimentacao.pesoAlvoGramas = p.pesoAlvoGramas;
    dados.configuracaoAlimentacao.fatorCalibracao = p.fatorCalibracao;
    dados.configuracaoAlimentacao.tempoMaximoMs = p.tempoMaximoAlimentacaoMs;
    dados.configuracaoHidratacao.tempoMaximoBombaMs = p.tempoMaximoBombaMs;

    dados.reservatorioAlimento.configuracao.distanciaNivelCheioCm = p.alimentoDistanciaCheioCm;
    dados.reservatorioAlimento.configuracao.distanciaNivelVazioCm = p.alimentoDistanciaVazioCm;
    dados.reservatorioAlimento.configuracao.nivelMinimoPercentual = p.alimentoNivelMinimoPercentual;
    dados.reservatorioAgua.configuracao.distanciaNivelCheioCm = p.aguaDistanciaCheioCm;
    dados.reservatorioAgua.configuracao.distanciaNivelVazioCm = p.aguaDistanciaVazioCm;
    dados.reservatorioAgua.configuracao.nivelMinimoPercentual = p.aguaNivelMinimoPercentual;

    dados.configuracaoManutencao.intervaloLimpezaSegundos = p.intervaloLimpezaSegundos;
    dados.manutencao.ultimaLimpezaEpoch = p.ultimaLimpezaEpoch;
    dados.manutencao.limpezaRegistrada = p.limpezaRegistrada != 0;
    dados.manutencao.limpezaPendente = false;
    dados.manutencao.alertaAtivo = false;

    copiarAgendaDaEEPROM(p.agendaAlimentacao, dados.agendaAlimentacao);
    copiarAgendaDaEEPROM(p.agendaHidratacao, dados.agendaHidratacao);
}
}

bool carregarDadosPersistidos(DadosSistema& dados) {
    RegistroEEPROM a{}, b{};
    const bool validoA = lerSlot(0, a);
    const bool validoB = lerSlot(1, b);

    if (!validoA && !validoB) return false;

    const RegistroEEPROM& escolhido =
        (!validoA) ? b : (!validoB) ? a :
        (sequenciaMaisNova(b.sequencia, a.sequencia) ? b : a);

    if (!validarConfiguracoes(escolhido.dados)) return false;
    aplicarSnapshot(escolhido.dados, dados);
    return true;
}

bool salvarDadosPersistidos(const DadosSistema& dados) {
    RegistroEEPROM atualA{}, atualB{};
    const bool validoA = lerSlot(0, atualA);
    const bool validoB = lerSlot(1, atualB);

    uint32_t novaSequencia = 1;
    uint8_t destino = 0;
    if (validoA && validoB) {
        const bool bMaisNovo = sequenciaMaisNova(atualB.sequencia, atualA.sequencia);
        novaSequencia = (bMaisNovo ? atualB.sequencia : atualA.sequencia) + 1UL;
        destino = bMaisNovo ? 0 : 1;
    } else if (validoA) {
        novaSequencia = atualA.sequencia + 1UL;
        destino = 1;
    } else if (validoB) {
        novaSequencia = atualB.sequencia + 1UL;
        destino = 0;
    }

    RegistroEEPROM novo{};
    novo.magic = MAGIC_EEPROM;
    novo.versao = VERSAO_EEPROM;
    novo.tamanho = sizeof(RegistroEEPROM);
    novo.sequencia = novaSequencia;
    criarSnapshot(dados, novo.dados);

    if (!validarConfiguracoes(novo.dados)) return false;
    novo.crc = calcularCRC32(
        reinterpret_cast<const uint8_t*>(&novo),
        offsetof(RegistroEEPROM, crc)
    );

    const int endereco = enderecoSlot(destino);
    if (endereco + (int)sizeof(novo) > EEPROM.length()) return false;

    // Invalida primeiro o slot de destino. O magic so e escrito por ultimo.
    uint32_t magicInvalido = 0;
    EEPROM.put(endereco, magicInvalido);
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&novo);
    for (size_t i = sizeof(novo.magic); i < sizeof(novo); ++i)
        EEPROM.update(endereco + (int)i, bytes[i]);
    EEPROM.put(endereco, novo.magic);

    RegistroEEPROM verificacao{};
    return lerSlot(destino, verificacao) &&
           verificacao.sequencia == novo.sequencia;
}

bool limparDadosPersistidos() {
    const uint32_t invalido = 0;
    EEPROM.put(enderecoSlot(0), invalido);
    EEPROM.put(enderecoSlot(1), invalido);
    return true;
}
